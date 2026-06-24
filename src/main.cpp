#include <iostream>
#include <fstream>
#include <omp.h>
#include "grid.h"
#include "benchmark.h"
#include "sequential_bfs.h"
#include "openmp_bfs.h"
#include "opencl_bfs.h"
using namespace std;

int main(){
    Grid g(5000, 5000);
    g.generateRandom(0.25);

    Benchmark bm;

    cout << "Running Sequential BFS...\n";
    bm.begin();
    int seq = SequentialBFS::run(g);
    double t1 = bm.end();

    cout << "Running OpenMP BFS...\n";
    bm.begin();
    int omp = OpenMPBFS::run(g);
    double t2 = bm.end();

    cout << "Running OpenCL BFS...\n";
    bm.begin();
    int cl = OpenCLBFS::run(g);
    double t3 = bm.end();

    cout << "\n===== RESULTS =====\n";
    cout << "Sequential: " << t1 << " sec\n";
    cout << "OpenMP    : " << t2 << " sec\n";
    cout << "OpenCL    : " << t3 << " sec\n";

    double speedup_omp = t1 / t2;
    double speedup_cl = t1 / t3;

    int omp_threads = omp_get_max_threads();
    double efficiency_omp = speedup_omp / omp_threads;

    cout << "\n\t\t| OpenMP \t| OpenCL\n";
    cout << "Speedup\t\t| " << speedup_omp << "x \t| " << speedup_cl << "x\n";
    cout << "Efficiency\t| " << efficiency_omp << " \t|";

    ofstream res_file("results.txt");
    if (res_file.is_open()) {
        res_file << t1 << "\n";
        res_file << t2 << "\n";
        res_file << t3 << "\n";
        res_file << speedup_omp << "\n";
        res_file << speedup_cl << "\n";
        res_file << efficiency_omp << "\n";
        res_file << omp_threads << "\n";
        res_file.close();
        cout << "\nData written successfully to results.txt";
    } else {
        cout << "\nCould not write data to results.txt";
    }

    return 0;
}