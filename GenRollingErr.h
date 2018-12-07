#ifndef GEN_ROLLING_ERR_H
#define GEN_ROLLING_ERR_H

#include <armadillo>
#include <iostream>
#include "Const.h"
#include "GenBmaWeights.h"
#include "Regress.h"

using namespace arma;
using namespace std;


/*
    function to calculate rolling OOS errors

    % INPUTS:
    %       rt: log equity premium
    %       x: predictor
    %       ini_win: initial window for the training sample
    %       propsigma_beta: hyperparameter sigma_beta used in the bma
    %       flag_bma: 1 -> generate rolling OOS error eb from the BMA 
    %                 0 -> Not to generate 

    % OUTPUTS:
            mat result (S+1, 4) = [en, ea, ew, eb]
    %       en: the rolling OOS errors from the historical mean
    %       ea: the rolling OOS errors from the ols
    %       ew: the rolling OOS errors from the equal weights
    %       eb: the rolling OOS errors from the bma
*/

mat genRollingErr (vec rt, vec x, int ini_win, double propsigma_beta, int flag_bma);
void output(ofstream &fout, mat m);

#endif  //GEN_ROLLING_ERR_H