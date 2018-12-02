#include "Cube.h"
#include <iostream>

using namespace std;

void invalid(const Cube &a, const Cube &b)
{
    printf("invalid Cube size: (%d, %d, %d) (%d, %d, %d)\n", a.a, a.b, a.c, b.a, b.b, b.c);
    exit(0);
}
void invalid_set_matab(const Cube &a, const Matrix &b)
{
    printf("invalid set matab: (%d, %d, %d) (%d, %d)\n", a.a, a.b, a.c, b.row, b.col);
    exit(0);
}

void Cube::set(const Cube& t)
{
    dealloc();
    a = t.a; b = t.b; c = t.c;
    build();
    for (int i = 0; i <= a; i++)
        for (int j = 0; j <= b; j++)
            for (int k = 0; k <= c; k++)
                data[i][j] = t.data[i][j];
}

void Cube::set_matab(int x, const Matrix& t)
{
    if (a < t.row || b < t.col) invalid_set_matab(*this, t);
    for (int i = 0; i <= t.row; i++)
        for (int j = 0; j <= t.col; j++)
            data[i][j][x] = t.m[i][j];
}

Cube::Cube(const Cube& t)
{
    data = 0;
    set(t);
}

Cube& Cube::operator =(const Cube& t)
{
    if (this != &t) set(t);
    return *this;
}

void Cube::set_zeros()
{
    for (int i = 0; i <= a; i++)
        for (int j = 0; j <= b; j++)
            for (int k = 0; k <= c; k++)
                data[i][j][k] = 0.0;
}

void Cube::set_ones()
{
    for (int i = 1; i <= a; i++)
        for (int j = 1; j <= b; j++)
            for (int k = 1; k <= c; k++)
                data[i][j][k] = 1.0;
}

Cube Cube::zeros(int _a, int _b, int _c)
{
    a = _a; b = _b; c = _c;
    build();
    set_zeros();
    return *this;
}

Cube Cube::ones(int _a, int _b, int _c)
{
    a = _a; b = _b; c = _c;
    build();
    set_ones();
    return *this;
}

Cube Cube::add(Cube t)
{
    if (a != t.a || b != t.b || c != t.c)   invalid(*this, t);
    Cube p = Cube(a, b, c);
    for (int i = 1; i <= a; i++)
        for (int j = 1; j <= b; j++)
            for (int k = 1; k <= c; k++)
                p.data[i][j][k] = data[i][j][k] + t.data[i][j][k];
    return p;
}

Cube Cube::sub(Cube t)
{
    if (a != t.a || b != t.b || c != t.c)   invalid(*this, t);
    Cube p = Cube(a, b, c);
    for (int i = 1; i <= a; i++)
        for (int j = 1; j <= b; j++)
            for (int k = 1; k <= c; k++)
                p.data[i][j][k] = data[i][j][k] - t.data[i][j][k];
    return p;
}

Cube Cube::mul_samepos(Cube t)
{
    if (a != t.a || b != t.b || c != t.c)   invalid(*this, t);
    Cube p = Cube(a, b, c);
    for (int i = 1; i <= a; i++)
        for (int j = 1; j <= b; j++)
            for (int k = 1; k <= c; k++)
                p.data[i][j][k] = data[i][j][k] * t.data[i][j][k];
    return p;
}

Cube add(Cube a, Cube b)  {return a.add(b);}
Cube sub(Cube a, Cube b)  {return a.sub(b);}
Cube mul_samepos(Cube a, Cube b)  {return a.mul_samepos(b);}
