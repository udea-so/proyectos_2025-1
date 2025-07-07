#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

// Usamos funciones GPIO simuladas
extern int gpio_sim_request(unsigned gpio, const char *label);
extern void gpio_sim_free(unsigned gpio);
extern int gpio_sim_direction_output(unsigned gpio, int value);
extern void gpio_sim_set_value(unsigned gpio, int value);

#define ROWS 4
#define COLS 4

static int row_pins[ROWS] = { 0, 1, 2, 3 };
static int col_pins[COLS] = { 4, 5, 6, 7 };
static int led_state[ROWS][COLS];

void led_on(int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return;

    gpio_sim_set_value(row_pins[row], 1);
    gpio_sim_set_value(col_pins[col], 1);
    led_state[row][col] = 1;

    printk(KERN_INFO "led_matrix: LED encendido en [%d][%d]\n", row, col);
}
EXPORT_SYMBOL(led_on);

void clear_matrix(void) {
    int row, col;
    for (row = 0; row < ROWS; row++)
        gpio_sim_set_value(row_pins[row], 0);
    for (col = 0; col < COLS; col++)
        gpio_sim_set_value(col_pins[col], 0);

    memset(led_state, 0, sizeof(led_state));
    printk(KERN_INFO "led_matrix: Todos los LEDs apagados\n");
}
EXPORT_SYMBOL(clear_matrix);

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

static int __init led_matrix_init(void) {
    int i, ret;

    for (i = 0; i < ROWS; i++) {
        ret = gpio_sim_request(row_pins[i], "row_gpio");
        if (ret) {
            printk(KERN_ERR "led_matrix: fallo al solicitar GPIO fila %d\n", row_pins[i]);
            return ret;
        }
        gpio_sim_direction_output(row_pins[i], 0);
    }

    for (i = 0; i < COLS; i++) {
        ret = gpio_sim_request(col_pins[i], "col_gpio");
        if (ret) {
            printk(KERN_ERR "led_matrix: fallo al solicitar GPIO columna %d\n", col_pins[i]);
            return ret;
        }
        gpio_sim_direction_output(col_pins[i], 0);
    }

    clear_matrix();
    printk(KERN_INFO "led_matrix: Módulo cargado\n");
    return 0;
}

static void __exit led_matrix_exit(void) {
    int i;
    clear_matrix();
    for (i = 0; i < ROWS; i++)
        gpio_sim_free(row_pins[i]);
    for (i = 0; i < COLS; i++)
        gpio_sim_free(col_pins[i]);
    printk(KERN_INFO "led_matrix: Módulo descargado\n");
}

extern int gpio_sim_get_value(unsigned gpio);

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


module_init(led_matrix_init);
module_exit(led_matrix_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edison Zapata");
MODULE_DESCRIPTION("Matriz de LEDs 4x4 controlada con GPIOs simulados");
