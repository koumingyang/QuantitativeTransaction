#ifndef CONST_H
#define CONST_H

const int DEFAULT_MATRIX_SIZE = 1000;
const double NAN = 1e49;

//set hyperparameter sigma_beta to: 0.0001, 0.001, 0.01, 0.1, 1, 10
const double propsigma_beta = 0.01;

//CHANGE when consider begin forecast in 1965 or in 1976
const int ini_win = 20;

#endif  //CONST_H