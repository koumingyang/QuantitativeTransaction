// obtain the bootstrapped crtical values for the bma method 
#include <iostream>
#include <armadillo>
#include "Const.h"
#include "BootFunc.h"
#include "BootFuncBMA.h"
#include "GenRollingErr.h"

using namespace std;
using namespace arma;

mat dat;
vec year, prc, dvd, x, rf;
vec rt;
int S;
int rows, cols;
double x0;

void input(void)
{
    system("python3 get_xls.py");
    freopen("xls.txt", "r", stdin);
    int i, j;
    cin >> rows >> cols;
    cout << rows << ' ' << cols << endl;
    dat = mat(rows, cols);

    for (i = 0; i = rows; i++)
        for (j = 0; j < cols; j++)
            scanf("%lf", &dat(i, j));

    year = dat.col(0);
    prc = dat.col(1);     //price 
    dvd = dat.col(2);     //dividend
    x = dat.col(4);       //CHANGE to the relevant predictor, book to market value
    rf = dat.col(11);     //risk free rate

    /*
    for (i = 1; i <= rows; i++)
        cout << year.v[i] << ' ' ;
    cout << endl;*/
}

void work(void)
{
    // generate the vector of rolling OOS errors
    // construct log equity premium
    srand(unsigned(time(0)));
    int end = prc.n_elem - 1;
    rt = log(prc.subvec(1, end) + dvd.subvec(1, end)) / prc.subvec(0, end - 1) - log(1.0 + rf.subvec(1, end));

    int ini_win = 20;               //CHANGE when consider begin forecast in 1965 or in 1976
    double propsigma_beta = 0.01;   //set hyperparameter sigma_beta to: 0.0001, 0.001, 0.01, 0.1, 1, 10
    
    double alpha_hat = mean(rt);
    vec rt_fit(rt.n_elem);
    rt_fit.fill(alpha_hat);                             //rt fit in the 1st equation of NULL predictability model
    vec u_hat = rt - rt_fit;                            //residual from rt_fit

    mat ty(x.n_elem - 1, 2);
    ty.col(0).fill(1.0);
    ty.col(1) = x.subvec(0, x.n_elem - 2);
    vec b = regress(x.subvec(1, x.n_elem - 1), ty, 1);

    vec x_fit = ty * b;                                 //x fit in the 2nd equation of NULL predictability model
    vec v_hat = x.subvec(1, x.n_elem - 1) - x_fit;      //residual from x_fit

    mat resid = join_horiz(u_hat, v_hat);               //residuals for bootstrap

    //bootstrapped MSE-F statistics for ea and ew
    int i;
    vec msef_boot_ea(400);
    vec msef_boot_ew(400);
    vec msef_boot_bma(400);
    for (i = 1; i <= 400; i++)
    {
        mat now_resid = bootstrapSample(resid);
        vec msef_boot = bootFuncBMA(now_resid, rt_fit, x_fit, ini_win, x, propsigma_beta);
        msef_boot_ea(i-1) = msef_boot(0);
        msef_boot_ew(i-1) = msef_boot(1);
        msef_boot_bma(i-1) = msef_boot(2);
    }
    //bootstrapped critical values

    double cv_90_bma = prctile(msef_boot_bma, 90);
    double cv_90_ea = prctile(msef_boot_ea, 90);
    double cv_90_ew = prctile(msef_boot_ew, 90);

    double cv_95_bma = prctile(msef_boot_bma, 95);
    double cv_95_ea = prctile(msef_boot_ea, 95);
    double cv_95_ew = prctile(msef_boot_ew, 95);

    double cv_99_bma = prctile(msef_boot_bma, 99);
    double cv_99_ea = prctile(msef_boot_ea, 99);
    double cv_99_ew = prctile(msef_boot_ew, 99);
}

void test(void)
{
    /*
    a = [[1;2;3;4],[3;5;3;12],[9;5;2;6]];
    cov(a)
    ans =

    1.6667    4.1667   -2.0000
    4.1667   18.2500    1.1667
   -2.0000    1.1667    8.3333
   */
}

int main(void)
{
    input();
    work();
    test();
    return 0;
}
