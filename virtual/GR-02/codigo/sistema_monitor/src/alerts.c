#include "alerts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <syslog.h>
#include <unistd.h>

// Cargar configuración desde config.ini
void load_config(Config *config) {
    FILE *file = fopen("config.ini", "r");
    if (!file) {
        // Valores por defecto
        config->cpu_threshold = 0.1;   // Umbral de CPU por defecto
        config->mem_threshold = 0.1;   // Umbral de memoria por defecto
        strcpy(config->log_path, "/var/log/monitor.log");
        config->admin_pid = getppid(); // PID del proceso padre por defecto
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "cpu_alert")) {
            sscanf(line, "cpu_alert = %f", &config->cpu_threshold);
        } else if (strstr(line, "mem_alert")) {
            sscanf(line, "mem_alert = %f", &config->mem_threshold);
        } else if (strstr(line, "log_path")) {
            sscanf(line, "log_path = %255s", config->log_path);
        } else if (strstr(line, "admin_pid")) {
            sscanf(line, "admin_pid = %d", &config->admin_pid);
        }
    }
    fclose(file);
}

// Escribir alerta en log y enviar señal
void trigger_alert(const Config *config, const char *message, pid_t pid, float value) {
    // 1. Loggear con syslog
    openlog("system_monitor", LOG_PID, LOG_USER);
    syslog(LOG_ALERT, "%s (PID: %d, Valor: %.1f%%)", message, pid, value);
    closelog();

    // 2. Escribir en archivo de log local
    FILE *log_file = fopen(config->log_path, "a");
    if (log_file) {
        time_t now = time(NULL);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(log_file, "[%s] ALERTA: %s (PID: %d, Valor: %.1f%%)\n", 
                timestamp, message, pid, value);
        fclose(log_file);
    }

    // 3. Enviar señal al administrador
    kill(config->admin_pid, SIGALRM);
}