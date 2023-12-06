#include "../inc/matrix.h"
#include <cassert>

template <typename T>
Matrix<T>::Matrix(size_t n, size_t m) : n(n), m(m) {
    matrix = std::vector<std::vector<T>>(n);
    for (size_t i = 0; i < n; i++) {
        matrix[i] = std::vector<T>(m);
    }
}


template <typename T>
Matrix<T> Matrix<T>::optimizedWinograd(const Matrix<T>& operand) {
    assert(getNumberOfColumns() == operand.getNumberOfRows());

    std::vector<T> rowFactor(getNumberOfColumns());
    for (size_t i = 0; i < getNumberOfRows(); i++) {
        size_t q = getNumberOfColumns() >> 1;
        for (size_t j = 0; j < q; j++) {
            rowFactor[i] += matrix[i][(j << 1) + 1] * matrix[i][j << 1];
        }
    }

    std::vector<T> columnFactor(operand.getNumberOfRows());
    for (size_t i = 0; i < operand.getNumberOfColumns(); i++) {
        size_t n = operand.getNumberOfColumns() >> 1;
        for (size_t j = 0; j < n; j++) {
            columnFactor[i] += operand[(j << 1) + 1][i] * operand[j << 1][i];
        }
    }

    Matrix<T> result(getNumberOfRows(),operand.getNumberOfColumns());
    for (size_t i = 0; i < getNumberOfRows(); ++i) {
        for (size_t j = 0; j < operand.getNumberOfColumns(); ++j) {
            result[i][j] = -rowFactor[i] - columnFactor[j];
            size_t n = getNumberOfColumns() >> 1;
            for (size_t k = 0; k < n; ++k) {
                result[i][j] += (matrix[i][k << 1] + operand[(k << 1) + 1][j]) * (matrix[i][(k << 1) + 1] + operand[k << 1][j]);
            }
        }
    }

    if (getNumberOfColumns() % 2 != 0) {
        for (size_t i = 0; i < getNumberOfRows(); ++i) {
            for (size_t j = 0; j < operand.getNumberOfColumns(); ++j) {
                result[i][j] += matrix[i][getNumberOfColumns() - 1] * operand[getNumberOfColumns() - 1][j];
            }
        }
    }

    return std::move(result);
}

template <typename T>
Matrix<T> Matrix<T>::classicMultiplication(const Matrix<T>& operand) {
    assert(getNumberOfColumns() == operand.getNumberOfRows());

    Matrix<T> result(getNumberOfRows(), operand.getNumberOfColumns());

    for (size_t i = 0; i < getNumberOfRows(); i++) {
        for (size_t j = 0; j < operand.getNumberOfColumns(); j++) {
            for (size_t k = 0; k < getNumberOfColumns(); k++) {
                result[i][j] = result[i][j] + matrix[i][k] * operand[k][j];
            }
        }
    }

    return std::move(result);
}

template <typename T>
Matrix<T> Matrix<T>::classicWinograd(const Matrix<T>& operand) {
    assert(getNumberOfColumns() == operand.getNumberOfRows());

    std::vector<T> rowFactor(getNumberOfRows());
    for (size_t i = 0; i < getNumberOfRows(); i++) {
        for (size_t j = 0; j < getNumberOfColumns() / 2; j++) {
            rowFactor[i] = rowFactor[i] + matrix[i][2 * j + 1] * matrix[i][2 * j];
        }
    }

    std::vector<T> columnFactor(operand.getNumberOfColumns());
    for (size_t i = 0; i < operand.getNumberOfColumns(); i++) {
        for (size_t j = 0; j < operand.getNumberOfRows() / 2; j++) {
            columnFactor[i] = columnFactor[i] + operand[2 * j + 1][i] * operand[2 * j][i];
        }
    }

    Matrix<T> result(getNumberOfRows(),operand.getNumberOfColumns());
    for (size_t i = 0; i < getNumberOfRows(); ++i) {
        for (size_t j = 0; j < operand.getNumberOfColumns(); ++j) {
            result[i][j] = -rowFactor[i] - columnFactor[j];
            for (size_t k = 0; k < getNumberOfColumns() / 2; ++k) {
                result[i][j] = result[i][j] + (matrix[i][2 * k] + operand[2 * k + 1][j]) * (matrix[i][2 * k + 1] + operand[2 * k][j]);
            }
        }
    }

    if (getNumberOfColumns() % 2 != 0) {
        for (size_t i = 0; i < getNumberOfRows(); ++i) {
            for (size_t j = 0; j < operand.getNumberOfColumns(); ++j) {
                result[i][j] = result[i][j] + matrix[i][getNumberOfColumns() - 1] * operand[getNumberOfColumns() - 1][j];
            }
        }
    }

    return std::move(result);
}

