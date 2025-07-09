obj-m += gpio_sim.o
obj-m += sensor_sim.o
obj-m += led_matrix.o
obj-m += controlador.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	@echo "[🧹] Limpiando módulos y binarios..."
	sudo rmmod controlador 2>/dev/null || true
	sudo rmmod led_matrix 2>/dev/null || true
	sudo rmmod sensor_sim 2>/dev/null || true
	sudo rmmod gpio_sim 2>/dev/null || true
	make -C $(KDIR) M=$(PWD) clean

	@echo "[🛠] Compilando módulos..."
	make -C $(KDIR) M=$(PWD) modules

	@echo "[🔁] Cargando módulos..."
	sudo insmod gpio_sim.ko
	sudo insmod sensor_sim.ko
	sudo insmod led_matrix.ko
	sudo insmod controlador.ko

	@echo "[✅] Sistema listo. Usa: sudo dmesg -w"

clean:
	@echo "[🧼] Limpiando..."
	sudo rmmod controlador 2>/dev/null || true
	sudo rmmod led_matrix 2>/dev/null || true
	sudo rmmod sensor_sim 2>/dev/null || true
	sudo rmmod gpio_sim 2>/dev/null || true
	make -C $(KDIR) M=$(PWD) clean
