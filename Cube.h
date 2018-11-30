#ifndef CUBE_H
#define CUBE_H

#include "Const.h"
#include "Vec.h"
#include "Matrix.h"

struct Cube
{
    int a, b, c;
    double ***data;
    void set_size(int _a, int _b, int _c)
    {
        dealloc();
        a = _a; b = _b; c = _c;
        build();
    }
    Cube()
    {
        data = 0;
        a = b = c = 2;
        build();
    }
    Cube(int _a, int _b, int _c)
    {
        data = 0;
        a = _a; b = _b; c = _c;
        build();
    }
    Cube(const Cube& t);
    void build()
    {
        data = new double **[a + 1];
        for (int i = 0; i <= a; i++)
        {
            data[i] = new double *[b + 1];
            for (int j = 0; j <= b; j++)
            {
                data[i][j] = new double [c + 1];
                for (int k = 0; k <= c; k++)
                    data[i][j][k] = 0;
            }
        }
    }
    void set_zeros();
    void set_ones();
    Cube& operator =(const Cube& t);
    Cube zeros(int _a, int _b, int _c);
    Cube ones(int _a, int _b, int _c);
    Cube add(Cube t);
    Cube sub(Cube t);
    Cube mul_samepos(Cube t);
    void set(const Cube& t);
    void dealloc()  {if (data != 0)  delete [] data;}
    ~Cube()   {dealloc();}
};

Cube add(Cube a, Cube b);
Cube sub(Cube a, Cube b);
Cube mul_samepos(Cube a, Cube b);

#endif  //CUBE_H