#ifndef S21_MATRIX_OOP_H_
#define S21_MATRIX_OOP_H_
#define EPS 1e-7

#include <cmath>
#include <iostream>

class S21Matrix {
 private:
  // Attributes
  int rows_, cols_;  // Rows and columns
  double** matrix_;  // Pointer to the memory where the matrix is allocated
  void s21_clear_matrix_() noexcept;
  void s21_createMatrix_(int rows, int cols);
  S21Matrix s21_minor_(int rows, int cols) const;

 public:
  // constructors
  S21Matrix();                        // default constructor
  S21Matrix(int rows, int cols);      // constructor with parameters
  S21Matrix(const S21Matrix& other);  // copy constructor
  S21Matrix(S21Matrix&& other);       // move constructor
  // destructor
  ~S21Matrix();
  // methods
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num) noexcept;
  void MulMatrix(const S21Matrix& other);
  bool EqMatrix(const S21Matrix& other) const noexcept;
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;
  // getters
  int GetRows() const;
  int GetCols() const;
  // setters
  void SetRows(int rows);
  void SetCols(int cols);
  // operators
  double& operator()(int i, int j);
  S21Matrix& operator=(const S21Matrix& other);
  bool operator==(const S21Matrix& other);
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix operator*(const double num);
  friend S21Matrix operator*(double num, S21Matrix& matrix);
  S21Matrix& operator*=(const double num);
  S21Matrix& operator*=(const S21Matrix& other);
};

#endif  // S21_MATRIX_OOP_H_