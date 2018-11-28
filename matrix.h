#ifndef MATRIX_H
#define MATRIX_H

#include "Const.h"
#include "Vec.h"

struct Matrix
{
    int row, col;
    double **m;
    void set_size(int _row, int _col)
    {
        dealloc();
        row = _row; col = _col;
        build();
    }
    Matrix()
    {
        m = 0;
        row = col = DEFAULT_MATRIX_SIZE;
        build();
    }
    Matrix(int _row, int _col)
    {
        m = 0;
        row = _row; col = _col;
        build();
    }
    Matrix(const Matrix& a);
    void build()
    {
        m = new double *[row + 1];
        for (int i = 0; i <= row; i++)
        {
            m[i] = new double [col + 1];
            for (int j = 0; j <= col; j++)
                m[i][j] = 0;
        }
    }
    void set_zeros();
    void set_ones();
    Matrix& operator =(const Matrix& a);
    Matrix zeros(int _row, int _col);
    Matrix ones(int _row, int _col);
    Matrix mul(Matrix a);
    Matrix add(Matrix a);
    Matrix sub(Matrix a);
    Matrix mul_samepos(Matrix a);
    void set(const Matrix& a);
    void dealloc()  {if (m != 0)  delete [] m;}
    Vec col_x(int x);
    Vec row_x(int x);
    ~Matrix()   {dealloc();}
};

Matrix mul(Matrix a, Matrix b);
Matrix add(Matrix a, Matrix b);
Matrix sub(Matrix a, Matrix b);
Matrix mul_samepos(Matrix a, Matrix b);

#endif  //MATRIX_H