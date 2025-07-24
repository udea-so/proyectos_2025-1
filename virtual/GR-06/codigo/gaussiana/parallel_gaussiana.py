import numpy as np
import csv
from numba import njit, prange
import time
import sys


@njit(parallel=True)
def gauss_jordan_inverse_numba(A):
    n = len(A)
    A = A.astype(np.float64)

    I = np.identity(n, dtype=np.float64)
    AI = np.hstack((A, I))  # Matriz aumentada [A | I]

    for i in range(n):
        # Pivoteo parcial (reemplazo del swap)
        max_row = i + np.argmax(np.abs(AI[i:, i]))
        temp = AI[i].copy()
        AI[i] = AI[max_row]
        AI[max_row] = temp

        # Normalizar la fila pivote
        AI[i] = AI[i] / AI[i, i]

        # Eliminar los elementos arriba y abajo del pivote (paralelo)
        for j in prange(n):
            if i != j:
                AI[j] = AI[j] - AI[j, i] * AI[i]

    return AI[:, n:]  # Retornar la parte derecha: la inversa


def leer_matriz_desde_csv(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lector = csv.reader(archivo)
        matriz = [list(map(float, fila)) for fila in lector]
    return np.array(matriz)


# ------------------- Lógica principal -------------------

if len(sys.argv) != 2:
    print("Uso: python3 este_archivo.py <dimension>")
    sys.exit(1)

try:
    n = int(sys.argv[1])
    if n <= 0:
        raise ValueError
except ValueError:
    print("La dimensión debe ser un número entero positivo.")
    sys.exit(1)

nombre_archivo = f"../../matrices/matriz_{n}x{n}_invertible.csv"

A = leer_matriz_desde_csv(nombre_archivo)

inicio = time.time()
inv_A = gauss_jordan_inverse_numba(A)
fin = time.time()
print(f"Tiempo de ejecución: {fin - inicio:.6f} segundos")

# Si la matriz es pequeña, imprimimos la inversa
if n <= 10:
    print("Inversa por Gauss-Jordan:")
    print(np.round(inv_A, 6))
else:
    np.savetxt("inversa.csv", inv_A, delimiter=",", fmt="%.10f")
    print("Matriz inversa guardada en 'inversa.csv' (no se muestra por ser mayor de 10x10).")
