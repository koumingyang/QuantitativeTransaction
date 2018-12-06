#ifndef BOOTFUNC_H
#define BOOTFUNC_H

#include <armadillo>
#include <cstdlib>
#include "GenRollingErr.h"
#include "Const.h"

using namespace arma;

/*
    function for randomly bootstrap sample
    input:
        mat t(m, n)
    output:
        mat ans(m, n)
*/
mat bootstrapSample(mat t);

/*
    function for prctile
    input:
        vec t(n)
        x
    output:
        d
        in vec t, x% of elements < d
*/
double prctile(vec t, double x);

/*
    function used in bootstrap to generate MSE-F statistics

    INPUTS:
        resid: resid for bootstrap
        rt_fit: fitted rt
        x_fit: fitted x
        ini_win: initial window for the training sample
        x: predictor, use to generate initial observation
        propsigma_beta: hyperparameter sigma_beta used in the bma

    OUTPUTS:
        msef_boot_ea: the bootstrapped MSE-F statistics from the ols
        msef_boot_ew: the bootstrapped MSE-F statistics from equal weight
*/
vec bootFunc(mat resid, vec rt_fit, vec x_fit, int ini_win, vec x, double propsigma_beta);

#endif  //BOOTFUNC_H
