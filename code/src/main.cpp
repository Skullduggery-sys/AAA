#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "../inc/fuzzyCMeans.h"
#include "../inc/time_measurment.h"

int main() {
//    size_t clustersCount = 1000;
//    double m = 1.5;
//    size_t threadsCount = 64;
//    std::vector<Color*> colors;
//
//    std::random_device rd;
//    std::mt19937 gen(rd());
//
//    std::uniform_int_distribution<int> distribution(0, 255);
//
//    for (size_t i = 0; i < 1000; i++) {
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

    get_time("time.csv", 1000);

    return 0;
}
