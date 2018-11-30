#include <iostream>
#include "Const.h"
#include "Matrix.h"
#include "Vec.h"
#include "GenRollingErr.h"

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
    Matrix a(4,3);
    a.m[1][1] = 1; a.m[2][1] = 2; a.m[3][1] = 3; a.m[4][1] = 4;
    a.m[1][2] = 3; a.m[2][2] = 5; a.m[3][2] = 3; a.m[4][2] = 12;
    a.m[1][3] = 9; a.m[2][3] = 5; a.m[3][3] = 2; a.m[4][3] = 6;
    Matrix b = a.cov();
    cout << b.row << ' ' << b.col << endl;
    for (int i = 1; i <= b.row; i++)
    {
        for (int j = 1; j <= b.col; j++)
            cout << b.m[i][j] << ' ';
        cout << endl;
    }
}

int main(void)
{
    input();
    init();
    test();
    return 0;
}