# -*- coding: utf-8 -*-
import numpy as np
import time
import sys
import os

"""En la siguente funcion se hace la carga del archivo CSV que contiene matriz de diferentes tamaños **NxN**"""

def cargar_matriz_csv(ruta_csv):
    A = np.loadtxt(ruta_csv, delimiter=',')
    N = A.shape[0]
    print("Matriz A cargada desde CSV:")
    print(A)
    return A, N

"""En la siguiente funcion inicializo la matriz de tamaño **NxN** llena de ceros que nos servira para ir guardando los diferentes resultados calculados"""

def inicializar_matriz_inversa(N):
    matriz_inv = np.zeros((N, N))
    print("\nMatriz inversa inicializada en ceros:")
    print(matriz_inv)
    return matriz_inv

"""En esta trabajamos la descomposicion de cholesky
lo primero hacemos es saber el tamaño de la matriz

cosas necesarias para que funcione el algoritmo
*   Matriz cuadrada
*   Simetrica
*   Positiva

Teniendo eso encuenta pasamos con la descripcion de el metodo de cholesky
Permite factorizar una matriz **A** en el producto de una matriz triangular inferior **L** y su transpuesta

$$
A = LL^T
$$

*  A es una matriz simétrica y definida positiva de tamaño N×N.

*  L es una matriz triangular inferior N×N cuyos elementos se calculan de manera recursiva.

Formulas utilizadas para el calculo de la matriz **L**
1.  Si está en la diagonal **(i=j)**
$$
L_{i,i} = \sqrt{A_{i,i} - \sum_{k=0}^{i-1} L_{i,k}^2}
$$
2.  Si esta debajo de la diagonal **(i>j)**
$$
L_{i,j} = \frac{1}{L_{j,j}} \left( A_{i,j} - \sum_{k=0}^{j-1} L_{i,k} L_{j,k} \right)
$$
"""

def descomposicion_cholesky(A):
    N = A.shape[0]
    #Se inicializa la matriz L
    L = np.zeros_like(A)
    #Recorre filas i y columnas j
    for i in range(N):
        for j in range(i+1):
            suma = 0.0
            #Elementos de la diagonal con la sumatoria de su formula para el calculo de L
            if j == i:
                for k in range(j):
                    suma += L[j,k] ** 2
                #Utilizamos val para avanzar en la formula y el calculo de si es positiva la matriz si no arrojamos error
                val = A[j,j] - suma
                if val <= 0:
                    raise ValueError(f"La matriz no es definida positiva en la posición ({j},{j})")
                #Se saca la raiz para completar la formula de la diagonal
                L[j,j] = np.sqrt(val)
            else:
                #Formula para los elementos fuera de la diagonal
                for k in range(j):
                    suma += L[i,k] * L[j,k]
                L[i,j] = (A[i,j] - suma) / L[j,j]
    print("\nMatriz L (Cholesky) calculada:")
    print(L)
    return L

"""**Funcion Sustitución de Forward**"""

def forward_substitution(L, e):
    N = L.shape[0]
    #Inici el vector para guardar los resultados
    y = np.zeros(N)
    #Se aplica la formula
    for j in range(N):
        suma = 0.0
        for k in range(j):
            suma += L[j, k] * y[k]
        y[j] = (e[j] - suma) / L[j, j]
    return y

"""**Funcion Sustitución de Backward**"""

def backward_substitution(L, y):
    N = L.shape[0]
    #Inicializamos el vector para guardar los resultados
    x = np.zeros(N)
    #Aplicamos la formula
    for j in reversed(range(N)):
        suma = 0.0
        for k in range(j+1, N):
            suma += L[k, j] * x[k]
        x[j] = (y[j] - suma) / L[j, j]
    return x

"""**Funcion para calcular la inversa**"""

def calcular_inversa(L):
    N = L.shape[0]
    # Inicializamos la matriz para guardar los resultados
    matriz_inv = np.zeros((N, N))
    for i in range(N):
        e = np.zeros(N)
        e[i] = 1
        # Aplicamos forward y backward substitution para obtener el vector x
        # y lo asignamos a la matriz inversa en la columna i-ésima
        y = forward_substitution(L, e)
        x = backward_substitution(L, y)
        matriz_inv[:, i] = x
    print("\nMatriz inversa calculada con Cholesky:")
    print(matriz_inv)
    return matriz_inv

def main():
    if len(sys.argv) != 2:
        print("Uso: python3 cholesky_secuencial.py <dimension>")
        sys.exit(1)

    try:
        N = int(sys.argv[1])
        if N <= 0:
            raise ValueError
    except ValueError:
        print("La dimensión debe ser un número entero positivo.")
        sys.exit(1)

    # Ruta con nombre dinámico según dimensión
    ruta_csv = f'../../matrices/def_pos_inv/matriz_{N}x{N}_dpi.csv'

    if not os.path.isfile(ruta_csv):
        print(f"Archivo no encontrado: {ruta_csv}")
        sys.exit(1)

    # 1. Cargar matriz
    A, N = cargar_matriz_csv(ruta_csv)

    # 2. Inicializar matriz inversa
    _ = inicializar_matriz_inversa(N)

    # 3. Calcular descomposición de Cholesky
    L = descomposicion_cholesky(A)

    # 4. Calcular inversa
    matriz_inv = calcular_inversa(L)

if __name__ == "__main__":
    inicio = time.time()    # ⏱️ Marca de tiempo inicial
    main()
    fin = time.time()       # ⏱️ Marca de tiempo final
    print(f"\nTiempo de ejecución total: {fin - inicio:.4f} segundos")