template <typename T>
Matrix<T> Matrix<T>::strassenMultiplication(const Matrix<T>& operand) {
    assert(getNumberOfColumns() == operand.getNumberOfRows());

    size_t n = getNumberOfRows();
    size_t m = operand.getNumberOfColumns();
    size_t k = getNumberOfColumns();

    if (n == 1 || m == 1 || k == 1) {
        Matrix<T> result(1, 1);
        result[0][0] = matrix[0][0] * operand[0][0];
        return result;
    }

    size_t halfSize = n / 2;

    // Разделение матрицы на четыре части
    Matrix<T> A11(halfSize, halfSize);
    Matrix<T> A12(halfSize, halfSize);
    Matrix<T> A21(halfSize, halfSize);
    Matrix<T> A22(halfSize, halfSize);

    Matrix<T> B11(halfSize, halfSize);
    Matrix<T> B12(halfSize, halfSize);
    Matrix<T> B21(halfSize, halfSize);
    Matrix<T> B22(halfSize, halfSize);

    for (size_t i = 0; i < halfSize; ++i) {
        for (size_t j = 0; j < halfSize; ++j) {
            A11[i][j] = matrix[i][j];
            A12[i][j] = matrix[i][j + halfSize];
            A21[i][j] = matrix[i + halfSize][j];
            A22[i][j] = matrix[i + halfSize][j + halfSize];

            B11[i][j] = operand[i][j];
            B12[i][j] = operand[i][j + halfSize];
            B21[i][j] = operand[i + halfSize][j];
            B22[i][j] = operand[i + halfSize][j + halfSize];
        }
    }

    Matrix<T> P1 = (A11).strassenMultiplication(B12 - B22);
    Matrix<T> P2 = (A11 + A12).strassenMultiplication(B22);
    Matrix<T> P3 = (A21 + A22).strassenMultiplication(B11);
    Matrix<T> P4 = (A22).strassenMultiplication(B21 - B11);
    Matrix<T> P5 = (A11 + A22).strassenMultiplication(B11 + B22);
    Matrix<T> P6 = (A12 - A22).strassenMultiplication(B21 + B22);
    Matrix<T> P7 = (A11 - A21).strassenMultiplication(B11 + B12);

    Matrix<T> C11 = (P5 + P4 + P6 - P2);
    Matrix<T> C12 = (P1 + P2);
    Matrix<T> C21 = (P3 + P4);
    Matrix<T> C22 = (P5 + P1 - P3 - P7);

    Matrix<T> result(n, m);

    for (size_t i = 0; i < halfSize; ++i) {
        for (size_t j = 0; j < halfSize; ++j) {
            result[i][j] = C11[i][j];
            result[i][j + halfSize] = C12[i][j];
            result[i + halfSize][j] = C21[i][j];
            result[i + halfSize][j + halfSize] = C22[i][j];
        }
    }

    return std::move(result);
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
    assert(getNumberOfRows() == other.getNumberOfRows() && getNumberOfColumns() == other.getNumberOfColumns());

    size_t n = getNumberOfRows();
    size_t m = getNumberOfColumns();

    Matrix<T> result(n, m);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result[i][j] = matrix[i][j] + other[i][j];
        }
    }

    return std::move(result);
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
    assert(getNumberOfRows() == other.getNumberOfRows() && getNumberOfColumns() == other.getNumberOfColumns());

    size_t n = getNumberOfRows();
    size_t m = getNumberOfColumns();

    Matrix<T> result(n, m);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result[i][j] = matrix[i][j] - other[i][j];
        }
    }

    return std::move(result);
}

template class Matrix<int>;
template class Matrix<double>;