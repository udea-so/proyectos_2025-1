#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>    // Para kthread
#include <linux/delay.h>      // Para msleep()

// Funciones exportadas por otros módulos
extern int read_simulated_temperature(void);       // sensor_sim.ko
extern void led_on(int row, int col);              // led_matrix.ko
extern void clear_matrix(void);                    // led_matrix.ko
extern void print_matrix_state(void);              // led_matrix.ko
extern void print_gpio_matrix_state(void);         // led_matrix.ko

// Hilo principal del controlador
static struct task_struct *control_thread;

// Enciende una cantidad específica de LEDs desde la esquina superior izquierda
static void encender_leds(int cantidad) {
    int row, col, count = 0;
    for (row = 0; row < 4 && count < cantidad; row++) {
        for (col = 0; col < 4 && count < cantidad; col++) {
            led_on(row, col);
            count++;
        }
    }
}

/*
 * Función ejecutada por el hilo del kernel.
 * Lee continuamente la temperatura simulada y actualiza la matriz de LEDs
 * únicamente si el valor cambia de rango: frío, templado o caliente.
 */
static int controlador_thread_fn(void *data) {
    int last_range = -1;

    while (!kthread_should_stop()) {
        int temp = read_simulated_temperature();

        // Determinar el rango de temperatura
        int current_range;
        if (temp < 25)
            current_range = 0;  // Frío
        else if (temp < 30)
            current_range = 1;  // Templado
        else
            current_range = 2;  // Caliente

        // Solo actualizar la matriz si el rango ha cambiado
        if (current_range != last_range) {
            last_range = current_range;

            printk(KERN_INFO "controlador: Temperatura = %d°C (nuevo rango: %d)\n", temp, current_range);

            clear_matrix();

            if (current_range == 0)
                encender_leds(4);
            else if (current_range == 1)
                encender_leds(8);
            else
                encender_leds(12);

            print_matrix_state();
            print_gpio_matrix_state();
        }

        // Pausa pequeña entre iteraciones
        msleep(500);
    }

    return 0;
}

// Inicialización del módulo
static int __init controlador_init(void) {
    printk(KERN_INFO "controlador: Iniciando hilo de control reactivo por rango...\n");

    control_thread = kthread_run(controlador_thread_fn, NULL, "controlador_hilo");

    if (IS_ERR(control_thread)) {
        printk(KERN_ERR "controlador: Error al crear el hilo\n");
        return PTR_ERR(control_thread);
    }

    return 0;
}

// Finalización del módulo
static void __exit controlador_exit(void) {
    printk(KERN_INFO "controlador: Deteniendo hilo...\n");

    if (control_thread)
        kthread_stop(control_thread);

    clear_matrix();  // Apaga la matriz al salir
    printk(KERN_INFO "controlador: Módulo descargado\n");
}

module_init(controlador_init);
module_exit(controlador_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edison Zapata");
MODULE_DESCRIPTION("Controlador reactivo por rangos que enciende una matriz de LEDs según la temperatura simulada");
