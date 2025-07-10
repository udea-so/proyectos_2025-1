#define _GNU_SOURCE
#include "process_monitor.h"  
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

void iniciar_monitor() {  
    initscr();  
    cbreak();  
    noecho();  
    keypad(stdscr, TRUE);  
    start_color(); 

    // Configurar pares de colores (fg, bg)
    init_pair(1, COLOR_RED, COLOR_BLACK);     // Para CPU crítico
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // Para memoria crítica
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   // Para valores normales
    init_pair(4, COLOR_CYAN, COLOR_BLACK);    // Para encabezados
}  

unsigned long get_procesos_memoria(pid_t pid) {
    char path[256], line[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    
    FILE *file = fopen(path, "r");
    if (!file) return 0;
    
    unsigned long mem_kb = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "VmRSS:")) {
            sscanf(line + 6, "%lu", &mem_kb);
            break;
        }
    }
    fclose(file);
    return mem_kb;
}

float get_uso_cpu() {
    static unsigned long last_total = 0, last_idle = 0;
    FILE *file = fopen("/proc/stat", "r");
    if (!file) return 0.0;

    unsigned long user, nice, system, idle;
    fscanf(file, "cpu %lu %lu %lu %lu", &user, &nice, &system, &idle);
    fclose(file);

    unsigned long total = user + nice + system + idle;
    unsigned long diff_total = total - last_total;
    unsigned long diff_idle = idle - last_idle;

    last_total = total;
    last_idle = idle;

    return (diff_total > 0) ? (100.0 * (diff_total - diff_idle) / diff_total) : 0.0;
}

float get_uso_memoria() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) return 0.0;

    unsigned long total = 0, free = 0, buffers = 0, cached = 0;
    char line[128];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal:", 9) == 0) sscanf(line + 9, "%lu", &total);
        if (strncmp(line, "MemFree:", 8) == 0) sscanf(line + 8, "%lu", &free);
        if (strncmp(line, "Buffers:", 8) == 0) sscanf(line + 8, "%lu", &buffers);
        if (strncmp(line, "Cached:", 7) == 0) sscanf(line + 7, "%lu", &cached);
    }
    fclose(file);

    unsigned long used = total - free - buffers - cached;
    return (total > 0) ? (100.0 * used / total) : 0.0;
}

int obtener_procesos(ProcessInfo *processes) {
    DIR *dir = opendir("/proc");
    if (!dir) return 0;
    
    struct dirent *entry;
    int count = 0;
    
    while ((entry = readdir(dir)) && count < MAX_PROCESSES) {
        if (!isdigit(entry->d_name[0])) continue;
        
        pid_t pid = atoi(entry->d_name);
        char path[256];
        snprintf(path, sizeof(path), "/proc/%d/stat", pid);
        
        FILE *stat_file = fopen(path, "r");
        if (!stat_file) continue;
        
        memset(&processes[count], 0, sizeof(ProcessInfo));
        
        // Leer nombre del proceso entre paréntesis
        char *line = NULL;
        size_t len = 0;
        if (getline(&line, &len, stat_file) != -1) {
            char *comm_end = strrchr(line, ')');
            if (comm_end) {
                *comm_end = '\0';
                char *comm_start = strchr(line, '(');
                if (comm_start) {
                    strncpy(processes[count].name, comm_start + 1, sizeof(processes[count].name) - 1);
                }
            }
            
            // Parsear campos importantes
            sscanf(comm_end + 2, "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu",
                   &processes[count].utime, &processes[count].stime);
        }
        free(line);
        fclose(stat_file);
        
        processes[count].pid = pid;
        processes[count].memory_kb = get_procesos_memoria(pid);
        count++;
    }
    closedir(dir);
    return count;
}

float get_total_memoria() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) return 0.0;

    unsigned long total = 0;
    char line[128];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line + 9, "%lu", &total);
            break;
        }
    }
    fclose(file);
    return total * 1024; // Convertir a bytes
}

