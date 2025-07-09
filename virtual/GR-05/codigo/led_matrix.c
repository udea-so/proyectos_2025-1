#include <linux/module.h>    // Requerido para la creación de módulos
#include <linux/kernel.h>    // Para printk()
#include <linux/init.h>      // Para macros __init y __exit
#include <linux/slab.h>      // Para memset()

// ================= FUNCIONES EXTERNAS DE GPIO SIMULADO =======================

// Declaración de funciones proporcionadas por el módulo gpio_sim.ko
extern int gpio_sim_request(unsigned gpio, const char *label);           // Solicita un GPIO virtual
extern void gpio_sim_free(unsigned gpio);                                 // Libera un GPIO virtual
extern int gpio_sim_direction_output(unsigned gpio, int value);           // Configura dirección como salida
extern void gpio_sim_set_value(unsigned gpio, int value);                 // Establece valor lógico
extern int gpio_sim_get_value(unsigned gpio);                             // Obtiene el valor actual

// ===================== DEFINICIÓN DE MATRIZ DE LEDS ==========================

#define ROWS 4
#define COLS 4

// Mapeo GPIOs: filas usan GPIOs 0-3 y columnas 4-7
static int row_pins[ROWS] = { 0, 1, 2, 3 };
static int col_pins[COLS] = { 4, 5, 6, 7 };

// Estado actual de los LEDs (1=encendido, 0=apagado)
static int led_state[ROWS][COLS];

// ========================== FUNCIONES EXPORTADAS =============================

/**
 * led_on - Enciende un LED específico en la matriz.
 * @row: Fila del LED (0 a 3)
 * @col: Columna del LED (0 a 3)
 */
void led_on(int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return;

    gpio_sim_set_value(row_pins[row], 1);    // Activar pin de fila
    gpio_sim_set_value(col_pins[col], 1);    // Activar pin de columna
    led_state[row][col] = 1;                 // Guardar estado encendido

    printk(KERN_INFO "led_matrix: LED encendido en [%d][%d]\n", row, col);
}
EXPORT_SYMBOL(led_on);

/**
 * clear_matrix - Apaga todos los LEDs de la matriz.
 */
void clear_matrix(void) {
    int row, col;

    // Apagar pines físicos (GPIOs) de filas y columnas
    for (row = 0; row < ROWS; row++)
        gpio_sim_set_value(row_pins[row], 0);

    for (col = 0; col < COLS; col++)
        gpio_sim_set_value(col_pins[col], 0);

    // Actualizar estado lógico de la matriz
    memset(led_state, 0, sizeof(led_state));

    printk(KERN_INFO "led_matrix: Todos los LEDs apagados\n");
}
EXPORT_SYMBOL(clear_matrix);

/**
 * print_matrix_state - Muestra el estado lógico de los LEDs.
 */
void print_matrix_state(void) {
    int row, col;

    printk(KERN_INFO "led_matrix: Estado actual de la matriz:");
    for (row = 0; row < ROWS; row++) {
        printk(KERN_CONT "[%d] ", row);
        for (col = 0; col < COLS; col++) {
            printk(KERN_CONT "%d ", led_state[row][col]);
        }
        printk(KERN_CONT "\n");
    }
}
EXPORT_SYMBOL(print_matrix_state);

/**
 * print_gpio_matrix_state - Imprime los estados de los GPIOs que controlan la matriz.
 * Útil para comprobar si los pines están en alto o bajo.
 */
void print_gpio_matrix_state(void) {
    int i;

    printk(KERN_INFO "led_matrix: Estado de GPIOs fila:");
    for (i = 0; i < ROWS; i++) {
        int val = gpio_sim_get_value(row_pins[i]);
        printk(KERN_INFO "Fila GPIO[%d] = %d", row_pins[i], val);
    }

    printk(KERN_INFO "led_matrix: Estado de GPIOs columna:");
    for (i = 0; i < COLS; i++) {
        int val = gpio_sim_get_value(col_pins[i]);
        printk(KERN_INFO "Columna GPIO[%d] = %d", col_pins[i], val);
    }
}
EXPORT_SYMBOL(print_gpio_matrix_state);

// ======================== INICIALIZACIÓN DEL MÓDULO ==========================

/**
 * led_matrix_init - Función de inicialización del módulo.
 * Solicita los GPIOs y los configura como salidas apagadas.
 */
static int __init led_matrix_init(void) {
    int i, ret;

    // Solicitar y configurar los GPIOs de filas
    for (i = 0; i < ROWS; i++) {
        ret = gpio_sim_request(row_pins[i], "row_gpio");
        if (ret) {
            printk(KERN_ERR "led_matrix: fallo al solicitar GPIO fila %d\n", row_pins[i]);
            return ret;
        }
        gpio_sim_direction_output(row_pins[i], 0);  // Inicializar apagado
    }

    // Solicitar y configurar los GPIOs de columnas
    for (i = 0; i < COLS; i++) {
        ret = gpio_sim_request(col_pins[i], "col_gpio");
        if (ret) {
            printk(KERN_ERR "led_matrix: fallo al solicitar GPIO columna %d\n", col_pins[i]);
            return ret;
        }
        gpio_sim_direction_output(col_pins[i], 0);  // Inicializar apagado
    }

    // Apagar todos los LEDs
    clear_matrix();

    printk(KERN_INFO "led_matrix: Módulo cargado\n");
    return 0;
}

/**
 * led_matrix_exit - Función de limpieza al descargar el módulo.
 * Apaga la matriz y libera todos los GPIOs usados.
 */
static void __exit led_matrix_exit(void) {
    int i;

    clear_matrix();  // Apaga todos los LEDs por seguridad

    // Liberar todos los GPIOs usados
    for (i = 0; i < ROWS; i++)
        gpio_sim_free(row_pins[i]);

    for (i = 0; i < COLS; i++)
        gpio_sim_free(col_pins[i]);

    printk(KERN_INFO "led_matrix: Módulo descargado\n");
}

// ============================ METADATOS DEL MÓDULO ===========================

module_init(led_matrix_init);
module_exit(led_matrix_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edison Zapata");
MODULE_DESCRIPTION("Matriz de LEDs 4x4 controlada con GPIOs simulados");
