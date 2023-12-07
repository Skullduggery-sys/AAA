#ifndef LAB04_FUZZYCMEANS_H
#define LAB04_FUZZYCMEANS_H

#include <vector>
#include "../inc/matrix.h"
using byte = unsigned char;

struct Color {
    byte r;
    byte g;
    byte b;

    Color(byte r, byte g, byte b) : r(r), g(g), b(b) {}
};

struct Centroid {
    double r;
    double g;
    double b;
    Centroid(double r, double g, double b) : r(r), g(g), b(b) {}
};

struct Borders {
    size_t start;
    size_t end;

    Borders(size_t start, size_t end) : start(start), end(end) {}
};

void normalizeMembership(Matrix<double> &membership);

void fillMembership(Matrix<double> &membership);

void renewCenters(const Matrix<double> &membership, std::vector<Color *> &colors, double m, size_t start, size_t end, std::vector<Centroid *>& newCenter);

double distToCenter(Color *point, Centroid *center);

double calculateU(double dist, double m);

double decision(const std::vector<Color *> &colors, const std::vector<Centroid *> &centers, const Matrix<double> &membership);

void decision(const std::vector<Color *> &colors, const std::vector<Centroid *> &centers,
              const Matrix<double> &membership, double &distance, Borders &borders);

void helper(std::vector<Color*>& colors, std::vector<Centroid*>& centers, double m, size_t start, size_t end, Matrix<double> &membership);

void calculateDistances(const std::vector<Color*>& colors, const std::vector<Centroid*>& centers,
                        size_t start, size_t end, const double m, Matrix<double>& membership);

Matrix<double> parallelFuzzyCMeans(std::vector<Color *> &colors, size_t clustersCount, double m, std::vector<Centroid *>& centroids, size_t threadsCount);

Matrix<double> fuzzyCMeans(std::vector<Color *> &colors, size_t clustersCount, double m, std::vector<Centroid *>& centroids);

#endif //LAB04_FUZZYCMEANS_H
