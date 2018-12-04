#ifndef GEN_ROLLING_ERR_H
#define GEN_ROLLING_ERR_H

#include <armadillo>
#include "Const.h"
#include "GenBmaWeights.h"

using namespace arma;

mat genRollingErr (vec rt, vec x, int ini_win, double propsigma_beta, int flag_bma);

#endif  //GEN_ROLLING_ERR_H