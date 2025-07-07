# -*- coding: utf-8 -*-
import numpy as np
import time
import sys

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
    L = np.zeros_like(A)
    for i in range(N):
        for j in range(i+1):
            suma = 0.0
            if j == i:
                for k in range(j):
                    suma += L[j,k] ** 2
                val = A[j,j] - suma
                if val <= 0:
                    raise ValueError(f"La matriz no es definida positiva en la posición ({j},{j})")
                L[j,j] = np.sqrt(val)
            else:
                for k in range(j):
                    suma += L[i,k] * L[j,k]
                L[i,j] = (A[i,j] - suma) / L[j,j]
    print("\nMatriz L (Cholesky) calculada:")
    print(L)
    return L

"""**Funcion Sustitución de Forward**
Resuelve Ly = e para una matriz triangular inferior
"""

def forward_substitution(L, e):
    N = L.shape[0]
    y = np.zeros(N)
    for j in range(N):
        suma = 0.0
        for k in range(j):
            suma += L[j, k] * y[k]
        y[j] = (e[j] - suma) / L[j, j]
    return y

"""**Funcion Sustitución de Backward**
Resuelve Lᵗx = y para una matriz triangular superior
"""

def backward_substitution(L, y):
    N = L.shape[0]
    x = np.zeros(N)
    for j in reversed(range(N)):
        suma = 0.0
        for k in range(j+1, N):
            suma += L[k, j] * x[k]
        x[j] = (y[j] - suma) / L[j, j]
    return x

"""**Funcion para calcular la inversa**
Usa la descomposición de Cholesky para obtener A⁻¹
"""

def calcular_inversa(L):
    N = L.shape[0]
    matriz_inv = np.zeros((N, N))
    for i in range(N):
        e = np.zeros(N)
        e[i] = 1
        y = forward_substitution(L, e)
        x = backward_substitution(L, y)
        matriz_inv[:, i] = x
    print("\nMatriz inversa calculada con Cholesky:")
    print(matriz_inv)
    return matriz_inv

def main():
    if len(sys.argv) != 2:
        print("Uso: python3 archivo_cholesky.py <dimension>")
        sys.exit(1)

    try:
        N = int(sys.argv[1])
        if N <= 0:
            raise ValueError
    except ValueError:
        print("La dimensión debe ser un número entero positivo.")
        sys.exit(1)

    ruta_csv = f"../../matrices/matriz_{N}x{N}_dpi.csv"

    A, N = cargar_matriz_csv(ruta_csv)
    _ = inicializar_matriz_inversa(N)
    L = descomposicion_cholesky(A)
    _ = calcular_inversa(L)

if __name__ == "__main__":
    inicio = time.time()
    main()
    fin = time.time()
    print(f"\nTiempo de ejecución total: {fin - inicio:.4f} segundos")
