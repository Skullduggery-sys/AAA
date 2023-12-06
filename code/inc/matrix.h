#ifndef LAB03_MATRIX_H
#define LAB03_MATRIX_H

#include <vector>
#include <cassert>

template <typename T>
class Matrix {
public:
    Matrix(size_t n, size_t m);

    const std::vector<T>& operator[](size_t row) const {
        assert(row < n);
        return matrix[row];
    }

    std::vector<T>& operator[](size_t row) {
        assert(row < n);
        return matrix[row];
    }

    Matrix<T> operator+(const Matrix<T>& other) const;
    Matrix<T> operator-(const Matrix<T>& other) const;

    const size_t& getNumberOfRows(void) const { return n; }
    const size_t& getNumberOfColumns(void) const { return m; }

    Matrix<T> classicMultiplication(const Matrix<T>& operand);
    Matrix<T> classicWinograd(const Matrix<T>& operand);
    Matrix<T> optimizedWinograd(const Matrix<T>& operand);
    Matrix<T> strassenMultiplication(const Matrix<T>& operand);
private:
    size_t n, m;
    std::vector<std::vector<T>> matrix;
};
#endif //LAB03_MATRIX_H
