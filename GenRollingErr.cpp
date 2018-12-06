#include "GenRollingErr.h"

mat genRollingErr (vec rt, vec x, int ini_win, double propsigma_beta, int flag_bma)
{
    int S = rt.n_elem - ini_win;

    //printf("genRollingErr p0\n");fflush(stdout);
    vec en(S+1); en.fill(0); en.fill(NAN);                                  //the rolling OOS errors from the historical mean
    vec ea(S+1); ea.fill(0); ea.fill(NAN);                                  //the rolling OOS errors from the ols
    vec ew(S+1); ew.fill(0); ew.fill(NAN);                                  //the rolling OOS errors from the equal weights
    vec eb(S+1); eb.fill(0); eb.fill(NAN);                                  //the rolling OOS errors from the bma

    if (flag_bma == 1)
        for (int s = 0; s <= S; s++)
        {
            //printf("genRollingErr 1 p1 %d\n", s);fflush(stdout);
            int Ts = ini_win + s;                                           //time for the 1-step forecast in rolling s
            mat Y = join_horiz(rt.subvec(0, Ts - 2), x.subvec(1, Ts - 1));
            mat X = join_horiz(ones<vec>(Ts - 1), x.subvec(0, Ts - 2));
            
            //printf("genRollingErr 1 p2 %d\n", s);fflush(stdout);
            double w0 = genBmaWeights(Y, X, propsigma_beta);              //bma weights
            double w1 = 1.0 - w0;
            //printf("genRollingErr 1 p3 %d\n", s);fflush(stdout);
            
            double f0 = mean(rt.subvec(0, Ts - 2));                         //forecast from the historical mean
            vec b = regress(rt.subvec(0, Ts - 2), X, 1);
            double f1 = b(0) + x(Ts - 1) * b(1);                            //forecast from the ols
            double f_ew = 0.5 * f0 + 0.5 * f1;                              //forecast from the naive forecast combination

            double f_bma = w0 * f0 + w1 * f1;                               //forecast from the bma
            //index s is due to s starting from 0
            en(s) = rt(Ts-1) - f0;                                          // OOS errors from the historical mean
            ea(s) = rt(Ts-1) - f1;                                          // OOS errors from the ols
            ew(s) = rt(Ts-1) - f_ew;                                        // OOS errors from the naive forecast combination
            eb(s) = rt(Ts-1) - f_bma;                                       // OOS errors from the bma
            //printf("genRollingErr 1 p4 %d\n", s);fflush(stdout);
            if (s % 10 == 0 || s == S)
                printf("genRollingErr_1 %d/%d\n", s, S);
        }
    else
        for (int s = 0; s <= S; s++)
        {
            //printf("genRollingErr 0 p1 %d\n", s);fflush(stdout);
            int Ts = ini_win + s;                                           //time for the 1-step forecast in rolling s
            mat Y = join_horiz(rt.subvec(0, Ts - 2), x.subvec(1, Ts - 1));
            mat X = join_horiz(ones<vec>(Ts - 1), x.subvec(0, Ts - 2));
            //printf("genRollingErr 0 p2 %d\n", s);fflush(stdout);
            
            //double w0 = genBmaWeights(Y, X, propsigma_beta);              //bma weights
            //double w1 = 1.0 - w0;
            
            double f0 = mean(rt.subvec(0, Ts - 2));                         //forecast from the historical mean
            vec b = regress(rt.subvec(0, Ts - 2), X, 1);
            double f1 = b(0) + x(Ts - 1) * b(1);                            //forecast from the ols
            double f_ew = 0.5 * f0 + 0.5 * f1;                              //forecast from the naive forecast combination

            //double f_bma = w0 * f0 + w1 * f1;                               //forecast from the bma
            //index s is due to s starting from 0
            en(s) = rt(Ts-1) - f0;                                          // OOS errors from the historical mean
            ea(s) = rt(Ts-1) - f1;                                          // OOS errors from the ols
            ew(s) = rt(Ts-1) - f_ew;                                        // OOS errors from the naive forecast combination
            //eb(s) = rt(Ts-1) - f_bma;                                       // OOS errors from the bma
            //printf("genRollingErr 0 p3 %d\n", s);fflush(stdout);
            //if (s % 10 == 1 || s == S)
            //    printf("genRollingErr_1 %d/%d\n", s, S);
        }

    mat result(S+1, 4);
    result.col(0) = en;
    result.col(1) = ea;
    result.col(2) = ew;
    result.col(3) = eb;

    return result;
}

