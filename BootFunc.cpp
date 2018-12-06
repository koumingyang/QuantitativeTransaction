#include "BootFunc.h"

using namespace std;

mat bootstrapSample(mat t)
{
    int i, j, k, m = t.n_rows, n = t.n_cols;
    mat ans(m, n);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
        {
            k = rand() % m;
            ans(i, j) = t(k, j);
        }
    return ans;
}

double prctile(vec t, double x)
{
    vec a = sort(t);
    int idx = (int)((double)t.n_elem * x / 100.0);
    if (idx < t.n_elem - 1) idx ++;
    return t(idx); 
}

vec bootFunc(mat resid, vec rt_fit, vec x_fit, int ini_win, vec x, double propsigma_beta)
{
    //printf("bootFunc p1\n");fflush(stdout);
    vec rt = rt_fit + resid.col(0);     //rt constructed by the bootstrapped residuals
    int S = rt.n_elem - ini_win;        //the out-of-sample evaluation periods is S+1
    x = x_fit + resid.col(1);             //x(2:end) constructed by the bootstrapped residuals
    int idx = rand() % x.n_elem;
    //printf("bootFunc p1.1 %d %d\n", idx, x.n_elem);fflush(stdout);
    vec x0(1);
    x0(0) = x(idx);                     //random draw to set the initial observation
    //printf("bootFunc p1.2 %d %d\n", idx, x.n_elem);fflush(stdout);
    x = join_vert(x0, x);               //with selected initial observation
    
    //to calculate en, ea, ew from the bootstrapped sample
    //printf("bootFunc p2\n");fflush(stdout);
    mat result = genRollingErr(rt, x, ini_win, propsigma_beta, 0);
    //printf("bootFunc p3\n");fflush(stdout);
    vec en = result.col(0);
    vec ea = result.col(1);
    vec ew = result.col(2);

    //calculate the OOS statistics
    vec en_2 = pow(en, 2);
    vec ea_2 = pow(ea, 2);
    vec ew_2 = pow(ew, 2);
    double mse_en = mean(en_2);
    double mse_ea = mean(ea_2);
    double mse_ew = mean(ew_2);

    // the MSE-F statistic
    double msef_boot_ea = ((double)(S+1-1+1)) * (mse_en - mse_ea) / (mse_ea);
    double msef_boot_ew = ((double)(S+1-1+1)) * (mse_en - mse_ew) / (mse_ew);

    vec res(2);
    res(0) = msef_boot_ea;
    res(1) = msef_boot_ew;
    //printf("bootFunc p4\n");fflush(stdout);

    return res;
}
