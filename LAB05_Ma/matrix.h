#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <string>
using Matrix = std::vector<std::vector<int>>;
// Problem 1
bool loadMatrices(const std::string& filename, int& n,
                  Matrix& matrix1, Matrix& matrix2);
// Print a matrix
void printMatrix(const Matrix& matrix);
// Problem 2
Matrix addMatrices(const Matrix& matrix1, const Matrix& matrix2);
// Problem 3
Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2);
// Problem 4
void diagonalSums(const Matrix& matrix);
// Problem 5
void swapRows(Matrix& matrix, int row1, int row2);
// Problem 6
void swapColumns(Matrix& matrix, int col1, int col2);
// Problem 7
void updateElement(Matrix& matrix, int row, int col, int value);
#endif
