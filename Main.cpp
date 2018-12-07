#include <iostream>
#include <armadillo>
#include "Const.h"
#include "BootFunc.h"
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

    for (i = 0; i < rows; i++)
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

    cout << "Input Over" << endl;
}

void work(void)
{
    // generate the vector of rolling OOS errors
    // construct log equity premium
    srand(unsigned(time(0)));
    int end = prc.n_elem - 1;
    rt = log(prc.subvec(1, end) + dvd.subvec(1, end)) / prc.subvec(0, end - 1) - log(1.0 + rf.subvec(1, end));

    //initial value of predictor
    x0 = x[0];
    cout << "Main Part0 Over" << endl;

    // the out-of-sample evaluation periods is S+1

    int ini_win = 20;               //CHANGE when consider begin forecast in 1965 or in 1976
    double propsigma_beta = 0.01;   //set hyperparameter sigma_beta to: 0.0001, 0.001, 0.01, 0.1, 1, 10
    S = rt.n_elem - ini_win;
    cout << "Main Part0.1 Over" << endl;

    
    mat result = genRollingErr(rt, x, ini_win, propsigma_beta, 1);
    cout << "Main Part0.2 Over" << endl;
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

    //the OOS R2; all compared with the historical mean model
    double R2_ea = 1.0 - mse_ea / mse_en;
    double R2_ew = 1.0 - mse_ew / mse_en;
    double R2_eb = 1.0 - mse_eb / mse_en;

    //the adjusted OOS R2
    double adjR2_ea = 1.0 - (1.0 - R2_ea) * (double)(S+1-1) / (double)(S+1-2);
    double adjR2_ew = 1.0 - (1.0 - R2_ew) * (double)(S+1-1) / (double)(S+1-2);
    double adjR2_eb = 1.0 - (1.0 - R2_eb) * (double)(S+1-1) / (double)(S+1-2);

    //the difference RMSE
    double drmse_ea = sqrt(mse_en) - sqrt(mse_ea);
    double drmse_ew = sqrt(mse_en) - sqrt(mse_ew);
    double drmse_eb = sqrt(mse_en) - sqrt(mse_eb);

    //the MSE-F statistic
    double msef_ea = ((double)(S+1-1+1)) * (mse_en - mse_ea) / (mse_ea);
    double msef_ew = ((double)(S+1-1+1)) * (mse_en - mse_ew) / (mse_ew);
    double msef_eb = ((double)(S+1-1+1)) * (mse_en - mse_eb) / (mse_eb);

    cout << "Main Part1 Over" << endl;
    

    //Bootstrap the critical values for the MSE-F statistics
    //use full sample to estimate the parameter in the NULL (no predictability)
    //model

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
    vec msef_boot_ea(10000);
    vec msef_boot_ew(10000);
    for (i = 1; i <= 10000; i++)
    {
        //printf("Bootstrap No. %d p0\n", i);
        mat now_resid = bootstrapSample(resid);
        //printf("Bootstrap No. %d p1\n", i);
        vec msef_boot = bootFunc(now_resid, rt_fit, x_fit, ini_win, x, propsigma_beta);
        //printf("Bootstrap No. %d p2\n", i);
        msef_boot_ea(i-1) = msef_boot(0);
        msef_boot_ew(i-1) = msef_boot(1);
        //printf("Bootstrap No. %d p3\n", i);
        if (i % 100 == 0)
            printf("Bootstrap No. %d\n", i);
    }
    //bootstrapped critical values

    double cv_90_ea = prctile(msef_boot_ea, 90);
    double cv_90_ew = prctile(msef_boot_ew, 90);

    double cv_95_ea = prctile(msef_boot_ea, 95);
    double cv_95_ew = prctile(msef_boot_ew, 95);

    double cv_99_ea = prctile(msef_boot_ea, 99);
    double cv_99_ew = prctile(msef_boot_ew, 99);

    printf("cv_90_ea = %lf\ncv_90_ew = %lf\ncv_95_ea = %lf\ncv_95_ew = %lf\ncv_99_ea = %lf\ncv_99_ew = %lf\n", 
        cv_90_ea, cv_90_ew, cv_95_ea, cv_95_ew, cv_99_ea, cv_99_ew);
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
