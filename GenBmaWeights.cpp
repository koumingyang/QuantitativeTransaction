#include "GenBmaWeights.h"
using namespace std;

Matrix genBmaWeights (Matrix Y, Matrix X, double propsigma_beta)
{
    int T = Y.row;
    double x0 = X.m[1][2];

    //Initialize the Metropolis sampler
    int N = 100000; //Set the maximum number of iterations
    Matrix Omega(2, 2);
    Omega.m[1][1] = Omega.m[1][2] = Omega.m[2][1] = 0.0;
    Omega.m[2][2] = pow(propsigma_beta, -2.0);
    Matrix state_a_list = Matrix::zeros(2, N);


Omega = [0 , 0; 0, propsigma_beta^(-2)];
state_a_list = zeros( 2 , N ); % Init storage space for the state of a's sampler
state_b_list = zeros( 2 , N ); % Init storage space for the state of b's sampler
state_Sigma_list = zeros( 2, 2 , N ); % Init storage space for the state of Sigma's sampler

% OLS estimates as the initial value
A_hat = (X'*X)\(X'*Y); % OLS estimates for A
Epsilon_hat = Y - X*A_hat;
Sigma_hat = cov(Epsilon_hat);
a_hat = A_hat(:,1);
b_hat = A_hat(:,2);

% current state of a, b, Sigma
state_a = A_hat(:,1);
state_b = A_hat(:,2);
state_Sigma = Sigma_hat;
}