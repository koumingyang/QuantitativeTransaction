#ifndef BOOTFUNC_BMA_H
#define BOOTFUNC_BMA_H

#include <armadillo>
#include <cstdlib>
#include "GenRollingErr.h"
#include "Const.h"

using namespace arma;

/*
    %% function used in bootstrap to generate MSE-F statistics including the one from the bma
    function [msef_boot] = bootfun_bma(resid, rt_fit, x_fit, ini_win, x,propsigma_beta)

    % INPUTS:
    %       resid: resid for bootstrap
    %       rt_fit: fitted rt
    %       x_fit: fitted x
    %       ini_win: initial window for the training sample
    %       x: predictor, use to generate initial observation
    %       propsigma_beta: hyperparameter sigma_beta used in the bma

    % OUTPUTS:
    %       msef_boot_ea: the bootstrapped MSE-F statistics from the ols
    %       msef_boot_ew: the bootstrapped MSE-F statistics from equal weight
    %       msef_boot_eb: the bootstrapped MSE-F statistics from the bma
*/

vec bootFuncBMA(mat resid, vec rt_fit, vec x_fit, int ini_win, vec x, double propsigma_beta);

#endif  //BOOTFUNC_BMA_H