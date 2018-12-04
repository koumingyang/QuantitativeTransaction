#include "GenBmaWeights.h"
using namespace std;

mat genBmaWeights (mat Y, mat X, double propsigma_beta)
{
    int T = Y.n_rows;
    double x0 = X(0, 1);
    
    //Initialize the Metropolis sampler
    int N = 100000;
    mat Omega;
    Omega << 0 << 0 << endr << 0 << pow(propsigma_beta, -2.0) << endr;

    mat state_a_list = zeros<mat>(2, N);                    //Init storage space for the state of a's sampler
    mat state_b_list = zeros<mat>(2, N);                    //Init storage space for the state of b's sampler
    cube state_Sigma_list = zeros<cube>(2, 2, N);           //Init storage space for the state of Sigma's sampler

    //OLS estimates as the initial value
    mat A_hat = solve(trans(X) * X, trans(X) * Y);          //OLS estimates for A
    mat Epsilon_hat = Y - X * A_hat;
    mat Sigma_hat = cov(Epsilon_hat);
    vec a_hat = A_hat.col(0);
    vec b_hat = A_hat.col(1);

    //current state of a, b, Sigma
    vec state_a = A_hat.col(0);
    vec state_b = A_hat.col(1);
    mat state_Sigma = Sigma_hat;

    int s = 1;                                              //initialize iteration at 1
    state_a_list.col(s - 1) = state_a;                      //save the current state of a
    state_b_list.col(s - 1) = state_b;                      //save the current state of b
    state_Sigma_list.slice(s - 1) = state_Sigma;            //save the current state of Sigma

    //Start sampling with 3 blocks
    while (s < N)                                           //Iterate until we have N samples
    {
        //block 1: draw from P(Sigma | a, b, D, M1)
        s++;
        //Propose a new value for Sigma
        mat state_A = join_horiz(state_a, state_b);
        mat tmp = Y - X * state_A;
        mat tau = trans(tmp) * tmp;                         //scalar factor tau
        mat new_Sigma = iwishrnd(tau, T - 2);               //a candidate draw from the inverted Wishart
        double sigma_u_star = pow(new_Sigma(0, 0), 0.5);    //value in the state
        double sigma2_v_star = new_Sigma(1, 1);
        double sigma_u_state = pow(state_Sigma(0, 0), 0.5); //value in the state
        double sigma2_v_state = state_Sigma(1, 1);
        double mu_x = state_b(0) / (1.0 - state_b(1));      //stationary mean of x
        double pratio = sigma_u_star * exp(-0.5 * (x0 - mu_x) * (x0 - mu_x) * (1.0 - state_b(1) * state_b(1)) / sigma2_v_star) /
                        sigma_u_state * exp(-0.5 * (x0 - mu_x) * (x0 - mu_x) * (1.0 - state_b(1) * state_b(1)) / sigma2_v_state);
        double alpha = min(1.0, pratio);                    //Calculate the acceptance ratio
        tmp.randu();
        double u = tmp(0, 0);                               //Draw a uniform deviate from [ 0 1 ]
        if (u < alpha)                                      //Do we accept this proposal?
            state_Sigma = new_Sigma;                        //proposal becomes new value for Sigma

        //Block 2: draw from P(b | a, Sigma, D, M1)
        mat V = inv(kron(inv(state_Sigma), trans(X) * X));
        mat v11 = V( span(0, 1), span(0, 1) );
        mat v12 = V( span(0, 1), span(2, 3) );
        mat v21 = V( span(2, 3), span(0, 1) );
        mat v22 = V( span(2, 3), span(2, 3) );
        //Propose a new value for b 
        vec prop_mu = b_hat + v21 * (solve(v11, (state_a - a_hat)));
        mat prop_Sigma = v22 - v21 * (solve(v11, v12));
        //eliminate the numerical rounding error
        prop_Sigma = (prop_Sigma + trans(prop_Sigma)) / 2;
        mat new_b = mvnrnd( prop_mu , prop_Sigma );
        //rho should be in (-1,1)
        while (new_b(1) < -1 || new_b(1) > 1)
            new_b = mvnrnd( prop_mu , prop_Sigma );
        sigma2_v_state = state_Sigma(1,1); 
        pratio = exp(-0.5 * pow((x0 - new_b(0) / (1.0 - new_b(2))), 2.0) * (1.0 - new_b(1) * new_b(1)) / sigma2_v_state) /
                 exp(-0.5 * pow((x0 - state_b(0) / (1.0 - state_b(2))), 2.0) * (1.0 - state_b(1) * state_b(1)) / sigma2_v_state);
        alpha = min(1.0, pratio);                           //Calculate the acceptance ratio
        tmp.randu();
        double u = tmp(0, 0);                               //Draw a uniform deviate from [ 0 1 ]
        if (u < alpha)                                      //Do we accept this proposal?
            state_b = new_b.col(0);                         //proposal becomes new value for b
        
        //Block 3: directly draw from P(a | b, Sigma, D, M1)

    }       


    
    %% Block 3: directly draw from P(a | b, Sigma, D, M1)
    
    mvn_mu = (Omega + inv(v11 - v12*(v22\v21)))\((v11 - v12*(v22\v21)) \ ...
         (a_hat + v12*(v22\(state_b - b_hat))));    
 
    mvn_Sigma = inv(Omega + inv(v11 - v12*(v22\v21)));
    
    % eliminate the numerical rounding error
    mvn_Sigma = (mvn_Sigma + mvn_Sigma')/2;
    
    new_a = mvnrnd(mvn_mu, mvn_Sigma);
    state_a = new_a';
    
    %% Save state
    state_a_list(:,s) = state_a;
    state_b_list(:,s) = state_b;
    state_Sigma_list(:,:,s) = state_Sigma; 
    
   
end

burning = 50000;
S = N - 50000; % number of effective draws
a_draws = state_a_list(:,burning+1:end);
b_draws = state_b_list(:,burning+1:end);
Sigma_draws = state_Sigma_list(:,:,burning+1:end);

%% Calculate Bayes factor by the Savage-Dickey ratio
% Calculate P(beta=0 | D��M1)
probbeta0 = NaN(S,1);
for s=1:S
    V = inv(kron(inv(Sigma_draws(:,:,s)),X'*X));
    v11 = V(1:2,1:2);
    v12 = V(1:2,3:4);
    v21 = V(3:4,1:2);
    v22 = V(3:4,3:4);
    
    % a | b, Sigma, D, M1 is N(mu_a, Sigma_a)
    mu_a = (Omega + inv(v11 - v12*(v22\v21)))\((v11 - v12*(v22\v21)) \ ...
         (a_hat + v12*(v22\(b_draws(:,s) - b_hat))));    
    Sigma_a = inv(Omega + inv(v11 - v12*(v22\v21)));
    
    % beta | d0, Sigma, D, M1 is N(postmu_beta, postsigma2_beta)
    postmu_beta = mu_a(2) + (a_draws(1,s)-mu_a(1))*Sigma_a(1,2)/Sigma_a(1,1);
    postsigma2_beta = (1 - Sigma_a(1,2)^2/(Sigma_a(1,1)*Sigma_a(2,2)))*Sigma_a(2,2);
    
    probbeta0(s) = exp(-0.5*postmu_beta^2/postsigma2_beta)/postsigma2_beta^0.5;
end

bayes_factor = mean(probbeta0) / (1/propsigma_beta);

w0 = bayes_factor / (1 + bayes_factor);
w1 = 1 - w0;
