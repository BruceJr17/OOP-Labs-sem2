#pragma once

// Task 2.2 - Matrix class (object as a mathematical entity).
//
// Stores a heap-allocated m x n matrix of doubles.
// Because this class owns heap memory it implements the Rule of Three:
//   - destructor
//   - copy constructor
//   - assignment operator

class Matrix {
private:
    int m;          // number of rows
    int n;          // number of columns
    double** data;  // 2-D array allocated on the heap

    void allocate(int rows, int cols);   // helper: allocate and zero-fill
    void deallocate();                   // helper: free all heap memory

public:
    // Construct an n x n identity matrix
    Matrix(int n);

    // Construct an m x n matrix filled with fill_value (default 0)
    Matrix(int m, int n, double fill_value = 0);

    // Copy constructor - performs a deep copy
    Matrix(const Matrix& other);

    // Assignment operator - needed because we own heap memory (Rule of Three)
    Matrix& operator=(const Matrix& other);

    ~Matrix();

    double get(int i, int j) const;
    void   set(int i, int j, double value);

    int get_height() const;  // number of rows
    int get_width()  const;  // number of columns

    void negate();                        // -A  (in place)
    void add_in_place(Matrix& other);     // this += other
    Matrix multiply(Matrix& other);       // returns this * other

    // Throws std::runtime_error if sizes are incompatible
};