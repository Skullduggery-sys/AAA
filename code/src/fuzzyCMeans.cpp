#include "../inc/fuzzyCMeans.h"
#include "../inc/time_measurment.h"

#include <thread>
#include <mutex>
#include <random>
#include <fstream>
#include <shared_mutex>
#include <memory>
#include <iostream>

#define MAXREPS 150
#define EPS 0.001

std::shared_mutex myMutex;

std::vector<Centroid *> generateRandCenters(size_t clustersCount) {
    std::random_device rd;
    std::mt19937 gen(rd()); // Используем Mersenne Twister engine
    std::uniform_int_distribution<int> distribution(0, 255);
    std::vector<Centroid *> centers(clustersCount);
    for (size_t i = 0; i < clustersCount; i++) {
        centers[i] = new Centroid(distribution(gen), distribution(gen), distribution(gen));
    }

    return std::move(centers);
}

Matrix<double> parallelFuzzyCMeans(std::vector<Color *> &colors, size_t clustersCount, double m, std::vector<Centroid *>& centroids, size_t threadsCount) {
    std::vector<Centroid *> centers = generateRandCenters(clustersCount);
    Matrix<double> membership(colors.size(), clustersCount);
    fillMembership(membership);

    size_t chunkCount;
    if (clustersCount < threadsCount) {
        threadsCount = clustersCount;
        chunkCount = 1;
    } else {
        chunkCount = clustersCount / threadsCount;
    }

    std::vector<std::thread> threads;
    for (size_t t = 0; t < threadsCount; t++) {
        size_t start = t * chunkCount;
        size_t end = t == threadsCount - 1 ? clustersCount : (t + 1) * chunkCount;
        threads.emplace_back(helper, std::ref(colors), std::ref(centers), m, start, end, std::ref(membership));
    }

    for (auto& thread : threads) {
        thread.join();
    }
    // printf("Размерность: %zu, Потоков: %zu, Время %zu\n", colors.size(), threadsCount, end - start);

    centroids = centers;
    return std::move(membership);
}

void helper(std::vector<Color*>& colors, std::vector<Centroid*>& centers, double m, size_t start, size_t end, Matrix<double> &membership) {
    double prevDecision = 0;
    double currDecision = 1;
    for (size_t i = 0; i < MAXREPS && std::abs(currDecision - prevDecision) > EPS; i++) { //
        prevDecision = currDecision;

        renewCenters(membership, colors, m, start, end, centers);

        calculateDistances(colors, centers, start, end, m, membership);

        if (myMutex.try_lock()) {
            normalizeMembership(membership);
            myMutex.unlock();
        }
        Borders border(start, end);
        decision(colors, centers, membership, currDecision, border);
    }
}

void calculateDistances(const std::vector<Color*>& colors, const std::vector<Centroid*>& centers,
                        size_t start, size_t end, const double m, Matrix<double>& membership) {
    for (size_t j = 0; j < membership.getNumberOfRows(); j++) {
        for (size_t k = start; k < end; k++) {
            auto dist = distToCenter(colors[j], centers[k]);
            membership[j][k] = calculateU(dist, m);
        }
    }
}

Matrix<double> fuzzyCMeans(std::vector<Color *> &colors, size_t clustersCount, double m, std::vector<Centroid *>& centroids) {
    std::vector<Centroid *> centers = generateRandCenters(clustersCount);
    Matrix<double> membership(colors.size(), clustersCount);
    fillMembership(membership);
    // отсюда
    double prevDecision = 0;
    double currDecision = 1;
    for (size_t i = 0; i < MAXREPS && std::abs(currDecision - prevDecision) > EPS; i++) {
        prevDecision = currDecision;

        renewCenters(membership, colors, m, 0, membership.getNumberOfColumns(), centers);
        for (size_t j = 0; j < membership.getNumberOfRows(); j++) {
            for (size_t k = 0; k < membership.getNumberOfColumns(); k++) {
                auto dist = distToCenter(colors[j], centers[k]);
                membership[j][k] = calculateU(dist, m);
            }
        }
        normalizeMembership(membership);
        currDecision = decision(colors, centers, membership);
    }

    centroids = centers;
    return membership;
}

void decision(const std::vector<Color *> &colors, const std::vector<Centroid *> &centers,
              const Matrix<double> &membership, double &distance, Borders &borders) {
    double sum = 0;
    for (size_t i = 0; i < membership.getNumberOfRows(); i++) {
        for (size_t j = borders.start; j < borders.end; j++) {
            sum += membership[i][j] * distToCenter(colors[i], centers[j]);
        }
    }
    distance = sum;
}

double decision(const std::vector<Color *> &colors, const std::vector<Centroid *> &centers, const Matrix<double> &membership) {
    double sum = 0;
    for (size_t i = 0; i < colors.size(); i++) {
        for (size_t j = 0; j < centers.size(); j++) {
            sum += membership[i][j] * distToCenter(colors[i], centers[j]);
        }
    }
    return sum;
}

double distToCenter(Color *point, Centroid *center) {
    return sqrt(pow(point->r - center->r, 2) + pow(point->g - center->g, 2) + pow(point->b - center->b, 2));
}

double calculateU(double dist, double m) {
    return pow(1 / dist, 2 / (m - 1));
}

void renewCenters(const Matrix<double> &membership, std::vector<Color *> &colors, double m, size_t start, size_t end, std::vector<Centroid *>& newCenter) {
    for (size_t i = start; i < end; i++) {
//        newCenter[i] = new Centroid(0.0, 0.0, 0.0);
        if (!newCenter[i]) {
            newCenter[i] = new Centroid(0.0, 0.0, 0.0);
        }
        double tempA = 0.0;
        double tempBr = 0.0;
        //double tempAg = 0.0;
        double tempBg = 0.0;
        //double tempAb = 0.0;
        double tempBb = 0.0;


        for (size_t key = 0; key < membership.getNumberOfRows(); ++key) {
            tempA += std::pow(membership[key][i], m);
            tempBr += std::pow(membership[key][i], m) * colors[key]->r;

            //tempAg += std::pow(membership[key][i], m);
            tempBg += std::pow(membership[key][i], m) * colors[key]->g;

            //tempAb += std::pow(membership[key][i], m);
            tempBb += std::pow(membership[key][i], m) * colors[key]->b;
        }
        newCenter[i]->r = tempBr / tempA;
        newCenter[i]->g = tempBg / tempA;
        newCenter[i]->b = tempBb / tempA;
    }
}

void fillMembership(Matrix<double> &membership) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < membership.getNumberOfRows(); i++) {
        for (size_t j = 0; j < membership.getNumberOfColumns(); j++) {
            membership[i][j] = distribution(gen);
        }
    }

    normalizeMembership(membership);
}

void normalizeMembership(Matrix<double> &membership) {
    for (size_t i = 0; i < membership.getNumberOfRows(); i++) {
        double sum = 0;
        for (auto weight: membership[i]) {
            sum += weight;
        }

        for (size_t j = 0; j < membership.getNumberOfColumns(); j++) {
            membership[i][j] /= sum;
        }
    }
}