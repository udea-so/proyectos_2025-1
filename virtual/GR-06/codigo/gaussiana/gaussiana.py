import numpy as np
import time
import csv
import sys
import os


def gauss_jordan_inverse(A):
    n = len(A)
    A = A.astype(float)

    I = np.identity(n)
    AI = np.hstack((A, I))  # Matriz aumentada [A | I]

    for i in range(n):
        # Pivoteo parcial
        max_row = np.argmax(np.abs(AI[i:, i])) + i
        AI[[i, max_row]] = AI[[max_row, i]]

        # Normalizar la fila pivote
        AI[i] = AI[i] / AI[i, i]

        # Eliminar los elementos arriba y abajo del pivote
        for j in range(n):
            if i != j:
                AI[j] = AI[j] - AI[j, i] * AI[i]

    return AI[:, n:]  # Retornar la parte derecha: la inversa


def leer_matriz_desde_csv(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lector = csv.reader(archivo)
        matriz = [list(map(float, fila)) for fila in lector]
    return np.array(matriz)


# --- Uso desde línea de comandos ---
if len(sys.argv) != 2:
    print("Uso: python gauss_jordan_inverse.py <dimension>")
    sys.exit(1)

try:
    dimension = int(sys.argv[1])
    if dimension <= 0:
        raise ValueError
except ValueError:
    print("La dimensión debe ser un número entero positivo.")
    sys.exit(1)

# Construcción dinámica del nombre del archivo
nombre_archivo = os.path.join("..", "..", "matrices", f"matriz_{dimension}x{dimension}_invertible.csv")

# Carga y validación de la matriz
try:
    A = leer_matriz_desde_csv(nombre_archivo)
except Exception as e:
    print(f"Error leyendo la matriz desde {nombre_archivo}: {e}")
    sys.exit(1)

# Inversión y medición de tiempo
inicio = time.time()
inv_A = gauss_jordan_inverse(A)
fin = time.time()

# Mostrar o guardar resultado
if dimension <= 10:
    print("Inversa por Gauss-Jordan:")
    print(np.round(inv_A, 6))
else:
    np.savetxt("inversa.csv", inv_A, delimiter=",", fmt="%.10f")
    print("Inversa guardada en 'inversa.csv' (no se muestra por ser mayor de 10x10).")

print(f"Tiempo de ejecución: {fin - inicio:.6f} segundos")
