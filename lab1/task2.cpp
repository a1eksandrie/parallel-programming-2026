#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <vector>
#include <iomanip> 
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <num_threads> <array_size>\n";
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int N = atoi(argv[2]);          // размер массива (для задачи = 16000)

    if (num_threads <= 0 || N < 3) {
        cerr << "Bad arguments\n";
        return 1;
    }

    omp_set_num_threads(num_threads);

    // Создаём и инициализируем массивы
    vector<double> a(N);
    vector<double> b(N, 0.0);       // сразу заполняем нулями

    // Инициализация a[i] = i  (можно параллельно, но для простоты последовательно)
    for (int i = 0; i < N; ++i) {
        a[i] = static_cast<double>(i);
    }

    // Параллельный цикл с разными schedule (несколько запусков с заменой)

#pragma omp parallel for schedule(guided)   // Вариант 1: static (по умолчанию)
    for (int i = 1; i < N - 1; ++i) {
        b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
    }

    // Печатаем несколько первых и последних элементов
    cout << "=== schedule(guided) ===\n";
    cout << fixed << setprecision(2);
    for (int i = 0; i < 5; ++i)
        cout << "b[" << i << "] = " << b[i] << "\n";
    cout << "...\n";
    for (int i = N - 5; i < N; ++i)
        cout << "b[" << i << "] = " << b[i] << "\n";

    return 0;
}