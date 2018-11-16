#ifndef VEC_H
#define VEC_H

#include "const.h"

struct Vec
{
    int size;
    double *v;

    void set_size(int _size)
    {
        size = _size;
        build();
    }
    void build()
    {
        v = new double [size + 1];
        set_zeros();
    }

    Vec()   {set_size(DEFAULT_MATRIX_SIZE);}
    Vec(int _size)  {set_size(_size);}
    Vec(const Vec& a);
    Vec& operator =(const Vec& a);
    
    void dealloc()  {delete [] v;}
    ~Vec()
    {
        delete [] v;
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

Vec add(Vec a, double b)    {return a.add(b);}
Vec add(Vec a, Vec b)  {return a.add(b);}
Vec sub(Vec a, Vec b)  {return a.sub(b);}
Vec mul(Vec a, Vec b)  {return a.mul(b);}
Vec div(Vec a, Vec b)  {return a.div(b);}

#endif  //VEC_H