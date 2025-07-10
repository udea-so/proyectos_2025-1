from fastapi import FastAPI
from prometheus_client import start_http_server
from app.routes import metrics, profiling, system
from app.services import cpu_monitor, memory_monitor, io_monitor, system_monitor, email_alert

import threading
import time
import logging

# Configuración de logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
)

app = FastAPI(title="Performance API")
app.include_router(metrics.router)
app.include_router(profiling.router)
app.include_router(system.router)

# Iniciar servidor Prometheus en otro puerto
start_http_server(8001)

# Estado para evitar correos duplicados por métrica
estado_alertas = {
    "cpu": False,
    "memoria": False,
    "disco": False
}

def verificar_y_alertar(nombre, obtener_valor_func, extraer_percent=False, umbral=80):
    try:
        valor = obtener_valor_func()
        porcentaje = valor["percent"] if extraer_percent and isinstance(valor, dict) else valor

        if porcentaje > umbral and not estado_alertas[nombre]:
            email_alert.enviar_alerta(
                f"Alerta de {nombre.capitalize()} Alta",
                f"El uso de {nombre.capitalize()} ha superado el {umbral}%: {porcentaje:.2f}%"
            )
            estado_alertas[nombre] = True
        elif porcentaje <= umbral:
            estado_alertas[nombre] = False
    except Exception as e:
        logging.error(f"Error en {nombre}: {e}")

def actualizar_metricas():
    while True:
        verificar_y_alertar("cpu", cpu_monitor.get_cpu_percent)
        verificar_y_alertar("memoria", memory_monitor.get_memory_usage, extraer_percent=True)
        verificar_y_alertar("disco", io_monitor.get_disk_usage, extraer_percent=True)

        try:
            system_monitor.update_system_metrics()
        except Exception as e:
            logging.error(f"Error al actualizar métricas del sistema: {e}")

        time.sleep(5)

# Iniciar hilo en segundo plano
threading.Thread(target=actualizar_metricas, daemon=True).start()
