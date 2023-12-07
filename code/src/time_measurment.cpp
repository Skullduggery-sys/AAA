#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <ostream>
#include <time.h>
#include <vector>
#include <chrono>
#include "../inc/matrix.h"
#include "../inc/time_measurment.h"
#include "../inc/fuzzyCMeans.h"
#include <algorithm>
#define REPS 2000
using namespace std;

std::vector<Color*> generate_random_vector(size_t len) {

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribution(0, 255);
    std::vector<Color*> colors;

    for (size_t i = 0; i < len; i++) {
        colors.push_back(new Color(distribution(gen), distribution(gen), distribution(gen)));
    }

    return std::move(colors);
}

long get_time() {
    struct timespec t;
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t)) {
        return 0;
    }

    return t.tv_sec * 10e9 + t.tv_nsec;
}

double get_cpu_time_t (std::vector<Color *> &colors, size_t &clustersCount, double m, size_t threadsCount)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start_t, end_t;
    double total_t;
    std::vector<Centroid *> centroids;
    if (!threadsCount) {
        start_t = std::chrono::high_resolution_clock::now();
        fuzzyCMeans(colors, clustersCount, m, centroids);
        end_t = std::chrono::high_resolution_clock::now();
    } else {
        start_t = std::chrono::high_resolution_clock::now();
        parallelFuzzyCMeans(colors, clustersCount, m, centroids, threadsCount);
        end_t = std::chrono::high_resolution_clock::now();
    }
    total_t = (double) std::chrono::duration_cast<std::chrono::microseconds>(end_t - start_t).count() ;// CLOCKS_PER_SEC;
    return total_t;
}

void get_time(const std::string& filename, size_t max_len) {
    std::ofstream f(filename);
    if (!f) {
        std::cout << "Не удалось открыть файл";
        return;
    }
    _get_time(f, max_len);
}


void _get_time(std::ofstream& stream, size_t max_len) {
    size_t i = 2;
    std::locale locale("C");
    stream.imbue(locale);

    size_t threadsCount = 64;
    stream << "len,0,1,2,4,8,16,32,64" << std::endl;
    double m = 1.5;
    while (i <= max_len) {
        vector<int> time(threadsCount, 0);
        auto colors = generate_random_vector(i);
        size_t clustersCount = colors.size();
        size_t t = 0;
        size_t counter = 0;
        while (t <= threadsCount) {
            for (size_t j = 0; j < REPS; j++) {
                time[counter] += get_cpu_time_t(colors, clustersCount, m, t);
            }

            if (t == 0)
                t++;
            else {
                t *= 2;
            }
            counter++;
        }
        stream << i;
        for (size_t i = 0; i < 8; i++) {
            time[i] /= REPS;
            stream << "," << time[i];
        }
        stream << endl;
        i += 10;
        stream.flush();
    }
}


//void get_concrete_time(size_t len) {
//    double time_classic = 0;
//    double time_winograd = 0;
//    double time_optimizedWinograd = 0;
//    double time_strassen = 0;
//
//    auto mA = generate_random_matrix(len);
//    auto mB = generate_random_matrix(len);
//    for (size_t j = 0; j < REPS; j++) {
//        time_classic += get_cpu_time_t(mA, mB, &Matrix<int>::classicMultiplication);
//        time_winograd += get_cpu_time_t(mA, mB, &Matrix<int>::classicWinograd);
//        time_optimizedWinograd += get_cpu_time_t(mA, mB, &Matrix<int>::optimizedWinograd);
//        time_strassen += get_cpu_time_t(mA, mB, &Matrix<int>::strassenMultiplication);
//    }
//
//    time_classic /= REPS;
//    time_winograd  /= REPS;
//    time_optimizedWinograd /= REPS;
//    time_strassen /= REPS;
//    std::cout << "len,classic,winograd,optimizedWinograd,strassen" << std::endl;
//    std::cout << len << "," << time_classic << "," << time_winograd << "," << time_optimizedWinograd << "," << time_strassen << std::endl;
//}