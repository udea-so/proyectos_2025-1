#ifndef PROCESS_MONITOR_H  
#define PROCESS_MONITOR_H  

#include <ncurses.h>  
#include <dirent.h>  
#include <unistd.h>  
#include <string.h>  
#include "alerts.h"

#define MAX_PROCESSES 512
#define MAX_ALERTS 10
#define COLOR_YELLOW 3
#define COLOR_CYAN   6

typedef struct {  
    pid_t pid;  
    char name[256];  
    float cpu_usage;  
    unsigned long memory_kb;
    unsigned long utime;  // Campos nuevos
    unsigned long stime;
} ProcessInfo;

typedef struct {  
    char message[256];
    pid_t pid;
} Alert;

// Funciones principales  
void iniciar_monitor();
int obtener_procesos(ProcessInfo *processes);
void dibujar_ui(ProcessInfo *processes, int count, Alert *alerts, int alert_count);
void comprobar_umbrales(ProcessInfo *processes, int count, Alert *alerts, int *alert_count);
float get_cpu_usage_for_process(pid_t pid);
float get_total_memoria();
float get_uso_cpu();
float get_uso_memoria();
unsigned long get_procesos_memoria(pid_t pid);
char* get_progreso_barra(float percent);

#endif  