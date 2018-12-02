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
    
    Matrix state_a_list(2, N);
    Matrix state_b_list(2, N);
    state_a_list.set_zeros();   //Init storage space for the state of a's sampler
    state_b_list.set_zeros();   //Init storage space for the state of b's sampler
    Cube state_Sigma_list(2, 2, N);
    state_Sigma_list.set_zeros();   //Init storage space for the state of Sigma's sampler

    //OLS estimates as the initial value
    Matrix X_trans = X.transpose();
    Matrix A_hat = inv_mul(mul(X_trans, X), mul(X_trans, Y));   //OLS estimates for A
    Matrix Epsilon_hat = sub(Y, mul(X, A_hat));
    Matrix Sigma_hat = Epsilon_hat.cov();
    Vec a_hat = A_hat.col_x(1);
    Vec b_hat = A_hat.col_x(2);

    //current state of a, b, Sigma
    Vec state_a = A_hat.col_x(1);
    Vec state_b = A_hat.col_x(2);
    Matrix state_Sigma = Sigma_hat;

    //initialize iteration at 1
    int s = 1;
    Matrix state_a_list(state_a.size, N + 10);
    Matrix state_b_list(state_b.size, N + 10);
    Cube state_Sigma_list(state_Sigma.row, state_Sigma.col, N + 10);
    state_a_list.set_col(s, state_a);   //save the current state of a
    state_b_list.set_col(s, state_b);   //save the current state of b
    state_Sigma_list.set_matab(s, state_Sigma);   //save the current state of Sigma

    Matrix state_A(state_a.size, 2);
    //Start sampling with 3 blocks
    while (s < N)   //Iterate until we have N samples
    {
        s++;
        //block 1: draw from P(Sigma | a, b, D, M1)
        //Propose a new value for Sigma 
        for (int i = 1; i <= state_a.size; i++)
        {
            state_A.m[i][1] = state_a.v[i];
            state_A.m[i][2] = state_b.v[i];
        }
        Matrix tmp = sub(Y, mul(X, state_A));
        Matrix tau = mul(tmp.transpose(), tmp);     //scalar factor tau
        
    }

    
    % Propose a new value for Sigma  
    state_A = [state_a, state_b];
    tau = (Y - X*state_A)'*(Y - X*state_A); % scalar factor tau
    new_Sigma = iwishrnd( tau , T-2 ); % a candidate draw from the inverted Wishart
    sigma_u_star = new_Sigma(1,1)^0.5;
    sigma2_v_star = new_Sigma(2,2);
    sigma_u_state = state_Sigma(1,1)^0.5; % value in the state
    sigma2_v_state = state_Sigma(2,2); 
    mu_x = state_b(1)/(1-state_b(2)); % stationary mean of x
    pratio = sigma_u_star*exp(-0.5*(x0-mu_x)^2*(1-state_b(2)^2)/sigma2_v_star) / ...
             sigma_u_state*exp(-0.5*(x0-mu_x)^2*(1-state_b(2)^2)/sigma2_v_state);  
    alpha = min( [ 1 pratio ] ); % Calculate the acceptance ratio        
    u = rand; % Draw a uniform deviate from [ 0 1 ]    
    if u < alpha  % Do we accept this proposal?
        state_Sigma = new_Sigma; % proposal becomes new value for Sigma
    end
        
    %% Block 2: draw from P(b | a, Sigma, D, M1)
     
    V = inv(kron(inv(state_Sigma),X'*X));
    v11 = V(1:2,1:2);
    v12 = V(1:2,3:4);
    v21 = V(3:4,1:2);
    v22 = V(3:4,3:4);

    
    % Propose a new value for b 
    prop_mu = b_hat + v21*(v11\(state_a - a_hat));
    prop_Sigma = v22 - v21*(v11\v12);
    
    % eliminate the numerical rounding error
    prop_Sigma = (prop_Sigma + prop_Sigma')/2;
    
    new_b = mvnrnd( prop_mu , prop_Sigma );
    
    % rho should be in (-1,1)
    while (new_b(2)<-1) || (new_b(2)>1)
        new_b = mvnrnd( prop_mu , prop_Sigma );
    end 
    
    sigma2_v_state = state_Sigma(2,2); 
    pratio = exp(-0.5*(x0-new_b(1)/(1-new_b(2)))^2*(1-new_b(2)^2)/sigma2_v_state) / ...
             exp(-0.5*(x0-state_b(1)/(1-state_b(2)))^2*(1-state_b(2)^2)/sigma2_v_state);          
    alpha = min( [ 1 pratio ] ); % Calculate the acceptance ratio    
    u = rand; % Draw a uniform deviate from [ 0 1 ]    
    if u < alpha  % Do we accept this proposal?
        state_b = new_b'; % proposal becomes new value for b
    end
    
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

