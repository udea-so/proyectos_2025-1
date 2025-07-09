# 💾 Desarrollo de un Monitor de Sistema en Tiempo Real con Notificaciones de Umbrales de Recursos

## 👥 Autores
- Duvan Ruiz
- Paula Guarin
- Claudia Serna

## 🎯 Objetivo
Desarrollar un monitor de sistema que supervise en tiempo real el consumo de recursos (CPU y memoria) de los procesos activos, notificando al administrador cuando se superen umbrales predefinidos, para facilitar la gestión eficiente del sistema.

## 📌 Requisitos Minimos del Sistema 
| Componente         | Requisito Mínimo                                                                 |
|--------------------|----------------------------------------------------------------------------------|
| Sistema Operativo  | Linux (Ubuntu/Debian, WSL2 en Windows, o cualquier distro con soporte para /proc) |
| Kernel             | Versión moderna (ej: 4.4+ para WSL2, 5.x para distribuciones recientes)           |
| CPU                | 1 núcleo (x86_64 o ARM64)                                                        |
| RAM                | 512 MB (suficiente para el monitor y procesos básicos)                           |
| Almacenamiento     | 100 MB libres (para instalación de dependencias y logs)                          |
| Terminal           | Terminal estándar (ej: Bash, Zsh) con soporte para ncurses                      |

## 🔍 Dependencias Críticas
El proyecto requiere las siguientes librerías y herramientas (instalables en Linux/WSL):

### 1. Compilador y herramientas básicas:
```bash
sudo apt update && sudo apt install -y build-essential gcc make
```
### 2. Librería ncurses (para la interfaz gráfica):
```bash
sudo apt install -y libncurses-dev
```
### 3. Valgrind (para depuración de memory leaks):
```bash
sudo apt install -y valgrind
```
### 4. stress-ng (para pruebas de carga):
```bash
sudo apt install -y stress-ng
```
## ⚠️ Consideraciones Especiales
- **Entorno WSL (Windows Subsystem for Linux):**:
  - Requiere WSL2 para acceso completo a `/proc` y rendimiento óptimo.
  - Configuración recomendada en VS Code: Extensión Remote - WSL.
- **Sistemas embebidos o antiguos:**:
  - Si el kernel no expone `/proc/stat` o `/proc/meminfo`, el proyecto no funcionará.
- **Permisos:**:
  - El programa necesita permisos de lectura en `/proc` (ejecutarlo como usuario normal suele ser suficiente).   
 
