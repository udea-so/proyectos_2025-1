#ifndef ALERTS_H
#define ALERTS_H

#include <sys/types.h>

typedef struct {
    float cpu_threshold;
    float mem_threshold;
    char log_path[256];
    pid_t admin_pid;
} Config;

void load_config(Config *config);
void trigger_alert(const Config *config, const char *message, pid_t pid, float value);

#endif