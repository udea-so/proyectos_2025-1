# Código

## Árbol de carpetas

```text
├── cholesky
│   ├── cholesky.c
│   ├── cholesky_paralelo.c
│   ├── cholesky_paralelo.py
│   └── cholesky.py
├── gaussiana
│   ├── gaussiana.c
│   ├── gaussiana.py
│   ├── parallel_gaussiana.c
│   └── parallel_gaussiana.py
├── inversion_manual_impl.ipynb
├── lu
│   ├── lu_matrix_inversion.c
│   ├── lu_matrix_inversion.ipynb
│   ├── parallel_lu_matrix_inversion.c
│   └── parallel_lu_matrix_inversion.ipynb
├── matrix_inversión_cuda.ipynb
└── README.md
```


Como se puede observar en el árbol de carpetas del repositorio el código principal está dividido en las carpetas de los algoritmos realizados. Por ejemplo, en la carpeta `lu` se encuentran alojados 4 archivos. La inversión de matrices de manera secuencial con el lenguaje C, la inversión de matrices de manera secuencial con el lenguaje Python, y otros 2 archivos con la implementación paralelizada del algoritmo. 
Aparte de esto, se encuentra también un notebook de Google Colab llamado `matrix_inversión_cuda.ipynb` en el cual se encuentra la implementación de los 3 algoritmos usando CUDA. 

## Compilación y ejecución

### Restricciones para la generación de matrices:
- Para los algoritmos `lu` y `gaussiana` las matrices que se deben usar deben cumplir con ser cuadradas e invertibles.
- Para el algoritmo `cholesky` se deben tener en cuenta las recomendaciones anteriores y adicionalmente, deben ser definidas positivas. (estas matrices también están cargadas en el repositorio en la carpeta proyectos_2025-1/virtual/GR-06/matrices/def_pos_inv)

Las matrices a usar se encuentran todas en este repositorio también en la carpeta proyectos_2025-1/virtual/GR-06/matrices

Se pueden cargar estas matrices en formato `.csv` o generar nuevas matrices de diferentes tamaños para probar la eficiencia de los algoritmos. 

### Python
Para los algoritmos de Python (todos implementados en un archivo .py) se debe tener Python3 instalado y si no se tiene instalado numpy es recomendable crear un entorno virtual en la carpeta code con los siguientes comandos:

```bash
python3 -m venv venv
source venv/bin/activate
pip install numpy
```

Luego, para ejecutar por ejemplo el algoritmo lu paralelo en python se navega hasta la carpeta `lu` y se ejecuta el siguiente comando:
```bash
python3 parallel_lu_matrix_inversion.py 100
```

Cambiando 100 por la dimensión de la matriz que se quiere invertir

> [!TIP]
> Para todos los algoritmos implementados en Python se hace el mismo proceso. Se navega hasta la carpeta del algoritmo y se ejecuta el archivo `.py` pasándole las dimensiones de la matriz.

### C
Para los algoritmos en C, necesitamos compilar cada uno de diferente manera dependiendo las necesidades de cada archivo.

- [lu_matrix_inversion.c](lu/lu_matrix_inversion.c): ``
