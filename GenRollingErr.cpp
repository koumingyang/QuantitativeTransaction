#include "GenRollingErr.h"

Matrix genRollingErr(Vec rt, Vec x, int ini_win, double propsigma_beta, int flag_bma)
{
    int S = rt.size - ini_win;

    Vec en(S+1, NAN);   //the rolling OOS errors from the historical mean
    Vec ea(S+1, NAN);   //the rolling OOS errors from the ols
    Vec ew(S+1, NAN);   //the rolling OOS errors from the equal weights
    Vec eb(S+1, NAN);   //the rolling OOS errors from the bma

    if (flag_bma == 1)
    {
        int s, i;
        for (s = 0; s <= S; s++)
        {
            int Ts = ini_win + s;   //time for the 1-step forecast in rolling s
            Matrix Y(Ts - 1, 2);
            Matrix X(Ts - 1, 2);
            for (i = 1; i <= Ts - 1; i++)
            {
                Y.m[i][1] = rt.v[i];
                Y.m[i][2] = x.v[i+1];
                X.m[i][1] = 1.0;
                X.m[i][2] = x.v[i];
            }
        }
    }
    
if flag_bma==1

    for s=0:S   

        Ts = ini_win + s; % time for the 1-step forecast in rolling s

        Y = [rt(1:Ts-1),x(2:Ts)]; 
        X = [ones(Ts-1,1),x(1:Ts-1)];

        [w0,w1] = gen_bma_weights(Y,X,propsigma_beta); % bma weights

        f0 = mean(rt(1:Ts-1)); % forecast from the historical mean

        b = regress(rt(1:Ts-1), X);
        f1 = [1, x(Ts)]*b; % forecast from the ols

        f_ew = 0.5*f0 + 0.5*f1; % forecast from the naive forecast combination

        f_bma = w0*f0 + w1*f1; % forecast from the bma

        % index s+1 is due to s starting from 0
        en(s+1) = rt(Ts) - f0; % OOS errors from the historical mean
        ea(s+1) = rt(Ts) - f1; % OOS errors from the ols
        ew(s+1) = rt(Ts) - f_ew; % OOS errors from the naive forecast combination
        eb(s+1) = rt(Ts) - f_bma; % OOS errors from the bma
    end 
    
}

