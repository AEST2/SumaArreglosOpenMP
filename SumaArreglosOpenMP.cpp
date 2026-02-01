
#include <iostream>   // Para imprimir en pantalla (cout) y leer (cin)
#include <vector>     // Para usar arreglos dinámicos (vector)
#include <random>     // Para generar números aleatorios
#include <iomanip>    // Para formato bonito al imprimir tablas
#include <omp.h>      // Librería de OpenMP (paralelismo)

using namespace std;

int main() {

    // =========================
    // 1) Presentación del programa
    // =========================
    cout << "=== Suma de Arreglos Paralela con OpenMP ===\n\n";

    // =========================
    // 2) Elegir cuántos hilos (threads) usar
    // =========================
    // Un "hilo" es como un "trabajador" que ejecuta una parte del trabajo.
    // Si tienes varios núcleos en tu CPU, puedes usar varios hilos.
    int nHilos;
    cout << "Cuantos hilos quieres trabajar (ej. 2, 4, 8): ";
    cin >> nHilos;

    // Si OpenMP está habilitado, _OPENMP estará definido automáticamente.
    // Con esto fijamos cuántos hilos usará OpenMP.
#ifdef _OPENMP
    omp_set_num_threads(nHilos);
    cout << "OpenMP esta habilitado. Usando " << nHilos << " hilos.\n\n";
#else
    cout << "WARNING: OpenMP NO esta habilitado. El programa correra secuencial.\n\n";
#endif

    // =========================
    // 3) Crear los arreglos A, B y C
    // =========================
    // La tarea sugiere usar 1000 elementos para que corra rápido.
    const int N = 1000;

    // A y B serán los arreglos de entrada.
    // C será el arreglo resultado donde guardamos la suma.
    vector<int> A(N), B(N), C(N);

    // =========================
    // 4) Llenar A y B con números aleatorios
    // =========================
    // Creamos un generador de números aleatorios con semilla fija
    // para que, si vuelves a ejecutar, salgan números similares (reproducible).
    mt19937 rng(12345);

    // Distribución uniforme: números entre 0 y 99
    uniform_int_distribution<int> dist(0, 99);

    // Llenamos A y B con valores aleatorios
    for (int i = 0; i < N; i++) {
        A[i] = dist(rng);
        B[i] = dist(rng);
    }

    // =========================
    // 5) Medir tiempo (para resultados)
    // =========================
    // omp_get_wtime() da el tiempo actual en segundos (tipo reloj).
    double start = omp_get_wtime();

    // =========================
    // 6) Suma paralela con OpenMP
    // =========================
    // Esta es la parte importante de la tarea:
    // Cada iteración i calcula C[i] = A[i] + B[i]
    //
    // Como cada C[i] es independiente, podemos repartir las iteraciones
    // del for entre varios hilos.
#pragma omp parallel for default(none) shared(A, B, C)
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }

    double end = omp_get_wtime();

    // =========================
    // 7) Mostrar una parte de los arreglos para comprobar
    // =========================
    // Imprimimos solo los primeros 10 valores para no llenar la pantalla.
    cout << " i |  A |  B |  C=A+B\n";
    cout << "----------------------\n";
    for (int i = 0; i < 10; i++) {
        cout << setw(2) << i << " | "
            << setw(2) << A[i] << " | "
            << setw(2) << B[i] << " | "
            << setw(3) << C[i] << "\n";
    }

    // =========================
    // 8) Validación (extra, pero muy útil)
    // =========================
    // Revisamos que en todas las posiciones se cumpla:
    // C[i] == A[i] + B[i]
    bool ok = true;
    for (int i = 0; i < N; i++) {
        if (C[i] != A[i] + B[i]) {
            ok = false;
            break;
        }
    }

    cout << "\nValidacion: " << (ok ? "OK" : "ERROR") << "\n";

    // =========================
    // 9) Mostrar el tiempo de ejecución
    // =========================
    cout << "Tiempo transcurrido: " << (end - start) << " segundos\n";

    return 0;
}