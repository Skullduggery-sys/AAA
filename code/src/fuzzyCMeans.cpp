#include "../inc/fuzzyCMeans.h"
#include "../inc/threadPool.h"

#include <thread>
#include <mutex>
#include <random>
#include <fstream>
#include <iostream>
#include <memory>

#define MAXREPS 150
#define EPS 0.001

std::mutex myMutex;

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

    double prevDecision = 0;
    double currDecision = 1;
    ThreadPool tp(threadsCount);
    for (size_t i = 0; i < MAXREPS && std::abs(currDecision - prevDecision) > EPS; i++) {
        prevDecision = currDecision;



        size_t chunksCount = clustersCount / threadsCount;
        for (size_t t = 0; t < threadsCount; t++) {
            size_t start = t * chunksCount;
            size_t end = (t == threadsCount - 1) ? clustersCount : (t + 1) * chunksCount;
            tp.Submit(renewCenters, std::ref(membership), std::ref(colors), m, start, end, std::ref(centers));
        }
        tp.Wait();
        //renewCenters(membership, colors, m, 0, membership.getNumberOfColumns(), centers);

        chunksCount = colors.size() / threadsCount;
        for (size_t t = 0; t < threadsCount; t++) {
            size_t start = t * chunksCount;
            size_t end = (t == threadsCount - 1) ? colors.size() : (t + 1) * chunksCount;
            tp.Submit(calculateDistances, std::ref(colors), std::ref(centers), start, end, m, std::ref(membership));
        }

        tp.Wait();

        for (size_t t = 0; t < threadsCount; t++) {
            size_t start = t * chunksCount;
            size_t end = (t == threadsCount - 1) ? colors.size() : (t + 1) * chunksCount;
            Borders borders(start, end);
            tp.Submit(parallelNormalizeMembership, std::ref(membership),borders);
        }
        tp.Wait();
        //normalizeMembership(membership);
        currDecision = decision(colors, centers, membership);
    }
    tp.Stop();
    centroids = centers;
    return std::move(membership);
}

void calculateDistances(const std::vector<Color*>& colors, const std::vector<Centroid*>& centers,
                        size_t start, size_t end, const double m, Matrix<double>& membership) {
    for (size_t j = start; j < end; j++) {
        for (size_t k = 0; k < membership.getNumberOfColumns(); k++) {
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
    for (size_t i = borders.start; i < borders.end; i++) {
        for (size_t j = 0; j < centers.size(); j++) {
            sum += membership[i][j] * distToCenter(colors[i], centers[j]);
        }
    }
    myMutex.lock();
    distance += sum;
    myMutex.unlock();
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

void parallelNormalizeMembership(Matrix<double> &membership, Borders borders) {
    for (size_t i = borders.start; i < borders.end; i++) {
        double sum = 0;
        //std::cout << "read " << borders.start << " " << borders.end << " " << i << " " << " " << std::endl;
        for (size_t j = 0; j < membership.getNumberOfColumns(); j++) {
            sum += membership[i][j];
        }

        //std::cout << "write " << borders.start << " " << borders.end << " " << i << " " << " " << std::endl;
        for (size_t j = 0; j < membership.getNumberOfColumns(); j++) {
            membership[i][j] = membership[i][j] /  sum;
        }
    }
}