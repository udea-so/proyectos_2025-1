#include <linux/module.h>        // Necesario para todos los módulos del kernel
#include <linux/kernel.h>        // Para funciones básicas del kernel como printk()
#include <linux/init.h>          // Para macros de inicialización y salida
#include <linux/kthread.h>       // Para creación de hilos del kernel
#include <linux/delay.h>         // Para funciones de retardo como msleep()

// ============================= FUNCIONES EXTERNAS =============================

// Declaración de funciones que se exportan desde otros módulos:
// Estas permiten comunicación entre módulos sin necesidad de duplicar lógica.
extern int read_simulated_temperature(void);       // sensor_sim.ko
extern void led_on(int row, int col);              // led_matrix.ko
extern void clear_matrix(void);                    // led_matrix.ko
extern void print_matrix_state(void);              // led_matrix.ko
extern void print_gpio_matrix_state(void);         // led_matrix.ko

// =========================== VARIABLES DEL MÓDULO ============================

// Estructura que representa el hilo del kernel que se ejecutará en segundo plano
static struct task_struct *control_thread;

// ============================= FUNCIONES INTERNAS ============================

/**
 * encender_leds - Enciende una cantidad dada de LEDs, empezando desde [0][0].
 * @cantidad: número total de LEDs que se deben encender en la matriz 4x4.
 */
static void encender_leds(int cantidad) {
    int row, col, count = 0;

    // Recorre la matriz en orden fila-columna y enciende LEDs hasta alcanzar el límite.
    for (row = 0; row < 4 && count < cantidad; row++) {
        for (col = 0; col < 4 && count < cantidad; col++) {
            led_on(row, col);
            count++;
        }
    }
}

/**
 * controlador_thread_fn - Función del hilo que se ejecuta en bucle dentro del kernel.
 * Se encarga de leer la temperatura simulada y actualizar la matriz de LEDs
 * solo si la temperatura cambia de categoría (frío, templado, caliente).
 */
static int controlador_thread_fn(void *data) {
    int last_range = -1;  // Guarda el rango anterior para evitar cambios innecesarios

    while (!kthread_should_stop()) {
        int temp = read_simulated_temperature();  // Leer temperatura actual

        // Clasificar la temperatura en uno de tres rangos
        int current_range;
        if (temp < 25)
            current_range = 0;  // Frío
        else if (temp < 30)
            current_range = 1;  // Templado
        else
            current_range = 2;  // Caliente

        // Solo actualizar si el rango ha cambiado desde la última lectura
        if (current_range != last_range) {
            last_range = current_range;

            printk(KERN_INFO "controlador: Temperatura = %d°C (nuevo rango: %d)\n", temp, current_range);

            clear_matrix();  // Apagar todos los LEDs antes de encender los nuevos

            // Encender LEDs según el rango de temperatura
            if (current_range == 0)
                encender_leds(4);   // Frío
            else if (current_range == 1)
                encender_leds(8);   // Templado
            else
                encender_leds(12);  // Caliente

            // Mostrar el estado actual de la matriz y los pines GPIO involucrados
            print_matrix_state();
            print_gpio_matrix_state();
        }

        msleep(500);  // Esperar medio segundo antes de la siguiente iteración
    }

    return 0;
}

// ======================== FUNCIONES DEL MÓDULO KERNEL ========================

/**
 * controlador_init - Función que se ejecuta cuando se carga el módulo.
 * Crea un hilo del kernel que realiza el monitoreo de temperatura.
 */
static int __init controlador_init(void) {
    printk(KERN_INFO "controlador: Iniciando hilo de control reactivo por rango...\n");

    control_thread = kthread_run(controlador_thread_fn, NULL, "controlador_hilo");

    if (IS_ERR(control_thread)) {
        printk(KERN_ERR "controlador: Error al crear el hilo\n");
        return PTR_ERR(control_thread);
    }

    return 0;
}

/**
 * controlador_exit - Función que se ejecuta al descargar el módulo.
 * Detiene el hilo y apaga la matriz de LEDs.
 */
static void __exit controlador_exit(void) {
    printk(KERN_INFO "controlador: Deteniendo hilo...\n");

    if (control_thread)
        kthread_stop(control_thread);  // Solicita la detención del hilo

    clear_matrix();  // Apaga todos los LEDs por seguridad
    printk(KERN_INFO "controlador: Módulo descargado\n");
}

// ============================ MACROS DEL MÓDULO =============================

module_init(controlador_init);
module_exit(controlador_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edison Zapata");
MODULE_DESCRIPTION("Controlador reactivo por rangos que enciende una matriz de LEDs según la temperatura simulada");
