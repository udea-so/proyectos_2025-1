#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

#define MAX_SIMULATED_GPIOS 32

// Estructura que representa un GPIO virtual
struct gpio_sim_pin {
    int number;
    int direction;  // 0 = input, 1 = output
    int value;
    bool requested;
};

static struct gpio_sim_pin gpio_table[MAX_SIMULATED_GPIOS];

/**
 * gpio_sim_request - Solicita un GPIO virtual
 */
int gpio_sim_request(unsigned gpio, const char *label) {
    if (gpio >= MAX_SIMULATED_GPIOS)
        return -EINVAL;
    if (gpio_table[gpio].requested)
        return -EBUSY;

    gpio_table[gpio].requested = true;
    gpio_table[gpio].number = gpio;
    gpio_table[gpio].direction = 0;
    gpio_table[gpio].value = 0;
    printk(KERN_INFO "gpio_sim: gpio_sim_request(%d) OK [%s]\n", gpio, label);
    return 0;
}
EXPORT_SYMBOL(gpio_sim_request);

/**
 * gpio_sim_free - Libera un GPIO
 */
void gpio_sim_free(unsigned gpio) {
    if (gpio >= MAX_SIMULATED_GPIOS)
        return;
    gpio_table[gpio].requested = false;
    gpio_table[gpio].value = 0;
    printk(KERN_INFO "gpio_sim: gpio_sim_free(%d)\n", gpio);
}
EXPORT_SYMBOL(gpio_sim_free);

/**
 * gpio_sim_direction_input - Configura un GPIO como entrada
 */
int gpio_sim_direction_input(unsigned gpio) {
    if (gpio >= MAX_SIMULATED_GPIOS || !gpio_table[gpio].requested)
        return -EINVAL;
    gpio_table[gpio].direction = 0;
    printk(KERN_INFO "gpio_sim: gpio_sim_direction_input(%d)\n", gpio);
    return 0;
}
EXPORT_SYMBOL(gpio_sim_direction_input);

/**
 * gpio_sim_direction_output - Configura un GPIO como salida
 */
int gpio_sim_direction_output(unsigned gpio, int value) {
    if (gpio >= MAX_SIMULATED_GPIOS || !gpio_table[gpio].requested)
        return -EINVAL;
    gpio_table[gpio].direction = 1;
    gpio_table[gpio].value = value ? 1 : 0;
    printk(KERN_INFO "gpio_sim: gpio_sim_direction_output(%d, %d)\n", gpio, value);
    return 0;
}
EXPORT_SYMBOL(gpio_sim_direction_output);

/**
 * gpio_sim_get_value - Lee el valor del GPIO
 */
int gpio_sim_get_value(unsigned gpio) {
    if (gpio >= MAX_SIMULATED_GPIOS || !gpio_table[gpio].requested)
        return -EINVAL;
    printk(KERN_INFO "gpio_sim: gpio_sim_get_value(%d) = %d\n", gpio, gpio_table[gpio].value);
    return gpio_table[gpio].value;
}
EXPORT_SYMBOL(gpio_sim_get_value);

/**
 * gpio_sim_set_value - Cambia el valor de salida
 */
void gpio_sim_set_value(unsigned gpio, int value) {
    if (gpio >= MAX_SIMULATED_GPIOS || !gpio_table[gpio].requested)
        return;
    if (gpio_table[gpio].direction != 1)
        return;
    gpio_table[gpio].value = value ? 1 : 0;
    printk(KERN_INFO "gpio_sim: gpio_sim_set_value(%d, %d)\n", gpio, value);
}
EXPORT_SYMBOL(gpio_sim_set_value);

/**
 * Inicialización del módulo
 */
static int __init gpio_sim_init(void) {
    memset(gpio_table, 0, sizeof(gpio_table));
    printk(KERN_INFO "gpio_sim: Módulo de GPIOs virtuales cargado\n");
    return 0;
}

/**
 * Limpieza del módulo
 */
static void __exit gpio_sim_exit(void) {
    printk(KERN_INFO "gpio_sim: Módulo descargado\n");
}

module_init(gpio_sim_init);
module_exit(gpio_sim_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edison Zapata");
MODULE_DESCRIPTION("Simulación de pines GPIO en el kernel");
