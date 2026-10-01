#include "matrix.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

// Problem 1
bool loadMatrices(const string& filename, int& n,
                  Matrix& matrix1, Matrix& matrix2)
{
    ifstream file(filename);
    if(!file)
    {
        return false;
    }
    file >> n;
    if (n <= 0)
    {
        return false;
    }
    matrix1.resize(n, vector<int>(n));
    matrix2.resize(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            file >> matrix1[i][j];
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            file >> matrix2[i][j];
        }
    }
    file.close();
    return true;
}
void printMatrix(const Matrix& matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
}
// Problem 2
Matrix addMatrices(const Matrix& matrix1, const Matrix& matrix2)
{
    int n = matrix1.size();
    Matrix result(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return result;
}


// Problem 3
Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2)
{
    int n = matrix1.size();
    Matrix result(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}
// Problem 4
void diagonalSums(const Matrix& matrix)
{
    int n = matrix.size();
    int mainDiagonalSum = 0;
    int secondaryDiagonalSum = 0;
    for (int i = 0; i < n; i++)
    {
        mainDiagonalSum += matrix[i][i];
        secondaryDiagonalSum += matrix[i][n - 1 - i];
    }
    cout << "Main diagonal sum: "
         << mainDiagonalSum << endl;
    cout << "Secondary diagonal sum: "
         << secondaryDiagonalSum << endl;
}
// Problem 5
void swapRows(Matrix& matrix, int row1, int row2)
{
    int n = matrix.size();
    if (row1 < 0 || row1 >= n ||
        row2 < 0 || row2 >= n)
    {
        cout << "Invalid row index." << endl;
        return;
    }
    swap(matrix[row1], matrix[row2]);
}
// Problem 6
void swapColumns(Matrix& matrix, int col1, int col2)
{
    int n = matrix.size();
    if (col1 < 0 || col1 >= n ||
        col2 < 0 || col2 >= n)
    {
        cout << "Invalid column index." << endl;
        return;
    }
    for (int i = 0; i < n; i++)
    {
        swap(matrix[i][col1], matrix[i][col2]);
    }
}
// Problem 7
void updateElement(Matrix& matrix, int row, int col, int value)
{
    int n = matrix.size();
    if (row < 0 || row >= n ||
        col < 0 || col >= n)
    {
        cout << "Invalid matrix index." << endl;
        return;
    }
    matrix[row][col] = value;
}
