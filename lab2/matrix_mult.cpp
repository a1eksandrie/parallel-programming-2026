#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include <iomanip>
#include <cmath>
using namespace std;

using Matrix = vector<vector<double>>;

Matrix create_matrix(int n, double value = 1.0) {
    return Matrix(n, vector<double>(n, value));
}

// Последовательное умножение
void multiply_sequential(const Matrix& A, const Matrix& B, Matrix& C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

// Параллельное умножение (по строкам)
void multiply_parallel(const Matrix& A, const Matrix& B, Matrix& C, int n) {
#pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void print_matrix(const Matrix& M, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // Если число целое — печатаем без дробной части
            if (M[i][j] == static_cast<int>(M[i][j])) {
                cout << setw(1) << static_cast<int>(M[i][j]) << " ";
            }
            else {
                cout << setw(1) << fixed << setprecision(2) << M[i][j] << " ";
            }
        }
        cout << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <N> <num_threads>\n";
        return 1;
    }

    int N = atoi(argv[1]);
    int threads = atoi(argv[2]);

    if (N <= 0 || threads <= 0) {
        cerr << "N and num_threads must be positive\n";
        return 1;
    }

    omp_set_num_threads(threads);

    Matrix A = create_matrix(N, 1.5);
    Matrix B = create_matrix(N, 2.0);
    Matrix C_seq(N, vector<double>(N, 0.0));
    Matrix C_par(N, vector<double>(N, 0.0));

    // Последовательная версия
    auto start = chrono::high_resolution_clock::now();
    multiply_sequential(A, B, C_seq, N);
    auto end = chrono::high_resolution_clock::now();
    double time_seq = chrono::duration<double>(end - start).count();

    // Параллельная версия
    start = chrono::high_resolution_clock::now();
    multiply_parallel(A, B, C_par, N);
    end = chrono::high_resolution_clock::now();
    double time_par = chrono::duration<double>(end - start).count();


    cout << fixed << setprecision(6);
    cout << "Sequential time:    " << time_seq << " s\n";
    cout << "Parallel time:      " << time_par << " s\n";
    cout << "Speedup (seq/par):  " << (time_seq / time_par) << "x\n";

    // Выводим матрицу только если N <= 15
    if (N <= 15) {
        cout << "\nResult matrix (parallel):\n";
        print_matrix(C_par, N);
    }

    return 0;
}
