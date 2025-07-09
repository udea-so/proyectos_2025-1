#include "process_monitor.h"
#include "alerts.h"
#include <signal.h>

int main() {
    Config config;
    load_config(&config);
    iniciar_monitor();

    Alert alerts[MAX_ALERTS] = {0};
    int alert_count = 0;

    while (1) {
        ProcessInfo processes[MAX_PROCESSES];
        int count = obtener_procesos(processes);
        
        // Calcular uso de CPU para cada proceso
        for (int i = 0; i < count; i++) {
            processes[i].cpu_usage = get_cpu_usage_for_process(processes[i].pid);
        }
        
        comprobar_umbrales(processes, count, alerts, &alert_count);
        dibujar_ui(processes, count, alerts, alert_count);
        napms(2000); // Esperar 2 segundos antes de actualizar
    }

    endwin();
    return 0;
}