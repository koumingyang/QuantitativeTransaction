#ifndef GEN_ROLLING_ERR_H
#define GEN_ROLLING_ERR_H

#include "Matrix.h"
#include "Vec.h"
#include "Const.h"
#include "GenBmaWeights.h"

Matrix genRollingErr (Vec rt, Vec x, double ini_win, double propsigma_beta, double flag_bma);

#endif  //GEN_ROLLING_ERR_H