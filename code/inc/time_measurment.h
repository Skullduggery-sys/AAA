#ifndef LAB04_TIME_MEASURMENT_H
#define LAB04_TIME_MEASURMENT_H

#include <iostream>
#include <vector>
#include "fuzzyCMeans.h"

Matrix<int> generate_random_matrix(size_t len);

long get_time();

double get_cpu_time_t (std::vector<Color *> &colors, size_t &clustersCount, double m, size_t threadsCount);

void get_time(const std::string& filename, size_t max_len);

void _get_time(std::ofstream& stream, size_t max_len);

void get_concrete_time(size_t len);


#endif //LAB04_TIME_MEASURMENT_H
