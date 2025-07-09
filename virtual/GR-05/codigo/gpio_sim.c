#include <linux/module.h>   // Funcionalidad base de módulos
#include <linux/kernel.h>   // Para printk()
#include <linux/init.h>     // Macros __init y __exit
#include <linux/slab.h>     // Para funciones de asignación de memoria (aunque no se usa aquí)

#define MAX_SIMULATED_GPIOS 32  // Cantidad máxima de GPIOs virtuales disponibles

// ============================ ESTRUCTURA INTERNA =============================

/**
 * struct gpio_sim_pin - Representa un pin GPIO virtual
 * @number:     Número identificador del GPIO
 * @direction:  Dirección del pin (0=input, 1=output)
 * @value:      Valor lógico del pin (0 o 1)
 * @requested:  Bandera que indica si el pin ha sido solicitado por algún módulo
 */
struct gpio_sim_pin {
    int number;
    int direction;      // 0 = entrada, 1 = salida
    int value;          // Estado lógico
    bool requested;     // true si está en uso
};

// Tabla que almacena todos los GPIOs simulados
static struct gpio_sim_pin gpio_table[MAX_SIMULATED_GPIOS];

// ============================= FUNCIONES EXPORTADAS ==========================

/**
 * gpio_sim_request - Solicita un GPIO virtual (como si se "reservara")
 * @gpio: número del GPIO
 * @label: etiqueta para fines de debug
 * Retorna 0 si fue exitoso, -EINVAL si el número es inválido, -EBUSY si ya está en uso.
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
 * gpio_sim_free - Libera un GPIO reservado
 * @gpio: número del GPIO a liberar
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
 * @gpio: número del GPIO
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
 * gpio_sim_direction_output - Configura un GPIO como salida con un valor inicial
 * @gpio: número del GPIO
 * @value: valor lógico inicial (0 o 1)
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
 * gpio_sim_get_value - Lee el valor actual de un GPIO
 * @gpio: número del GPIO
 * Retorna 0 o 1 si es válido, -EINVAL si no se ha solicitado.
 */
int gpio_sim_get_value(unsigned gpio) {
    if (gpio >= MAX_SIMULATED_GPIOS || !gpio_table[gpio].requested)
        return -EINVAL;

    printk(KERN_INFO "gpio_sim: gpio_sim_get_value(%d) = %d\n", gpio, gpio_table[gpio].value);
    return gpio_table[gpio].value;
}
EXPORT_SYMBOL(gpio_sim_get_value);

/**
 * gpio_sim_set_value - Modifica el valor lógico de un GPIO (si es salida)
 * @gpio: número del GPIO
 * @value: nuevo valor (0 o 1)
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

// ======================== FUNCIONES DE INICIO Y SALIDA =======================

/**
 * gpio_sim_init - Función llamada al insertar el módulo
 */
static int __init gpio_sim_init(void) {
    memset(gpio_table, 0, sizeof(gpio_table));  // Limpia todos los estados
    printk(KERN_INFO "gpio_sim: Módulo de GPIOs virtuales cargado\n");
    return 0;
}

/**
 * gpio_sim_exit - Función llamada al remover el módulo
 */
static void __exit gpio_sim_exit(void) {
    printk(KERN_INFO "gpio_sim: Módulo descargado\n");
}

module_init(gpio_sim_init);
module_exit(gpio_sim_exit);

// ================================ METADATOS ==================================

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edison Zapata");
MODULE_DESCRIPTION("Simulación de pines GPIO en el kernel");
