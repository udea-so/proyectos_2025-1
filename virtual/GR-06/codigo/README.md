# Código

## Árbol de carpetas

├── cholesky
│   ├── cholesky.c
│   ├── cholesky_paralelo.c
│   ├── cholesky_paralelo.py
│   └── cholesky.py
├── gaussiana
│   ├── gaussiana.c
│   ├── gaussiana.py
│   ├── parallel_gaussiana.c
│   └── parallel_gaussiana.py
├── inversion_manual_impl.ipynb
├── lu
│   ├── lu_matrix_inversion.c
│   ├── lu_matrix_inversion.ipynb
│   ├── parallel_lu_matrix_inversion.c
│   └── parallel_lu_matrix_inversion.ipynb
├── matrix_inversión_cuda.ipynb
└── README.md


Como se puede observar en el árbol de carpetas del repositorio el código principal está dividido en las carpetas de los algoritmos realizados. Por ejemplo, en la carpeta `lu` se encuentran alojados 4 archivos. La inversión de matrices de manera secuencial con el lenguaje C, la inversión de matrices de manera secuencial con el lenguaje Python, y otros 2 archivos con la implementación paralelizada del algoritmo. 
Aparte de esto, se encuentra también un notebook de Google Colab llamado `matrix_inversión_cuda.ipynb` en el cual se encuentra la implementación de los 3 algoritmos usando CUDA. 
