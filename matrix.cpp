#include "matrix.h"
#include <iostream>

using namespace std;

void invalid(Matrix a, Matrix b)
{
    printf("invalid matrix size: (%d, %d) (%d, %d)\n", a.row, a.col, b.row, b.col);
    exit(0);
}

void Matrix::set(const Matrix& a)
{
    if (m != 0)  dealloc();
    row = a.row; col = a.col;
    build();
    for (int i = 0; i <= row; i++)
        for (int j = 0; j <= col; j++)
            m[i][j] = a.m[i][j];
}

Matrix::Matrix(const Matrix& a)
{
    set(a);
}

Matrix& Matrix::operator =(const Matrix& a)
{
    if (this != &a) set(a);
    return *this;
}

void Matrix::set_zeros()
{
    for (int i = 0; i <= row; i++)
        for (int j = 0; j <= col; j++)
            m[i][j] = 0.0;
}

void Matrix::set_ones()
{
    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++)
            m[i][j] = 1.0;
}

Matrix Matrix::zeros(int _row, int _col)
{
    row = _row; col = _col;
    build();
    set_zeros();
    return *this;
}

Matrix Matrix::ones(int _row, int _col)
{
    row = _row; col = _col;
    build();
    set_ones();
    return *this;
}

Matrix Matrix::mul(Matrix a)
{
    if (col != a.row)   invalid(*this, a);
    Matrix b = Matrix(row, a.col);
    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++)
            for (int k = 1; k <= a.col; k++)
                b.m[i][j] += m[i][j] * a.m[i][j];
    return b;
}

Matrix Matrix::add(Matrix a)
{
    if (col != a.col || row != a.row)   invalid(*this, a);
    Matrix b = Matrix(row, col);
    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++)
            b.m[i][j] = m[i][j] + a.m[i][j];
    return b;
}

Matrix Matrix::sub(Matrix a)
{
    if (col != a.col || row != a.row)   invalid(*this, a);
    Matrix b = Matrix(row, col);
    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++)
            b.m[i][j] = m[i][j] - a.m[i][j];
    return b;
}

Matrix Matrix::mul_samepos(Matrix a)
{
    if (col != a.col || row != a.row)   invalid(*this, a);
    Matrix b = Matrix(row, col);
    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++)
            b.m[i][j] = m[i][j] * a.m[i][j];
    return b;
}