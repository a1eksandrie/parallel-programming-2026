#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

// ------- Способ 3. Использование барьеров (barrier) -------
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
        int my_id = omp_get_thread_num();
        int total = omp_get_num_threads();

         Идём от большего номера к меньшему
        for (int id = total - 1; id >= 0; --id) {
            if (my_id == id) {
                cout << "Thread " << my_id << " of " << total << ": Hello World\n";
            }
                         Все потоки ждут, пока текущий id не напечатает
#pragma omp barrier
        }
    }

    return 0;
}
