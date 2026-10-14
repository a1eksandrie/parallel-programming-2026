#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

struct Point {
    double x, y, z;
};

// ===================== Генерация файла =====================
void generate_points(const string& filename, int count) {
    ofstream out(filename);
    if (!out) {
        cerr << "Cannot create file " << filename << endl;
        return;
    }

    srand(time(nullptr));

    for (int i = 0; i < count; ++i) {
        double x = (rand() % 20000 - 10000) / 100.0;
        double y = (rand() % 20000 - 10000) / 100.0;
        double z = (rand() % 20000 - 10000) / 100.0;
        out << x << " " << y << " " << z << "\n";
    }

    cout << "File \"" << filename << "\" created with " << count << " points.\n";
}

// ===================== Чтение точек =====================
bool read_points(const string& filename, vector<Point>& points) {
    ifstream in(filename);
    if (!in) return false;

    double x, y, z;
    while (in >> x >> y >> z) {
        points.push_back({ x, y, z });
    }
    return !points.empty();
}

// ===================== Вычисление по формуле с critical =====================
double compute_formula(const vector<Point>& points) {
    double global_sum = 0.0;
    int n = (int)points.size();

#pragma omp parallel
    {
        double local_sx = 0.0;
        double local_sy = 0.0;
        double local_sz = 0.0;

#pragma omp sections
        {
#pragma omp section
            {for (int i = 0; i < n; ++i) local_sx += points[i].x;}
#pragma omp section
            {for (int i = 0; i < n; ++i) local_sy += points[i].y;}
#pragma omp section
            {for (int i = 0; i < n; ++i) local_sz += points[i].z;}
        }

        // Безопасное добавление в глобальную сумму
#pragma omp critical
        {global_sum += local_sx + local_sy + local_sz;}
    }

    return global_sum / (3.0 * n);
}

// ===================== main =====================
int main(int argc, char* argv[]) {
    // Режим генерации файла
    if (argc >= 2 && string(argv[1]) == "generate") {
        int count = (argc >= 3) ? atoi(argv[2]) : 1000000;
        generate_points("points.txt", count);
        return 0;
    }

    if (argc != 3) {
        cerr << "Usage:\n";
        cerr << "  Generate:  " << argv[0] << " generate [count]\n";
        cerr << "  Calculate: " << argv[0] << " <points_file> <num_threads>\n";
        return 1;
    }

    string filename = argv[1];
    int threads = atoi(argv[2]);
    omp_set_num_threads(threads);

    vector<Point> points;
    if (!read_points(filename, points)) {
        cerr << "Cannot read points from " << filename << endl;
        return 1;
    }

    cout << "Points loaded: " << points.size() << endl;
    cout << "Threads: " << threads << endl << endl;

    auto start = chrono::high_resolution_clock::now();
    double result = compute_formula(points);
    auto end = chrono::high_resolution_clock::now();
    double time = chrono::duration<double>(end - start).count();

    cout << fixed << setprecision(8);
    cout << "Result = " << result << endl;
    cout << "Time   = " << time << " s" << endl;

    return 0;
}
