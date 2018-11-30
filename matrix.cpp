#include "Matrix.h"
#include <iostream>

using namespace std;

void invalid(Matrix a, Matrix b)
{
    printf("invalid matrix size: (%d, %d) (%d, %d)\n", a.row, a.col, b.row, b.col);
    exit(0);
}

void invalid_inverse(Matrix a)
{
    printf("invalid matrix size for inverse: (%d, %d)\n", a.row, a.col);
    exit(0);
}

void Matrix::set(const Matrix& a)
{
    dealloc();
    row = a.row; col = a.col;
    build();
    for (int i = 0; i <= row; i++)
        for (int j = 0; j <= col; j++)
            m[i][j] = a.m[i][j];
}

Matrix::Matrix(const Matrix& a)
{
    m = 0;
    set(a);
}

Matrix& Matrix::operator =(const Matrix& a)
{
    if (this != &a) set(a);
    return *this;
}

void Matrix::set_zeros()
{
    memset(m, 0, sizeof(m));
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

Matrix Matrix::inv_mul(Matrix a)
{
    Matrix inv = this->inverse();
    return inv.mul(a);
}

Matrix Matrix::transpose()
{
    Matrix a = Matrix(col, row);
    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++)
            a.m[j][i] = m[i][j];
    return a;
}

Matrix Matrix::inverse()
{
    if (col != row) invalid_inverse(*this);
    int i, j, k, p, n = row;
    double tmp, sum;
    Matrix t(row, col), inva(row, col);
    t.set(*this); inva.set_zeros();
    for (i = 1; i <= n; i++)
        inva.m[i][i] = 1.0;

    for (i = 1; i < n; i++)
    {
        tmp = 0;
		for(j = i; j <= n; j++)
			if (fabs(t.m[i][j]) > tmp)
				tmp = fabs(t.m[i][j]), p = j;
        if(p != i)
			for(j = 1; j <= n; j++)
				swap(t.m[i][j], t.m[p][j]), swap(inva.m[i][j], inva.m[p][j]);
        for(k = i+1; k <= n; k++)
		{
			tmp = t.m[k][i] / t.m[i][i];
			for(j = 1; j <= n; j++)
				t.m[k][j] -= tmp * t.m[i][j], inva.m[k][j] -= tmp * inva.m[i][j];
		}
    }

    for (i = 1; i <= n; i++)
    {
        for (j = i+1; j <= n; j++)
            if (t.m[i][j] != 0)
            {
                tmp = t.m[i][j] / t.m[j][j];
                for (k = 1; k <= n; k++)
                    t.m[i][k] -= tmp * t.m[j][k], inva.m[i][k] -= tmp * inva.m[j][k];
            }
        tmp = 1.0 / t.m[i][i];
        for (j = 1; j <= n; j++)
            t.m[i][j] *= tmp, inva.m[i][j] *= tmp;
    }
    return inva;
}

Matrix Matrix::cov()
{
    Vec avg(col);
    Matrix t(row, col), ans(row, col); 
    int i, j, k;
    for (j = 1; j <= col; j++)
    {
        avg.v[j] = 0;
        for (i = 1; i <= row; i++)
            avg.v[j] += m[i][j];
        avg.v[j] /= (double)(row);
    }
    for (i = 1; i <= row; i++)
        for (j = 1; j <= col; j++)
            t.m[i][j] = m[i][j] - avg.v[j];
    for (i = 1; i <= col; i++)
        for (j = 1; j <= col; j++)
        {
            ans.m[i][j] = 0;
            for (k = 1; k <= row; k++)
                ans.m[i][j] += t.m[k][i] * t.m[k][j];
            ans.m[i][j] /= (double)(row - 1);
        }
    return ans;
}

Vec Matrix::col_x(int x)
{
    Vec p(row);
    for (int i = 1; i <= row; i++)
        p.v[i] = m[i][x];
    return p;
}

Vec Matrix::row_x(int x)
{
    Vec p(col);
    for (int i = 1; i <= col; i++)
        p.v[i] = m[x][i];
    return p;
}

Matrix mul(Matrix a, Matrix b)  {return a.mul(b);}
Matrix add(Matrix a, Matrix b)  {return a.add(b);}
Matrix sub(Matrix a, Matrix b)  {return a.sub(b);}
Matrix mul_samepos(Matrix a, Matrix b)  {return a.mul_samepos(b);}
Matrix inv_mul(Matrix a, Matrix b)  {return a.inv_mul(b);}