// Función para crear una barra de progreso (nueva)
char* get_progreso_barra(float percent) {
    static char bar[21]; // 20 caracteres + null terminator
    int filled = (int)(percent * 20 / 100.0);
    
    if (filled > 20) filled = 20;
    if (filled < 0) filled = 0;
    
    for (int i = 0; i < 20; i++) {
        bar[i] = (i < filled) ? '|' : ' ';
    }
    bar[20] = '\0';
    
    return bar;
}

void dibujar_ui(ProcessInfo *processes, int count, Alert *alerts, int alert_count) {
    clear();
    
    Config config;
    load_config(&config);
    float cpu_usage = get_uso_cpu();
    float mem_usage_percent = get_uso_memoria();
    float total_mem_kb = get_total_memoria() / 1024.0; // Convertir a KB
    /************************************************************************************************/
    int running_processes = count; // Esto debería ser calculado realmente
    int row = 0;
    
    int header_color = 3; // Verde por defecto
    if (cpu_usage > config.cpu_threshold) {
        header_color = 1; // Rojo solo si CPU supera umbral
    } else if (mem_usage_percent > config.mem_threshold) {
        header_color = 2; // Amarillo si memoria supera umbral
    }
    
    attron(COLOR_PAIR(header_color) | A_BOLD);
    // 1. Barra de CPU
    mvprintw(++row, 2, "CPU: ");
    mvprintw(row, 10, "%s %.1f%%", get_progreso_barra(cpu_usage), cpu_usage);
    
    // 2. Barra de Memoria
    mvprintw(++row, 2, "Memoria: ");
    mvprintw(row, 10, "%s %.1f%%", get_progreso_barra(mem_usage_percent), mem_usage_percent);

    // 3. Información de procesos
    mvprintw(++row, 2, "Procesos en ejecución: %d", running_processes);

    //4. Umbrales
    mvprintw(++row, 2, "Umbrales: CPU > %.1f%%, MEM > %.1f%%", config.cpu_threshold, config.mem_threshold);
    attroff(COLOR_PAIR(header_color) | A_BOLD);
    
    // 2. Encabezados de la tabla
    attron(COLOR_PAIR(4) | A_BOLD);
    printw("\n\n------------------------------------------------------------\n");
    printw("%-6s %-20s %6s %10s %8s\n", "PID", "Nombre", "%CPU", "Memoria", "Mem%");
    printw("------------------------------------------------------------");
    attroff(COLOR_PAIR(4) | A_BOLD);
    
    // 3. Lista de procesos
    for (int i = 0; i < count; i++) {
        // Calcular porcentaje de memoria correctamente
        float mem_percent = (processes[i].memory_kb / total_mem_kb) * 100.0;
        
        // Determinar color basado en umbrales reales
        int color = 3; // Verde por defecto
        if (processes[i].cpu_usage > config.cpu_threshold) {
            color = 1; // Rojo para CPU alto
        } else if (mem_percent > config.mem_threshold) {
            color = 2; // Amarillo para memoria alta
        }
        
        attron(COLOR_PAIR(color));
        printw("\n%-6d %-20.20s %5.1f%% %8lu KB %6.1f%%\n", 
               processes[i].pid, 
               processes[i].name, 
               processes[i].cpu_usage,
               processes[i].memory_kb,
               mem_percent);
        attroff(COLOR_PAIR(color));
    }
    
    // 4. Sección de alertas
    if (alert_count > 0) {
        attron(COLOR_PAIR(1) | A_BOLD);
        printw("\n--- ALERTAS (%d) ---\n", alert_count);
        for (int i = 0; i < alert_count; i++) {
            printw("! %s\n", alerts[i].message);
        }
        attroff(COLOR_PAIR(1) | A_BOLD);
    }
    
    refresh();
}

