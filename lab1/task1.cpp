using namespace std;
#include <iostream>
#include <omp.h>          

int main(int argc, char* argv[]) {

    int num_threads;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }
        
    omp_set_num_threads(num_threads);   // Устанавливаем количество потоков
        
#pragma omp parallel                    // Параллельная область (здесь создаются потоки)
    {
        int id = omp_get_thread_num();  // Каждый поток получает свой уникальный номер (0, 1, 2, ...)
        int nthreads = omp_get_num_threads();       // Общее количество потоков в этой параллельной области
        cout << "Hello World from thread " << id    // Печатаем (порядок вывода не гарантирован)
            << " of " << nthreads << endl;
    }
    // После закрывающей скобки все потоки (кроме главного) завершаются

    return 0;
}


