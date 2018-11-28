#include <iostream>
#include "Const.h"
#include "Matrix.h"
#include "Vec.h"

using namespace std;

Matrix dat;
Vec year, prc, dvd, x, rf;
Vec rt;
int def_size, S;
int rows, cols;
double x0;

void input(void)
{
    system("python3 get_xls.py");
    freopen("xls.txt", "r", stdin);
    int i, j;
    cin >> rows >> cols;
    cout << rows << ' ' << cols << endl;
    dat.set_size(rows, cols);


    for (i = 1; i <= rows; i++)
        for (j = 1; j <= cols; j++)
            scanf("%lf", &dat.m[i][j]);
    def_size = DEFAULT_MATRIX_SIZE;

    year = dat.col_x(1);
    prc = dat.col_x(2);     //price 
    dvd = dat.col_x(3);     //dividend
    x = dat.col_x(5);       //CHANGE to the relevant predictor, book to market value
    rf = dat.col_x(12);     //risk free rate

    /*
    for (i = 1; i <= rows; i++)
        cout << year.v[i] << ' ' ;
    cout << endl;*/
}

void init(void)
{
    // generate the vector of rolling OOS errors
    // construct log equity premium
    Vec tmp = div(add(prc.sub_vec(2, prc.size), dvd.sub_vec(2, dvd.size)), prc.sub_vec(1, prc.size - 1));
    rt = sub(tmp.log_all(), add(rf.sub_vec(2, rf.size), 1.0).log_all());
    
    //initial value of predictor
    x0 = x.v[1];

    // the out-of-sample evaluation periods is S+1
    S = rt.size - ini_win;
}

int main(void)
{
    input();
    init();
    return 0;
}