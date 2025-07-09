#!/bin/bash

# Configuración
LOG_FILE="test_results.log"
MONITOR_BIN="./monitor"
CONFIG_FILE="config.ini"
STRESS_DURATION=20  # segundos

# Funciones auxiliares
log_result() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1" >> "$LOG_FILE"
}

run_test() {
    local test_name=$1
    local command=$2
    
    log_result "INICIANDO PRUEBA: $test_name"
    log_result "Comando: $command"
    
    # Ejecutar monitor en background
    "$MONITOR_BIN" &
    MONITOR_PID=$!
    
    # Ejecutar prueba
    eval "$command"
    
    # Finalizar monitor
    kill -SIGTERM "$MONITOR_PID"
    wait "$MONITOR_PID"
    
    log_result "PRUEBA COMPLETADA: $test_name"
    echo "---------------------------------" >> "$LOG_FILE"
}

# 1. Prueba de CPU
run_test "SOBRECARGA CPU" "stress-ng --cpu 4 --timeout ${STRESS_DURATION}s"

# 2. Prueba de Memoria
run_test "SOBRECARGA MEMORIA" "stress-ng --vm 4 --vm-bytes 1G --timeout ${STRESS_DURATION}s"

# 3. Prueba de Procesos
run_test "MULTIPLES PROCESOS" "stress-ng --fork 100 --timeout ${STRESS_DURATION}s"

# 4. Prueba Combinada
run_test "CARGA COMBINADA" "stress-ng --cpu 2 --vm 2 --vm-bytes 512M --fork 50 --timeout ${STRESS_DURATION}s"

# 5. Prueba de Umbrales
echo -e "[thresholds]\ncpu_alert = 0.1\nmem_alert = 0.1" > "$CONFIG_FILE"
run_test "UMBRALES BAJOS" "stress-ng --cpu 1 --timeout 10s"

# 6. Verificación de Alertas
check_alerts() {
    local alert_log="/var/log/monitor.log"
    if grep -q "ALERTA" "$alert_log"; then
        log_result "ALERTAS GENERADAS CORRECTAMENTE"
        grep "ALERTA" "$alert_log" | tail -n 3 >> "$LOG_FILE"
    else
        log_result "ERROR: No se generaron alertas"
    fi
}

# 7. Prueba de Memoria con Valgrind
valgrind_test() {
    log_result "INICIANDO PRUEBA DE MEMORIA (VALGRIND)"
    valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all \
             --log-file="valgrind_report.log" "$MONITOR_BIN" &
    sleep 10
    kill $!
    wait $!
    
    if grep -q "no leaks are possible" "valgrind_report.log"; then
        log_result "VALGRIND: Sin fugas de memoria"
    else
        log_result "VALGRIND: Se detectaron fugas - ver valgrind_report.log"
    fi
}

valgrind_test
check_alerts

echo "Todas las pruebas completadas. Resultados en $LOG_FILE"