#include <iostream>
#include "const.h"
#include "matrix.h"
#include "vec.h"

using namespace std;

Vec year, prc, dvd, x, rf;
Vec rt;
int def_size, S;
double x0;

void input(void)
{
    def_size = DEFAULT_MATRIX_SIZE;
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