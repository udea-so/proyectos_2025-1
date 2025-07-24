# Inversión de matrices de gran tamaño


## Lista de chequeo

### Documentos

Dentro del directorio [documentos](documentos/) agregar los pdf de:
- [x] Propuesta.
- [x] Diapositivas de la presentacion de la propuesta
- [x] Reporte tecnico.
- [x] Dispositivas de la presentacion final.


### Archivos y elementos del repositorio

- [x] El repositorio del código implementado con su documentación. 
- [x] Código que incluya todos los recursos relevantes para ejecutar la aplicación desarrollada para resolver el desafío. 
- [x] Explicación de los requisitos del sistema
- [x] Librerias y dependencias necesarias (pasos necesarios para llevar a cabo la instalación)
- [x] Pasos necesarios para ejecutar la aplicación.
- [x] Ademas del código, es deseable que tenga un Notebook de Jupyter como complemento para la parte estadistica.


Este proyecto se basa en la aplicación de diferentes algoritmos y tecnologías centrados en la inversión de matrices de gran tamaño con el objetivo de medir tiempos, recolectarlos y realizar un análisis que permita identificar qué algoritmo, lenguaje o tecnología permite la inversión más eficiente temporalmente. 

El proyecto usa exclusivamente 2 lenguajes de programación (bajo y alto nivel):
1. C
2. Python

También, se exploran 3 algoritmos de inversión de matrices secuenciales a los cuales se les aplica paralelismo en las zonas del código que se permita dado cada algoritmo. Esto para realizar la comparación de los algoritmos paralelos con los algoritmos secuenciales. 
1. Inversión por eliminación Gaussiana.
2. Inversión por descomposición LU.
3. Inversión por descomposición de Cholesky. 

## ¿Cómo usar el repositorio?
1. Clonar el repositorio con `git clone https://github.com/brandugar/proyectos_2025-1.git`
2. Navegar hasta la carpeta `GR-06`
3. Dependiendo del algoritmo a usar, leer el [archivo README](codigo/README.md) de la carpeta código. 
4. En la carpeta [resultados](codigo/resultados) se pueden observar 3 notebooks de Google Colab con la recolección de tiempos que se realizó tomando en cuenta cada uno de los 12 algoritmos implementados. Aquí ya hay información acerca de cuales algoritmos son los que mostraron mejor rendimiento.

## Resultados encontrados
### Apreciaciones generales
- En la mayoría de algoritmos en Python se presenta un comportamiento interesante, y es que cuando se invierte una matriz con cualquiera de los algoritmos secuenciales (en especial matrices pequeñas) suele ser más eficiente que su par con paralelismo. Esto nos condujo a una duda, dado que al paralelizar ciertas partes del algoritmo, este debería ocupar menos tiempo en hacer la inversión de dichas matrices. Al investigar las causas de este comportamiento nos dimos cuenta de la existencia del GIL. El GIL pudimos observar que es un mecanismo utilizado en la implementación estándar de Python que impide que múltiples hilos de ejecución, ejecuten código Python simultáneamente en múltiples núcleos del procesador con el objetivo de evitar condiciones de carrera. Sin embargo, al usar multiprocessing para evitar el GIL, pudimos observar que el aumento de la velocidad de inversión de las matrices no es significativo, e incluso en ocasiones, empeora en vez de mejorar.
- En algunos algoritmos en C, se presenta el mismo comportamiento anterior pero a menor escala, y es que al momento de invertir pequeñas matrices (10x10, 50x50) se puede observar que el algoritmo secuencial tiene un mejor comportamiento en términos de tiempo de inversión que el algoritmo paralelo. Este comportamiento, sin embargo, tiene una explicación diferente que los algoritmos en Python, y es que para pequeños tamaños de matrices, al demorarse milisegundos, tiene un mejor rendimiento que cuando se deben crear hilos. Ya que la creación de estos mismos hilos implica un tiempo de cómputo que el algoritmo secuencial aprovecha en invertir partes de la matriz. 

### Comparación de tiempos de inversión de matrices 500x500

| Algoritmo                | C Secuencial | C Paralelo | Python Secuencial | Python Paralelo | 
|--------------------------|--------------|------------|-------------------|------------------|
| Eliminación Gaussiana    |  `0.098 ± 0.023s`   | `0.290s ± 0.029s`  |   `0.833s ± 0.027s`       |   `7.005s ± 0.125s`      |
| Descomposición LU        |  `0.536s ± 0.006s`   | `0.195s ± 0.003s`  |   `33.91s ± 0.08s`       |   `36.21s ± 1.26s`      |
| Descomposición Cholesky  |  `62.062s ± 0.627`   | `0.107s ± 0.786s`  |   `62.062s ± 0.627s`       |   `62.141s ± 0.293s`      |

### Comparación de tiempos CUDA (Matrices n=500)

| Algoritmo | Cupy | Pytorch |
|-----------|------|---------|
|Eliminación Gaussiana |`12.840`| `10.613`|
| Descomposición LU| `97.844` | `52.987` |
|Descomposión Cholesky |`50.892` | ` 16.975 ` |

> **Nota:** Los tiempos son promedios obtenidos al invertir matrices de tamaño `500x500`. Las implementaciones paralelas en Python utilizan `multiprocessing`, pero no siempre mejoran el rendimiento por el efecto del **GIL** y el overhead de crear procesos.

### Video a la ejecución del proyecto
Enlace al video de Youtube:
[Anexo Video](https://youtu.be/GP2SXB5RNGQ)
