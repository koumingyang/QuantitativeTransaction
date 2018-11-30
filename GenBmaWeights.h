#ifndef GEN_BMA_WEIGHTS_H
#define GEN_BMA_WEIGHTS_H

#include "Matrix.h"
#include "Vec.h"
#include "Const.h"
#include "Cube.h"
#include <cmath>

/*
    MH algorithm to calculate the BMA weights
    function to calculate the weights by Bayesian Model Average (BMA)

    INPUTS:
        Y: 
            Size: T * 2
            [r(1);...;r(T),x(1);...;x(T)] 
        X: 
            Size: T * 2
            [1;...;1, x(0),...,x(T-1)]
        r:
            double
            equity premium, x: predictor
        propsigma_beta: standard dev. of proposal distribution for beta

    OUTPUTS:
        W: [w0, w1]
        w0: the weight for model 0
        w1: the weight for model 1
*/

Matrix genBmaWeights (Matrix Y, Matrix X, double propsigma_beta);

#endif  //GEN_BMA_WEIGHTS_H