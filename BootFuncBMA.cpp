#include "BootFuncBMA.h"

vec bootFuncBMA(mat resid, vec rt_fit, vec x_fit, int ini_win, vec x, double propsigma_beta)
{
    vec rt = rt_fit + resid.col(0);     //rt constructed by the bootstrapped residuals
    int S = rt.n_elem - ini_win;        //the out-of-sample evaluation periods is S+1
    x = x_fit + resid.col(1);           //x(2:end) constructed by the bootstrapped residuals
    int idx = rand() % x.n_elem;
    vec x0(1);
    x0(0) = x(idx);                     //random draw to set the initial observation
    x = join_vert(x0, x);               //with selected initial observation
    //to calculate en, ea, ew, eb from the bootstrapped sample
    mat result = genRollingErr(rt, x, ini_win, propsigma_beta, 1);
    vec en = result.col(0);
    vec ea = result.col(1);
    vec ew = result.col(2);
    vec eb = result.col(3);

    //calculate the OOS statistics
    vec en_2 = pow(en, 2);
    vec ea_2 = pow(ea, 2);
    vec ew_2 = pow(ew, 2);
    vec eb_2 = pow(eb, 2);
    double mse_en = mean(en_2);
    double mse_ea = mean(ea_2);
    double mse_ew = mean(ew_2);
    double mse_eb = mean(eb_2);


    // the MSE-F statistic
    double msef_boot_ea = ((double)(S+1-1+1)) * (mse_en - mse_ea) / (mse_ea);
    double msef_boot_ew = ((double)(S+1-1+1)) * (mse_en - mse_ew) / (mse_ew);
    double msef_boot_eb = ((double)(S+1-1+1)) * (mse_en - mse_eb) / (mse_eb);

    vec res(3);
    res(0) = msef_boot_ea;
    res(1) = msef_boot_ew;
    res(2) = msef_boot_eb;

    return res;
}
