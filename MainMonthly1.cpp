#include <iostream>
#include <armadillo>
#include "Const.h"
#include "BootFunc.h"
#include "BootFuncBMA.h"
#include "GenRollingErr.h"

using namespace std;
using namespace arma;

mat dat, out;
vec year, Pt, Dt, Et, b_m, cay, ntis, Rfee, eqis, i_k;
vec tbl, Ity, Itr, tms, AAA, BAA, corpr, infl, svar, dfy, dfr;
vec csp, E10, D1, E1;
vec Rt, lagPt, d_e, d_p, d_y, e_p, e10_p;
int rows, cols;

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
    Pt = dat.col(1);
    Dt = dat.col(2);
    Et = dat.col(3);
    b_m = dat.col(4);
    tbl = dat.col(5);
    AAA = dat.col(6);
    BAA = dat.col(7);
    Ity = dat.col(8);
    ntis = dat.col(9);
    Rfee = dat.col(10);
    infl = dat.col(11);
    Itr = dat.col(12);
    corpr = dat.col(13);
    svar = dat.col(14);
    csp = dat.col(15);
    E10 = dat.col(18);
    D1 = dat.col(19);
    E1 = dat.col(20);

    tms=Ity-tbl;
    dfy=BAA-AAA;
    dfr=corpr-Itr;

    int n = Pt.n_elem;
    Rt = vec(n); lagPt = vec(n);
    Rt(0) = 0; lagPt(0) = NAN;
    for (i = 1; i < n; i++)
    {
        lagPt(i) = Pt(i-1);
        Rt(i) = log((Dt(i) + Pt(i)) / Pt(i-1)) - log(1.0 + Rfee(i));
    }

    d_e = log(Dt) - log(Et);
    d_p = log(Dt) - log(Pt);
    d_y = log(Dt) - log(lagPt);
    e_p = log(Et) - log(Pt);
    e10_p = log(E10) - log(Pt);
    out = zeros<mat>(15, 19);
    
    dat.col(0) = Rt;
    dat.col(1) = d_p;
    dat.col(2) = d_y;
    dat.col(3) = e_p;
    dat.col(4) = d_e;
    dat.col(5) = svar;
    dat.col(6) = csp;
    dat.col(7) = b_m;
    dat.col(8) = ntis;
    dat.col(9) = tbl;
    dat.col(10) = Ity;
    dat.col(11) = Itr;
    dat.col(12) = tms;
    dat.col(13) = dfy;
    dat.col(14) = dfr;
    dat.col(15) = infl;
    cout << "Input p5 over" << endl;
}

void work(void)
{
    int i;
    for (i = 0; i < 15; i++)
    {
        int rows = 0;
        for (int j = 0; j < dat.n_rows; j++)
            if (!is_finite(dat(j, i+1)))        //find the rows with nan in book to market value (col 5 in data)
            {
                dat.shed_row(j);        //delete the rows with nan in book to market value
                j--; rows++;
            }
        vec x = dat.col(i+1);           //CHANGE to the relevant predictor, book to market value
        double x0 = x(0);               //initial value of predictor
        vec rt = dat.col(0);
        rt = rt.subvec(1, dat.n_rows - 1);
        int ini_win = 240 - rows;       //forecast from 194701
        double propsigma_beta = 0.0001; //set hyperparameter sigma_beta to: 0.0001, 0.001, 0.01, 0.1, 1, 10
        int S = rt.n_elem - ini_win;    //the out-of-sample evaluation periods is S+1

        //cout << "Main Part0 Over" << endl;
        mat result = genRollingErr(rt, x, ini_win, propsigma_beta, 1);
        //cout << "Main Part0.2 Over" << endl;
        vec en = result.col(0);
        vec ea = result.col(1);
        vec ew = result.col(2);
        vec eb = result.col(3);

        //cout << "P1 OK" << endl;
        //calculate the IS statistics
        int n = rt.n_elem;
        mat X0 = join_horiz(ones<vec>(n), x.subvec(0, n-1));
        vec r0 = regress_r(rt, X0, 1);
        double ssr1 = pow(norm(r0), 2);
        vec mean_rt(n); mean_rt.fill(mean(rt));
        double sst1 = pow(norm(rt - mean_rt), 2);
        double IS_R2 = 1.0 - ssr1 / sst1;
        double IS_adjR2 = 1.0 - (1.0 - IS_R2) * (double)(n-1) / (double)(n-1-1);

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

        vec msef_boot_ea(400);
        vec msef_boot_ew(400);
        vec msef_boot_bma(400);
        for (i = 1; i <= 400; i++)
        {
            //if (i % 100 == 0)
                printf("Bootstrap No. %d\n", i);
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

        vec out1(7);
        out1(0) = IS_adjR2; out1(1) = adjR2_ea; out1(2) = adjR2_ew; out1(3) = adjR2_eb; 
        out1(4) = drmse_ea; out1(5) = drmse_ew; out1(6) = drmse_eb;
        out1 *= 100;
        vec out2(12);
        out2(0) = msef_ea; out2(1) = msef_ew; out2(2) = msef_eb; 
        out2(3) = cv_90_ea; out2(4) = cv_95_ea; out2(5) = cv_99_ea;
        out2(6) = cv_90_ew; out2(7) = cv_95_ew; out2(8) = cv_99_ew; 
        out2(9) = cv_90_bma; out2(10) = cv_95_bma; out2(11) = cv_99_bma;
        out(i, span(0, 6)) = out1;
        out(i, span(7, 18)) = out2;
    }
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
