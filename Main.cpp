#include <iostream>
#include <armadillo>
#include "Const.h"
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

void init(void)
{
    // generate the vector of rolling OOS errors
    // construct log equity premium
    int end = prc.n_elem - 1;
    rt = log(prc.subvec(1, end) + dvd.subvec(1, end)) / prc.subvec(0, end - 1) - log(1.0 + rf.subvec(1, end));

    //initial value of predictor
    x0 = x[0];

    // the out-of-sample evaluation periods is S+1
    S = rt.n_elem - ini_win;
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
    init();
    test();
    return 0;
}