%% MH algorithm to calculate the BMA weights
function [w0,w1] = gen_bma_weights(Y,X,propsigma_beta)
% function to calculate the weights by Bayesian Model Average (BMA)

% INPUTS:
%       Y: [r(1);...;r(T),x(1);...;x(T)] 
%       X: [1;...;1, x(0),...,x(T-1)]
%       r: equity premium, x: predictor
%       propsigma_beta: standard dev. of proposal distribution for beta

% OUTPUTS:
%       w0: the weight for model 0
%       w1: the weight for model 1

T = size(Y,1);
x0 = X(1,2);

%% Initialize the Metropolis sampler
N     = 100000; % Set the maximum number of iterations
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

s = 1; % initialize iteration at 1
state_a_list(:,s) = state_a; % save the current state of a
state_b_list(:,s) = state_b; % save the current state of b
state_Sigma_list(:,:,s) = state_Sigma; % save the current state of Sigma

%% Start sampling with 3 blocks
while s < N % Iterate until we have N samples
    s = s + 1;    
    
    %% block 1: draw from P(Sigma | a, b, D, M1)
    
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
