#include "matrix.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string filename;
    int n;
    Matrix matrix1;
    Matrix matrix2;
    // Problem 1
    cout << "Enter input file name: ";
    cin >> filename;
    if (!loadMatrices(filename, n, matrix1, matrix2))
    {
        cout << "Unable to open or read the file." << endl;
        return 1;
    }
    cout << "\nMatrix 1:" << endl;
    printMatrix(matrix1);
    cout << "\nMatrix 2:" << endl;
    printMatrix(matrix2);
    // Problem 2
    cout << "\nMatrix Addition:" << endl;
    Matrix sum = addMatrices(matrix1, matrix2);
    printMatrix(sum);
    //PROBLEM 3
    cout << "\nMatrix Multiplication:" << endl;
    Matrix product = multiplyMatrices(matrix1, matrix2);
    printMatrix(product);
    // Problem 4
    cout << "\nDiagonal Sums of Matrix 1:" << endl;
    diagonalSums(matrix1);
    // Problem 5
    int row1;
    int row2;
    cout << "\nEnter two row indices to swap: ";
    cin >> row1 >> row2;
    swapRows(matrix1, row1, row2);
    cout << "\nMatrix 1 after row swap:" << endl;
    printMatrix(matrix1);
    // Problem 6
    int col1;
    int col2;
    cout << "\nEnter two column indices to swap: ";
    cin >> col1 >> col2;
    swapColumns(matrix1, col1, col2);
    cout << "\nMatrix 1 after column swap:" << endl;
    printMatrix(matrix1);
    // Problem 7
    int row;
    int col;
    int value;
    cout << "\nEnter row, column, and new value: ";
    cin >> row >> col >> value;
    updateElement(matrix1, row, col, value);
    cout << "\nMatrix 1 after updating element:" << endl;
    printMatrix(matrix1);
    return 0;
}
