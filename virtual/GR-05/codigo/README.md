# Módulo Kernel: Control de Matriz de LEDs con Sensor de Temperatura (Simulado con GPIO)

---
---
### Video del proyecto en ejecución: [https://youtu.be/Y1quVVN6jZM](https://youtu.be/Y1quVVN6jZM)
---
---

## 📊 Resumen del Proyecto

Este proyecto implementa un sistema de control embebido completamente en **espacio de kernel**, donde un **sensor de temperatura simulado (DHT22)** es leído mediante pines GPIO virtuales y una **matriz de LEDs 4x4** es controlada como salida también a través de GPIOs. Todo el comportamiento se gestiona mediante **módulos del kernel de Linux**, emulando un entorno de hardware sin necesidad de una Raspberry Pi física.

### 🌟 Enfoque

- 100% ejecutado en **espacio kernel**.
- Control de **periféricos virtuales** mediante simulación de GPIOs.
- Modularidad: cada componente (sensor, matriz, GPIO, controlador) es independiente.
- Comportamiento **reactivo por rango**: la matriz solo se actualiza cuando la temperatura cambia de categoría (frío, templado, caliente).

---

## 📁 Estructura del Proyecto

```
.
├── gpio_sim.c            # Simula los pines GPIO
├── sensor_sim.c          # Genera temperaturas aleatorias (como un DHT22)
├── led_matrix.c          # Controla la matriz 4x4 de LEDs usando GPIOs
├── controlador.c         # Lee la temperatura y actualiza la matriz por rangos
├── Makefile              # Compila, limpia y carga los módulos
├── test.sh               # Script para observar el comportamiento

```

---

## 🌐 En qué consiste el proyecto

Este sistema kernel simula el comportamiento de un sensor de temperatura y su interacción con una salida visual (matriz de LEDs), utilizando pines GPIO virtuales para simular tanto la entrada (sensor) como la salida (LEDs). El objetivo es demostrar el diseño de drivers de kernel que interactúan entre módulos usando una arquitectura limpia, escalable y realista.

---

## 📃 Módulos desarrollados

### 1. `gpio_sim.ko`

- **Propósito**: Simula la API de GPIO del kernel (`gpio_request`, `gpio_set_value`, etc).
- **Funciones exportadas**:
  - `gpio_sim_request`, `gpio_sim_free`, `gpio_sim_direction_input/output`, `gpio_sim_get/set_value`
- **Estructuras**: arreglo estático de estados de 32 GPIOs simulados.

### 2. `sensor_sim.ko`

- **Propósito**: Emula un sensor DHT22 generando una nueva temperatura cada 5 segundos.
- **Salida**: GPIO[10] se pone en 1 si la temperatura supera los 25ºC, 0 si es menor.
- **Funciones exportadas**: `read_simulated_temperature()`
- **Estructura**: `workqueue` con `delayed_work` cíclico.

### 3. `led_matrix.ko`

- **Propósito**: Controla una matriz 4x4 de LEDs.
- **Entrada/salida**: GPIOs 0-3 (filas), GPIOs 4-7 (columnas).
- **Funciones exportadas**:
  - `led_on`, `clear_matrix`, `print_matrix_state`, `print_gpio_matrix_state`
- **Interno**: Matriz `leds[4][4]` y funciones que usan GPIOs para representar encendido/apagado.

### 4. `controlador.ko`

- **Propósito**: Hilo del kernel que lee temperatura y actualiza la matriz de LEDs según rangos.
- **Enlace**: Usa funciones exportadas de los otros tres módulos.
- **Rangos**:
  - Frío (<25ºC): 4 LEDs encendidos.
  - Templado (25-29ºC): 8 LEDs.
  - Caliente (>=30ºC): 12 LEDs.
- **Condición**: Solo se actualiza si cambia de rango anterior.

---

## 🔀 Uso de GPIOs simulados

| GPIO | Módulo         | Dirección | Descripción                        |
| ---- | -------------- | --------- | ---------------------------------- |
| 0-3  | led\_matrix.ko | salida    | Controlan filas de la matriz       |
| 4-7  | led\_matrix.ko | salida    | Controlan columnas de la matriz    |
| 10   | sensor\_sim.ko | salida    | Representa "estado" de temperatura |

**Nota**: Todos los GPIOs son gestionados por `gpio_sim.ko`.

---

## 🚜 Requisitos del sistema

- Ubuntu 20.04+ (probado en 24.04)
- Headers del kernel instalados: `linux-headers-$(uname -r)`
- Compilador GCC compatible (ej: gcc-13)
- Acceso root (para cargar módulos y ver `dmesg`)

---

## 📊 Dependencias y librerías

- `linux/module.h`, `linux/kernel.h`, `linux/kthread.h`, `linux/gpio.h`, etc.
- `Makefile` usa el sistema de construcción del kernel con:
  ```make
  make -C /lib/modules/$(uname -r)/build M=$(PWD) modules
  ```

---

## ⚖️ Instalación y ejecución

```bash
# 1. Compilar y Cargar módulos
make


# 3. Ver salidas
sudo dmesg -w  # Observa el comportamiento en tiempo real

# 4. Detener y Descargar los módulos
make clean
```

---

## 📈 Observación en tiempo real

Puedes ejecutar:

```bash
watch -n 1 "dmesg | tail -n 30"
```

para ver cómo cambian las temperaturas y la matriz según el rango.

---

## ❓ Otros detalles importantes

- Todos los módulos están documentados con comentarios en el código fuente.
- El proyecto es extensible: podrías reemplazar `gpio_sim` por GPIOs reales en una Raspberry Pi si se desea.
- Se demuestra el uso de `workqueues`, `kthreads`, exportación de símbolos y simulación hardware.

---

## 🌟 Autores y licencia

- **Autor**: Edison Zapata
- **Asistencia técnica y guía**: ChatGPT + Linux Kernel Docs
- **Licencia**: GPLv2

---

