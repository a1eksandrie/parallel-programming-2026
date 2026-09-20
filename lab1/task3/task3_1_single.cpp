#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

// ------- Cпособ 1. конструкция single -------
 
int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "укажите число потоков\n";
        cerr << "пример: " << argv[0] << " 8\n";
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        cerr << "число потоков должно быть положительным\n";
        return 1;
    }

    omp_set_num_threads(num_threads);

#pragma omp parallel
    {
        int total = omp_get_num_threads();

#pragma omp single
        {
            // один поток печатает все номера в обратном порядке
            for (int id = total - 1; id >= 0; --id) {
                cout << "thread " << id << " of " << total << ": hello world\n";
            }
        }
    }
    return 0;
}
