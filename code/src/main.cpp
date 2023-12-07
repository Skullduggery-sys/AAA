#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <fstream>
#include <time.h>
#include <chrono>
#include <sys/time.h>
#include "../inc/fuzzyCMeans.h"
#include "../inc/time_measurment.h"

void func(size_t start, size_t end, Matrix<double> &matrix, int &i);

static long long getThreadCpuTimeNs() {
    struct timespec t;
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t)) {
        perror("clock_gettime");
        return 0;
    }
    return t.tv_sec * 1000000LL + t.tv_nsec / 1000LL;
}

long long
mtime()
{
    struct timeval t;

    gettimeofday(&t, NULL);
    long long mt = (long long)t.tv_sec * 1000000 + t.tv_usec;
    return mt;
}

int main() {
//    size_t clustersCount = 500;
//    double m = 1.5;
//    size_t threadsCount = 64;
//    std::vector<Color*> colors;
//
//    std::random_device rd;
//    std::mt19937 gen(rd());
//
//    std::uniform_int_distribution<int> distribution(0, 255);
//
//    for (size_t i = 0; i < clustersCount; i++) {
//        colors.push_back(new Color(distribution(gen), distribution(gen), distribution(gen)));
//    }
//
//    std::ofstream outFile("output.csv");
//    outFile << "r,g,b";
//    for (size_t i = 0; i < clustersCount; i++) {
//        outFile << ",cluster_" << i + 1;
//    }
//    outFile << std::endl;
//    std::vector<Centroid *> centers(clustersCount);
//    Matrix<double> result = parallelFuzzyCMeans(colors, clustersCount, m, centers, threadsCount);
//    for (size_t i = 0; i < result.getNumberOfRows(); i++) {
//        outFile << (int) colors[i]->r << "," << (int) colors[i]->g << "," << (int) colors[i]->b << ",";
//        for (size_t j = 0; j < result.getNumberOfColumns(); j++) {
//            outFile << result[i][j];
//            if (j != result.getNumberOfColumns() - 1)
//                outFile << ",";
//        }
//        outFile << std::endl;
//    }
//
//    for (auto & center : centers) {
//        std::cout << "[" <<  center->r << "," << center->g << "," << center->b << "]," << std::endl;
//    }

    get_time("time.csv", 200);

//    std::cout << std::thread::hardware_concurrency();

//    size_t len = 5000;
//    size_t threadsCount = 8;
//
//
//    size_t chunkCount = len / threadsCount;
//    std::vector<std::thread> threads;
//
//    Matrix<double> matrix(len, len);
//    size_t NewStart = getThreadCpuTimeNs();
//    size_t NewNewStart = mtime();
//    size_t ProcTimeStart = get_time();
//    int i = 0;
//    auto start = std::chrono::high_resolution_clock::now();;
//    for (size_t t = 0; t < threadsCount; t++) {
//        size_t start = t * chunkCount;
//        size_t end = t == threadsCount - 1 ? len : (t + 1) * chunkCount;
//        threads.emplace_back(func, start, end, std::ref(matrix), std::ref(i));
//    }
//
//    for (auto& thread : threads) {
//        thread.join();
//    }
//    size_t ProcTimeEnd = get_time();
//    auto end = std::chrono::high_resolution_clock::now();
//    size_t NewEnd = getThreadCpuTimeNs();
//    size_t NewNewEnd = mtime();
//    std::cout << threadsCount << " потоков (chrono): " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
//    std::cout << threadsCount << " потоков (get_time): " << NewEnd - NewStart << std::endl;
//    std::cout << threadsCount << " потоков (gettimeOfDay): " << NewNewEnd - NewNewStart << std::endl;
//    std::cout << threadsCount << " потоков (proctime): " << (ProcTimeEnd - ProcTimeStart) / static_cast<double>(threadsCount) << std::endl;
//    std::cout << threadsCount << " потоков (clo): " << threadsCount << std::endl;
//    func(0, len, len);

   // std::cout << "0 потоков: " << end - start << std::endl;

    return 0;
}

void func(size_t start, size_t end, Matrix<double>&matrix, int &i) {
    double sum = 0;
    //std::cout << start << std::endl;

    for (size_t i = 0; i < matrix.getNumberOfRows(); i++) {
        for (size_t j = start; j < end; j++) {
            sum += matrix[i][j];
        }
    }
}