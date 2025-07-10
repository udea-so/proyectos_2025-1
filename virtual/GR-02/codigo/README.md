# 💾 Desarrollo de un Monitor de Sistema en Tiempo Real con Notificaciones de Umbrales de Recursos

## 👥 Autores
|Integrante|correo|usuario github|
|---|---|---|
|Paula Andrea Guarin Guarin |paula.guaring@udea.edu.co |panguaring|
|Claudia Lucía Serna Gómez|claudial.serna@udea.edu.co|claudialserna|
|Duvan Ferney Ruiz Ocampo|duvan.ruiz1@udea.edu.co|DuvanR0598|

## 🎯 Objetivo
Desarrollar un monitor de sistema que supervise en tiempo real el consumo de recursos (CPU y memoria) de los procesos activos, notificando al administrador cuando se superen umbrales predefinidos, para facilitar la gestión eficiente del sistema.

## 💻 Requisitos Minimos del Sistema 
| Componente         | Requisito Mínimo                                                                 |
|--------------------|----------------------------------------------------------------------------------|
| Sistema Operativo  | Linux (Ubuntu/Debian, WSL2 en Windows, o cualquier distro con soporte para /proc) |
| Kernel             | Versión moderna (ej: 4.4+ para WSL2, 5.x para distribuciones recientes)           |
| CPU                | 1 núcleo (x86_64 o ARM64)                                                        |
| RAM                | 512 MB (suficiente para el monitor y procesos básicos)                           |
| Almacenamiento     | 100 MB libres (para instalación de dependencias y logs)                          |
| Terminal           | Terminal estándar (ej: Bash, Zsh) con soporte para ncurses                      |

## 📌 Estructura del Proyecto
```
sistema_monitor/  
├── src/  
│   ├── main.c            # Punto de entrada  
│   └── process_monitor.c # Lógica del monitor
|   └── alerts.c          # Lógica para alertas
├── include/  
│   └── process_monitor.h # Cabeceras
|   └── alerts.h  
├── build/                # Archivos compilados  
└── Makefile              # Automatización
└── config.ini
└── run_tests.sh
```

## 🔍 Dependencias Críticas
El proyecto requiere las siguientes librerías y herramientas (instalables en Linux/WSL):

#### 1. Compilador y herramientas básicas:
```bash
sudo apt update && sudo apt install -y build-essential gcc make
```
#### 2. Librería ncurses (para la interfaz gráfica):
```bash
sudo apt install -y libncurses-dev
```
#### 3. Valgrind (para depuración de memory leaks):
```bash
sudo apt install -y valgrind
```
#### 4. stress-ng (para pruebas de carga):
```bash
sudo apt install -y stress-ng
```
#### 5. Ejecucion:
```bash
make
./build/monitor
```
## ⚠️ Consideraciones Especiales
- **Entorno WSL (Windows Subsystem for Linux):**:
  - Requiere WSL2 para acceso completo a `/proc` y rendimiento óptimo.
  - Configuración recomendada en VS Code: Extensión Remote - WSL.
- **Sistemas embebidos o antiguos:**:
  - Si el kernel no expone `/proc/stat` o `/proc/meminfo`, el proyecto no funcionará.
- **Permisos:**:
  - El programa necesita permisos de lectura en `/proc` (ejecutarlo como usuario normal suele ser suficiente).   

# 🌐 Script de Pruebas Automatizadas para el Monitor de Sistema
En la estructura del codigo se encuentra un script Bash `run_test.sh` que permite validar el funcionamiento del monitor bajo diferentes escenarios de carga (CPU, memoria, procesos) y verificar:
- Correcta deteccion de umbrales.
- Generación de alertas
- Estabilidad del sistema
- Consumo de recursos

### Instrucciones de Uso
#### 1. Dar permisos de ejecución:
```bash
chmod +x run_tests.sh
```
#### 2. Ejecutar (requiere root para syslog):
```bash
sudo ./run_tests.sh
```

#### 3. Salida Esperada
```bash
[2025-07-09 10:00:00] PRUEBA COMPLETADA: SOBRECARGA CPU
[2025-07-09 10:01:00] ALERTAS GENERADAS:
[2025-07-09 10:00:45] ALERTA: CPU excedido (95.5% > 1.0%) en stress-ng (PID: 12345)
[2025-07-09 10:00:47] VALGRIND: Sin fugas de memoria
```

Link para ver video de ejecución: https://drive.google.com/drive/folders/1W4xpABlzpevXa1pERU968nx6R1xZwtY6?usp=sharing
