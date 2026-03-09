#pragma once
#include <stdexcept>

class Matrix {
private:
    int m;
    int n;
    double** data;
    void allocate(int rows, int cols);
    void deallocate();

public:
    // ── constructors / destructor ─────────────────────────────────────────
    Matrix(int n);
    Matrix(int m, int n, double fill_value = 0);
    Matrix(const Matrix& other);           // copy constructor
    Matrix(Matrix&& other);                // move constructor  (Task 1.3)
    ~Matrix();

    // ── assignment operators ──────────────────────────────────────────────
    Matrix& operator=(const Matrix& other); // copy assign  (Task 1.1)
    Matrix& operator=(Matrix&& other);      // move assign  (Task 1.3)

    // ── compound assignment operators ─────────────────────────────────────
    Matrix& operator+=(const Matrix& other); // Task 1.2.1
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(double scalar);
    Matrix& operator/=(double scalar);

    // ── binary arithmetic operators ───────────────────────────────────────
    Matrix operator+(const Matrix& other) const; // Task 1.2.2
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar)       const;
    Matrix operator/(double scalar)       const;

    // ── unary minus ───────────────────────────────────────────────────────
    // Task 1.2.3: global function declared friend
    friend Matrix operator-(const Matrix& m);

    // ── scalar * matrix  (global, Task 1.2.3) ────────────────────────────
    friend Matrix operator*(double scalar, const Matrix& m);

    // ── existing methods (kept for compatibility) ─────────────────────────
    double get(int i, int j) const;
    void   set(int i, int j, double value);
    int get_height() const;
    int get_width()  const;
    void negate();
    void add_in_place(const Matrix& other);
    Matrix multiply(const Matrix& other);
};