float get_cpu_usage_for_process(pid_t pid) {
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static struct {
        pid_t pid;
        unsigned long last_utime;
        unsigned long last_stime;
        struct timespec last_time;
    } proc_data[MAX_PROCESSES] = {0};
    static int initialized = 0;
    
    pthread_mutex_lock(&mutex);
    
    // Inicializar estructura si es necesario
    if (!initialized) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
            proc_data[i].pid = -1;
        }
        initialized = 1;
    }
    
    // Buscar o crear entrada para este PID
    int index = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (proc_data[i].pid == pid || proc_data[i].pid == -1) {
            index = i;
            if (proc_data[i].pid == -1) {
                proc_data[i].pid = pid;
                proc_data[i].last_utime = 0;
                proc_data[i].last_stime = 0;
                proc_data[i].last_time.tv_sec = 0;
                proc_data[i].last_time.tv_nsec = 0;
            }
            break;
        }
    }
    
    if (index == -1) {
        pthread_mutex_unlock(&mutex);
        return 0.0; // No hay espacio para más procesos
    }
    
    // Leer estadísticas actuales del proceso
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    
    FILE *file = fopen(path, "r");
    if (!file) {
        pthread_mutex_unlock(&mutex);
        return 0.0;
    }
    
    unsigned long utime = 0, stime = 0;
    char line[1024];
    
    if (fgets(line, sizeof(line), file)) {
        char *p = strrchr(line, ')');
        if (p) {
            // Saltar hasta después del comm y leer campos 13 (utime) y 14 (stime)
            p += 2;
            int field = 1;
            char *token = strtok(p, " ");
            
            while (token && field < 15) {
                if (field == 13) utime = strtoul(token, NULL, 10);
                if (field == 14) stime = strtoul(token, NULL, 10);
                token = strtok(NULL, " ");
                field++;
            }
        }
    }
    fclose(file);
    
    // Obtener tiempo actual
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    
    float usage = 0.0;
    
    // Solo calcular si tenemos datos previos
    if (proc_data[index].last_time.tv_sec != 0) {
        // Calcular tiempo transcurrido en segundos
        double delta_time = (now.tv_sec - proc_data[index].last_time.tv_sec) +
                          (now.tv_nsec - proc_data[index].last_time.tv_nsec) / 1e9;
        
        // Calcular ticks de CPU usados
        unsigned long delta_utime = utime - proc_data[index].last_utime;
        unsigned long delta_stime = stime - proc_data[index].last_stime;
        unsigned long total_ticks = delta_utime + delta_stime;
        
        // Obtener ticks por segundo del sistema
        long ticks_per_sec = sysconf(_SC_CLK_TCK);
        
        // Calcular porcentaje de uso (evitar división por cero)
        if (delta_time > 0.0001 && ticks_per_sec > 0) {
            usage = 100.0 * ((double)total_ticks / ticks_per_sec) / delta_time;
        }
    }
    
    // Actualizar datos para la próxima llamada
    proc_data[index].last_utime = utime;
    proc_data[index].last_stime = stime;
    proc_data[index].last_time = now;
    
    pthread_mutex_unlock(&mutex);
    
    // Limitar el valor máximo a 1000% para evitar valores absurdos
    return (usage > 1000.0) ? 1000.0 : usage;
}

void comprobar_umbrales(ProcessInfo *processes, int count, Alert *alerts, int *alert_count) {
    Config config;
    load_config(&config);
    float total_mem = get_total_memoria();
    
    *alert_count = 0;
    
    for (int i = 0; i < count && *alert_count < MAX_ALERTS; i++) {
        // Verificar CPU
        if (processes[i].cpu_usage > config.cpu_threshold) {
            snprintf(alerts[*alert_count].message, 256, 
                    "CPU excedido (%.1f%% > %.1f%%) en %.50s (PID: %d)", 
                    processes[i].cpu_usage, config.cpu_threshold,
                    processes[i].name, processes[i].pid);
            (*alert_count)++;
        }
        
        // Verificar memoria (convertir KB a porcentaje)
        float mem_percent = (processes[i].memory_kb / 1024.0) / (total_mem / 1024.0 / 1024.0) * 100.0;
        if (mem_percent > config.mem_threshold) {
            snprintf(alerts[*alert_count].message, 256, 
                    "Memoria excedida (%.1f%% > %.1f%%) en %.50s (PID: %d)", 
                    mem_percent, config.mem_threshold,
                    processes[i].name, processes[i].pid);
            (*alert_count)++;
        }
    }
}