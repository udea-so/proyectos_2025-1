import numpy as np
import threading
import time
import sys

def leer_csv(nombre_archivo):
    try:
        A = np.loadtxt(nombre_archivo, delimiter=",")
        if A.shape[0] != A.shape[1]:
            raise ValueError("La matriz no es cuadrada")
        return A
    except Exception as e:
        print(f"Error al leer el archivo CSV: {e}")
        return None

def lu_decomposition(A):
    N = A.shape[0]
    L = np.zeros((N, N))
    U = np.zeros((N, N))

    for i in range(N):
        for k in range(i, N):
            suma = sum(L[i][j] * U[j][k] for j in range(i))
            U[i][k] = A[i][k] - suma

        for k in range(i, N):
            if i == k:
                L[i][i] = 1.0
            else:
                suma = sum(L[k][j] * U[j][i] for j in range(i))
                L[k][i] = (A[k][i] - suma) / U[i][i]

    return L, U

def forward_substitution(L, b):
    N = L.shape[0]
    y = np.zeros(N)
    for i in range(N):
        y[i] = b[i] - np.dot(L[i, :i], y[:i])
    return y

def backward_substitution(U, y):
    N = U.shape[0]
    x = np.zeros(N)
    for i in reversed(range(N)):
        x[i] = (y[i] - np.dot(U[i, i+1:], x[i+1:])) / U[i][i]
    return x

def invertir_columna(i, L, U, A_inv, N):
    e = np.zeros(N)
    e[i] = 1.0
    y = forward_substitution(L, e)
    x = backward_substitution(U, y)
    A_inv[:, i] = x

def invertir_matriz(A):
    N = A.shape[0]
    L, U = lu_decomposition(A)
    A_inv = np.zeros_like(A)

    hilos = []
    for i in range(N):
        hilo = threading.Thread(target=invertir_columna, args=(i, L, U, A_inv, N))
        hilos.append(hilo)
        hilo.start()

    for hilo in hilos:
        hilo.join()

    return A_inv

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python3 parallel_lu_matrix_inversion.py <dimensión>")
        sys.exit(1)

    try:
        N = int(sys.argv[1])
        if N <= 0:
            raise ValueError()
    except ValueError:
        print("Error: La dimensión debe ser un entero positivo")
        sys.exit(1)

    archivo = f"../../matrices/matriz_{N}x{N}_invertible.csv"
    A = leer_csv(archivo)

    if A is not None:
        start = time.time()
        A_inv = invertir_matriz(A)
        end = time.time()

        if N <= 10:
            print("\nMatriz inversa A^-1:")
            print(np.round(A_inv, 6))
        else:
            np.savetxt("inversa.csv", A_inv, delimiter=",", fmt="%.10f")
            print("\nMatriz inversa guardada en 'inversa.csv' (no se muestra por ser mayor de 10x10).")

        print(f"\nTiempo de ejecución: {end - start:.6f} segundos")
