#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/random.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

// Usamos funciones GPIO simuladas
extern int gpio_sim_request(unsigned gpio, const char *label);
extern void gpio_sim_free(unsigned gpio);
extern int gpio_sim_direction_output(unsigned gpio, int value);
extern void gpio_sim_set_value(unsigned gpio, int value);

static int current_temperature = 25;
static struct timer_list temp_timer;

#define SENSOR_GPIO_DATA 10  // GPIO virtual para el sensor

// Temporizador: genera una nueva temperatura cada 5s
static void update_temperature(struct timer_list *t) {
    u8 rand;
    get_random_bytes(&rand, sizeof(rand));
    current_temperature = 20 + (rand % 21); // 20–40°C

    printk(KERN_INFO "sensor_sim: Temperatura generada = %d°C\n", current_temperature);

    gpio_sim_set_value(SENSOR_GPIO_DATA, current_temperature > 25 ? 1 : 0);
    mod_timer(&temp_timer, jiffies + msecs_to_jiffies(5000));
}

// Función exportada
int read_simulated_temperature(void) {
    return current_temperature;
}
EXPORT_SYMBOL(read_simulated_temperature);

static int __init sensor_sim_init(void) {
    int ret;

    ret = gpio_sim_request(SENSOR_GPIO_DATA, "sensor_data");
    if (ret) {
        printk(KERN_ERR "sensor_sim: Error al solicitar GPIO %d\n", SENSOR_GPIO_DATA);
        return ret;
    }

    gpio_sim_direction_output(SENSOR_GPIO_DATA, 0);

    timer_setup(&temp_timer, update_temperature, 0);
    mod_timer(&temp_timer, jiffies + msecs_to_jiffies(2000));

    printk(KERN_INFO "sensor_sim: Módulo cargado\n");
    return 0;
}

static void __exit sensor_sim_exit(void) {
    del_timer_sync(&temp_timer);
    gpio_sim_free(SENSOR_GPIO_DATA);
    printk(KERN_INFO "sensor_sim: Módulo descargado\n");
}

module_init(sensor_sim_init);
module_exit(sensor_sim_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edison Zapata");
MODULE_DESCRIPTION("Sensor DHT22 simulado con GPIOs virtuales");
