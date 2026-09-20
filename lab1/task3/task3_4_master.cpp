#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

// ------- Способ 4. Директива master -------
int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Укажите число потоков\n";
        cerr << "Пример: " << argv[0] << " 8\n";
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        cerr << "Число потоков должно быть положительным\n";
        return 1;
    }

    omp_set_num_threads(num_threads);

#pragma omp parallel
    {
        int total = omp_get_num_threads();

#pragma omp master
        {
            // Только главный поток (номер 0) печатает все номера
            for (int id = total - 1; id >= 0; --id) {
                cout << "Thread " << id << " of " << total << ": Hello World\n";
            }
        }
    }

    return 0;
}
