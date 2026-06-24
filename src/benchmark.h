#pragma once
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;

class Benchmark{
public:
    high_resolution_clock::time_point start;

    void begin(){
        start = high_resolution_clock::now();
    }

    double end(){
        auto stop = high_resolution_clock::now();
        duration<double> diff = stop - start;
        return diff.count();
    }
};