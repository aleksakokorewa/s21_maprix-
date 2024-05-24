#include "s21_matrix_oop.h"

// constructors
S21Matrix::S21Matrix() : S21Matrix(3, 3) {}

// constructor with parameters;
S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows < 1 || cols < 1)
    throw std::invalid_argument(
        "CreationError: Matrix sizes should be equal 1 or greater");  // exeption
  else {
    s21_createMatrix_(rows_, cols_);
  }
}

// copy constructor
S21Matrix::S21Matrix(const S21Matrix& other)
    : S21Matrix(other.rows_, other.cols_) {
  for (int i = 0; i < other.rows_; ++i) {
    for (int k = 0; k < other.cols_; k++) {
      matrix_[i][k] = other.matrix_[i][k];
    }
  }
}

// move constructor
S21Matrix::S21Matrix(S21Matrix&& other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

// destructor
S21Matrix::~S21Matrix() { s21_clear_matrix_(); }

// getters
int S21Matrix::GetRows() const { return rows_; }
int S21Matrix::GetCols() const { return cols_; }

// setters
void S21Matrix::SetCols(int cols) {
  if (cols < 1)
    throw std::out_of_range("Cols of matrix should be more or equal 1");
  if (cols != cols_) {
    S21Matrix result(rows_, cols);
    for (int i = 0; i < rows_; ++i)
      for (int k = 0; k < (cols_ < cols ? cols_ : cols); ++k)
        result.matrix_[i][k] = matrix_[i][k];
    *this = std::move(result);
  }
}

void S21Matrix::SetRows(int rows) {
  if (rows < 1)
    throw std::out_of_range("Rows of matrix should be more or equal 1");
  if (rows != rows_) {
    S21Matrix result(rows, cols_);
    for (int i = 0; i < (rows_ < rows ? rows_ : rows); ++i)
      for (int k = 0; k < cols_; ++k) result.matrix_[i][k] = matrix_[i][k];
    *this = std::move(result);
  }
}

// methods
bool S21Matrix::EqMatrix(const S21Matrix& other) const noexcept {
  bool is_eq = true;
  if (rows_ != other.rows_ || cols_ != other.cols_)
    is_eq = false;
  else {
    for (int i = 0; i < rows_; ++i) {
      for (int k = 0; k < cols_; ++k) {
        if (fabs(matrix_[i][k] - other.matrix_[i][k]) > EPS) is_eq = false;
      }
    }
  }
  return is_eq;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::out_of_range(
        "Incorrect input, matrices should have the same size");
  else {
    for (int i = 0; i < rows_; ++i)
      for (int k = 0; k < cols_; ++k) matrix_[i][k] += other.matrix_[i][k];
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::out_of_range(
        "Incorrect input, matrices should have the same size");
  else {
    for (int i = 0; i < rows_; ++i)
      for (int k = 0; k < cols_; ++k) matrix_[i][k] -= other.matrix_[i][k];
  }
}

void S21Matrix::MulNumber(const double num) noexcept {
  for (int i = 0; i < rows_; i++)
    for (int k = 0; k < cols_; ++k) matrix_[i][k] *= num;
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix transposed(cols_, rows_);
  for (int i = 0; i < rows_; ++i)
    for (int k = 0; k < cols_; ++k) transposed.matrix_[k][i] = matrix_[i][k];
  return transposed;
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_)
    throw std::out_of_range(
        "Columns of matrix 1 should be the same size as rows of matrix 2");
  S21Matrix result(rows_, other.cols_);
  for (int i = 0; i < result.rows_; ++i)
    for (int k = 0; k < result.cols_; ++k)
      for (int p = 0; p < cols_; ++p)
        result.matrix_[i][k] += matrix_[i][p] * other.matrix_[p][k];
  *this = result;
}

// operators
double& S21Matrix::operator()(int i, int j) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_)
    throw std::out_of_range(
        "Arguments i(j) should be more or equal then 0 and less then"
        "rows_(cols_)");
  else
    return matrix_[i][j];
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this != &other) {
    s21_clear_matrix_();
    s21_createMatrix_(other.rows_, other.cols_);
    for (int i = 0; i < other.rows_; ++i)
      for (int k = 0; k < other.cols_; ++k) matrix_[i][k] = other.matrix_[i][k];
  }
  return *this;
}

bool S21Matrix::operator==(const S21Matrix& other) { return EqMatrix(other); }

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(const double num) {
  S21Matrix result(*this);
  result.MulNumber(num);
  return result;
}

S21Matrix operator*(double num, S21Matrix& matrix) { return matrix * num; }

double S21Matrix::Determinant() const {
  if (cols_ != rows_)
    throw std::out_of_range("Exception: The matrix is not square");
  double det = 0;
  if (rows_ == 1) {
    det = matrix_[0][0];
  } else if (rows_ == 2) {
    det = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  } else {
    for (int i = 0; i < rows_; i++) {
      S21Matrix M(s21_minor_(1, i + 1));
      det += pow((-1), i) * matrix_[0][i] * M.Determinant();
    }
  }
  return det;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_)
    throw std::out_of_range("CalcComplementsError: The matrix must be square");

  S21Matrix result(rows_, cols_);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) {
      S21Matrix M(s21_minor_(i + 1, j + 1));
      result.matrix_[i][j] = M.Determinant() * pow(-1, i + j);
    }

  return result;
}

S21Matrix S21Matrix::InverseMatrix() const {
  double det = Determinant();
  if (det == 0) throw std::out_of_range("InverseError: Determinant is 0");

  S21Matrix result(rows_, cols_);
  result = CalcComplements();
  result = result.Transpose();
  return result * (1.0 / det);
}

void S21Matrix::s21_clear_matrix_() noexcept {
  if (matrix_) {
    for (int i = 0; i < rows_; ++i) delete[] matrix_[i];  // clear memory
  }
  delete[] matrix_;
}

void S21Matrix::s21_createMatrix_(int rows, int cols) {
  rows_ = rows;
  cols_ = cols;
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_];
    for (int j = 0; j < cols_; ++j) matrix_[i][j] = 0;
  }
}

S21Matrix S21Matrix::s21_minor_(int rows, int cols) const {
  S21Matrix M(rows_ - 1, cols_ - 1);
  int m = 0, n = 0;

  for (int i = 0; i < rows_; i++) {
    m = i;
    if (i > rows - 1) {
      m--;
    }
    for (int j = 0; j < cols_; j++) {
      n = j;
      if (j > cols - 1) {
        n--;
      }
      if (i != rows - 1 && j != cols - 1) {
        M.matrix_[m][n] = matrix_[i][j];
      }
    }
  }
  return M;
}