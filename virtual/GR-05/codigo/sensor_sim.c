#include <linux/module.h>     // Para macros del kernel y carga de módulos
#include <linux/kernel.h>     // Para printk()
#include <linux/init.h>       // Para macros __init y __exit
#include <linux/random.h>     // Para generar temperaturas aleatorias
#include <linux/timer.h>      // Para temporizadores tipo timer_list
#include <linux/jiffies.h>    // Para trabajar con el tiempo del kernel

// ========================== FUNCIONES GPIO SIMULADAS =========================

// Declaraciones de funciones exportadas por gpio_sim.ko
extern int gpio_sim_request(unsigned gpio, const char *label);           // Solicita un GPIO
extern void gpio_sim_free(unsigned gpio);                                 // Libera un GPIO
extern int gpio_sim_direction_output(unsigned gpio, int value);           // Configura dirección
extern void gpio_sim_set_value(unsigned gpio, int value);                 // Establece valor

// =========================== VARIABLES INTERNAS ==============================

static int current_temperature = 25;          // Temperatura inicial (°C)
static struct timer_list temp_timer;          // Temporizador para simular lectura periódica

#define SENSOR_GPIO_DATA 10                   // GPIO virtual que representa el pin de salida del sensor

// ======================= FUNCIÓN DE SIMULACIÓN DE SENSOR =====================

/**
 * update_temperature - Función invocada cada 5 segundos para simular una nueva lectura
 * @t: puntero al temporizador (no usado directamente)
 */
static void update_temperature(struct timer_list *t) {
    u8 rand;

    // Generar un valor aleatorio entre 20 y 40 grados
    get_random_bytes(&rand, sizeof(rand));
    current_temperature = 20 + (rand % 21);  // [20, 40]

    // Mostrar la nueva temperatura en los logs
    printk(KERN_INFO "sensor_sim: Temperatura generada = %d°C\n", current_temperature);

    // Simular el valor del pin DATA del DHT22: 1 si temp > 25, 0 si <= 25
    gpio_sim_set_value(SENSOR_GPIO_DATA, current_temperature > 25 ? 1 : 0);

    // Reprogramar el temporizador para 5 segundos después
    mod_timer(&temp_timer, jiffies + msecs_to_jiffies(5000));
}

// ========================== FUNCIÓN EXPORTADA ================================

/**
 * read_simulated_temperature - Retorna la última temperatura generada
 * Retorna: temperatura actual (int)
 */
int read_simulated_temperature(void) {
    return current_temperature;
}
EXPORT_SYMBOL(read_simulated_temperature);

// =========================== INICIALIZACIÓN DEL MÓDULO =======================

/**
 * sensor_sim_init - Inicializa el módulo y configura el temporizador
 */
static int __init sensor_sim_init(void) {
    int ret;

    // Solicita un GPIO virtual para simular el pin DATA del sensor
    ret = gpio_sim_request(SENSOR_GPIO_DATA, "sensor_data");
    if (ret) {
        printk(KERN_ERR "sensor_sim: Error al solicitar GPIO %d\n", SENSOR_GPIO_DATA);
        return ret;
    }

    // Configura como salida inicialmente en 0
    gpio_sim_direction_output(SENSOR_GPIO_DATA, 0);

    // Configura e inicia el temporizador con una primera ejecución en 2 segundos
    timer_setup(&temp_timer, update_temperature, 0);
    mod_timer(&temp_timer, jiffies + msecs_to_jiffies(2000));

    printk(KERN_INFO "sensor_sim: Módulo de sensor DHT22 simulado cargado\n");
    return 0;
}

// ============================ FINALIZACIÓN DEL MÓDULO ========================

/**
 * sensor_sim_exit - Limpieza al remover el módulo
 */
static void __exit sensor_sim_exit(void) {
    del_timer_sync(&temp_timer);                // Detiene el temporizador
    gpio_sim_free(SENSOR_GPIO_DATA);            // Libera el GPIO
    printk(KERN_INFO "sensor_sim: Módulo descargado\n");
}

// =========================== METADATOS DEL MÓDULO ============================

module_init(sensor_sim_init);
module_exit(sensor_sim_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edison Zapata");
MODULE_DESCRIPTION("Sensor DHT22 simulado con GPIOs virtuales");
