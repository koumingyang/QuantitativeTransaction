#include "Vec.h"
#include <iostream>
#include <cmath>

using namespace std;

void invalid(Vec a, Vec b)
{
    printf("invalid vector size: (%d) (%d)\n", a.size, b.size);
    exit(0);
}

void Vec::set(const Vec& a)
{
    dealloc();
    size = a.size;
    build();
    for (int i = 0; i <= size; i++)
        v[i] = a.v[i];
}

Vec::Vec(const Vec& a)
{
    v = 0;
    set(a);
}

Vec& Vec::operator =(const Vec& a)
{
    if (this != &a) set(a);
    return *this;
}

void Vec::set_zeros()
{
    for (int i = 0; i <= size; i++)
        v[i] = 0;
}

void Vec::set_ones()
{
    for (int i = 0; i <= size; i++)
        v[i] = 1.0;
}

Vec Vec::sub_vec(int st, int fi)
{
    Vec a = Vec(fi - st + 1);
    for (int i = st; i <= fi; i++)
        a.v[i] = v[i - st + 1];
    return a;
}

Vec Vec::zeros(int _size)
{
    size = _size;
    build();
    set_zeros();
    return *this;
}

Vec Vec::ones(int _size)
{
    size = _size;
    build();
    set_ones();
    return *this;
}

Vec Vec::log_all()
{
    Vec a = Vec(size);
    for (int i = 1; i <= size; i++)
        a.v[i] = log(v[i]);
    return a;
}

Vec Vec::add(Vec a)
{
    if (size != a.size)   invalid(*this, a);
    Vec b = Vec(size);
    for (int i = 1; i <= size; i++)
        b.v[i] = v[i] + a.v[i];
    return b;
}

Vec Vec::add(double a)
{
    Vec b = Vec(size);
    for (int i = 1; i <= size; i++)
        b.v[i] = v[i] + a;
    return b;
}

Vec Vec::sub(Vec a)
{
    if (size != a.size)   invalid(*this, a);
    Vec b = Vec(size);
    for (int i = 1; i <= size; i++)
        b.v[i] = v[i] - a.v[i];
    return b;
}

Vec Vec::mul(Vec a)
{
    if (size != a.size)   invalid(*this, a);
    Vec b = Vec(size);
    for (int i = 1; i <= size; i++)
        b.v[i] = v[i] * a.v[i];
    return b;
}

Vec Vec::div(Vec a)
{
    if (size != a.size)   invalid(*this, a);
    Vec b = Vec(size);
    for (int i = 1; i <= size; i++)
        b.v[i] = v[i] / a.v[i];
    return b;
}

Vec add(Vec a, double b)    {return a.add(b);}
Vec add(Vec a, Vec b)  {return a.add(b);}
Vec sub(Vec a, Vec b)  {return a.sub(b);}
Vec mul(Vec a, Vec b)  {return a.mul(b);}
Vec div(Vec a, Vec b)  {return a.div(b);}
