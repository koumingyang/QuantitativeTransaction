#ifndef REGRESS_H
#define REGRESS_H

#include <cmath>
#include <armadillo>
#include "Const.h"

using namespace arma;

/*
    regression (times = 1: linear regression)
    input: 
        x: vec (m)
        y: mat (m, 2)
            = [[y1, 1], [y2, 1], ..., [ym, 1]]      (c style)
            = [ones(m), y(1:m)]                     (matlab style)
        times : times of output data
                times = 1: linear regression
    output:
        c: vec(times + 1)
        y = c[0] + c[1] * x + c[2] * x ^ 2 + ... + c[times] * x ^ times
*/
vec regress(vec x, mat y, int times);

#endif  //REGRESS_H
