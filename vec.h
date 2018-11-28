#ifndef VEC_H
#define VEC_H

#include "Const.h"

struct Vec
{
    int size;
    double *v;

    void set_size(int _size)
    {
        dealloc();
        size = _size;
        build();
    }
    void build()
    {
        v = new double [size + 1];
        set_zeros();
    }

    Vec()   {v = 0; set_size(DEFAULT_MATRIX_SIZE);}
    Vec(int _size)  {v = 0; set_size(_size);}
    Vec(const Vec& a);
    Vec& operator =(const Vec& a);
    
    void dealloc()  {if (v != 0)  delete [] v;}
    ~Vec()
    {
        dealloc();
    }
    
    void set_zeros();
    void set_ones();
    
    Vec zeros(int _size);
    Vec ones(int _size);
    Vec sub_vec(int st, int fi);
    Vec log_all();
    Vec add(double a);
    Vec add(Vec a);
    Vec sub(Vec a);
    Vec mul(Vec a);
    Vec div(Vec a);
    void set(const Vec& a);
};


Vec add(Vec a, double b);
Vec add(Vec a, Vec b);
Vec sub(Vec a, Vec b);
Vec mul(Vec a, Vec b);
Vec div(Vec a, Vec b);

#endif  //VEC_H