# -*- coding: utf-8 -*-
import numpy as np
import threading
import time
import sys
import os

def cargar_matriz_csv(ruta_csv):
    A = np.loadtxt(ruta_csv, delimiter=',')
    N = A.shape[0]
    print("Matriz A cargada desde CSV:")
    print(A)
    return A, N

def inicializar_matriz_inversa(N):
    matriz_inv = np.zeros((N, N))
    print("\nMatriz inversa inicializada en ceros:")
    print(matriz_inv)
    return matriz_inv

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

def forward_substitution(L, e):
    N = L.shape[0]
    y = np.zeros(N)
    for j in range(N):
        suma = 0.0
        for k in range(j):
            suma += L[j, k] * y[k]
        y[j] = (e[j] - suma) / L[j, j]
    return y

def backward_substitution(L, y):
    N = L.shape[0]
    x = np.zeros(N)
    for j in reversed(range(N)):
        suma = 0.0
        for k in range(j+1, N):
            suma += L[k, j] * x[k]
        x[j] = (y[j] - suma) / L[j, j]
    return x

def calcular_columna_inversa(i, L, matriz_inv):
    """Calcula la columna i-ésima de la matriz inversa y la guarda en matriz_inv."""
    N = L.shape[0]
    e = np.zeros(N)
    e[i] = 1
    y = forward_substitution(L, e)
    x = backward_substitution(L, y)
    matriz_inv[:, i] = x

def calcular_inversa_threading(L):
    N = L.shape[0]
    matriz_inv = np.zeros((N, N))
    threads = []

    for i in range(N):
        t = threading.Thread(target=calcular_columna_inversa, args=(i, L, matriz_inv))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    print("\nMatriz inversa calculada con Cholesky (threading):")
    print(matriz_inv)
    return matriz_inv

def main():
    if len(sys.argv) != 2:
        print("Uso: python3 cholesky_paralelo.py <dimension>")
        sys.exit(1)

    try:
        N = int(sys.argv[1])
        if N <= 0:
            raise ValueError
    except ValueError:
        print("La dimensión debe ser un número entero positivo.")
        sys.exit(1)

    ruta_csv = f'../../matrices/def_pos_inv/matriz_{N}x{N}_dpi.csv'

    if not os.path.isfile(ruta_csv):
        print(f"Archivo no encontrado: {ruta_csv}")
        sys.exit(1)

    # 1. Cargar matriz
    A, N = cargar_matriz_csv(ruta_csv)

    # 2. Inicializar matriz inversa (solo para mostrar)
    _ = inicializar_matriz_inversa(N)

    # 3. Calcular descomposición de Cholesky
    L = descomposicion_cholesky(A)

    # 4. Calcular inversa
    matriz_inv = calcular_inversa_threading(L)

if __name__ == "__main__":
    inicio = time.time()    # ⏱️ Marca de tiempo inicial
    main()
    fin = time.time()       # ⏱️ Marca de tiempo final
    print(f"\nTiempo de ejecución total: {fin - inicio:.4f} segundos")
