# Inversión de matrices de gran tamaño


## Lista de chequeo

### Documentos

Dentro del directorio [documentos](documentos/) agregar los pdf de:
- [x] Propuesta.
- [x] Diapositivas de la presentacion de la propuesta
- [ ] Reporte tecnico.
- [ ] Dispositivas de la presentacion final.


### Archivos y elementos del respositorio

- [ ] El repositorio del código implementado con su documentación. 
- [ ] Código que incluya todos los recursos relevantes para ejecutar la aplicación desarrollada para resolver el desafío. 
- [ ] Explicación de los requisitos del sistema
- [ ] Librerias y dependencias necesarias (pasos necesarios para llevar a cabo la instalación)
- [ ] Pasos necesarioas para ejecutar la aplicación.
- [ ] Ademas del código, es deseable que tenga un Notebook de Jupyter como complemento para la parte estadistica.


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
