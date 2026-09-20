#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

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

    int next = 0;   // общий счётчик (shared)

#pragma omp parallel shared(next)
    {
        int my_id = omp_get_thread_num();
        int total = omp_get_num_threads();

        while (true) {
#pragma omp critical
            {
                if (next >= total) {
                    // все уже напечатали — выходим
                }
                else {
                    int id_to_print = total - 1 - next;
                    if (my_id == id_to_print) {
                        cout << "Thread " << my_id << " of " << total << ": Hello World\n";
                        ++next;
                    }
                }
            }

            // Проверяем условие выхода вне critical
            if (next >= total) break;
        }
    }

    return 0;
}
