#include "matrix.h"
#include <stdexcept>
using namespace std;
// ------------------------------------------------------------------ helpers

void Matrix::allocate(int rows, int cols) {
    data = new double* [rows];
    for (int i = 0; i < rows; i++) {
        data[i] = new double[cols];
        for (int j = 0; j < cols; j++)
            data[i][j] = 0.0;
    }
}

void Matrix::deallocate() {
    for (int i = 0; i < m; i++)
        delete[] data[i];
    delete[] data;
}

// ------------------------------------------------------------------ constructors

// Identity matrix: ones on the main diagonal, zeros elsewhere

Matrix::Matrix(int n) : m(n), n(n) {
    allocate(n, n);
    for (int i = 0; i < n; i++)
        data[i][i] = 1.0;
}

Matrix::Matrix(int m, int n, double fill_value) : m(m), n(n) {
    allocate(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            data[i][j] = fill_value;
}

// Copy constructor: deep copy - allocate fresh memory and copy all elements

Matrix::Matrix(const Matrix& other) : m(other.m), n(other.n) {
    allocate(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            data[i][j] = other.data[i][j];
}

// ------------------------------------------------------------------ destructor

Matrix::~Matrix() {
    deallocate();
}

// ------------------------------------------------------------------ assignment operator
// Needed because we own heap memory. Without it the default (shallow) copy
// would cause double-free when both objects are destroyed.

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) return *this;   // self-assignment guard
    deallocate();                        // release old memory first
    m = other.m;
    n = other.n;
    allocate(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            data[i][j] = other.data[i][j];
    return *this;
}

// ------------------------------------------------------------------ element access

double Matrix::get(int i, int j) const { return data[i][j]; }
void   Matrix::set(int i, int j, double value) { data[i][j] = value; }

int Matrix::get_height() const { return m; }
int Matrix::get_width()  const { return n; }

// ------------------------------------------------------------------ operations

void Matrix::negate() {
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            data[i][j] = -data[i][j];
}

void Matrix::add_in_place(Matrix& other) {
    if (m != other.m || n != other.n)
        throw runtime_error("Matrix::add_in_place: incompatible sizes");
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            data[i][j] += other.data[i][j];
}

// Returns a new matrix = this * other

Matrix Matrix::multiply(Matrix& other) {
    if (n != other.m)
        throw runtime_error("Matrix::multiply: incompatible sizes");

    Matrix result(m, other.n, 0.0);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < other.n; j++)
            for (int k = 0; k < n; k++)
                result.data[i][j] += data[i][k] * other.data[k][j];
    return result;
}