%% function to calculate rolling OOS errors
function [en, ea, ew, eb] = gen_rolling_err(rt,x,ini_win,propsigma_beta,flag_bma)

% INPUTS:
%       rt: log equity premium
%       x: predictor
%       ini_win: initial window for the training sample
%       propsigma_beta: hyperparameter sigma_beta used in the bma
%       flag_bma: 1 -> generate rolling OOS error eb from the BMA 
%                 0 -> Not to generate 

% OUTPUTS:
%       en: the rolling OOS errors from the historical mean
%       ea: the rolling OOS errors from the ols
%       ew: the rolling OOS errors from the equal weights
%       eb: the rolling OOS errors from the bma

S = size(rt,1) - ini_win; % the out-of-sample evaluation periods is S+1

en = NaN(S+1,1); % the rolling OOS errors from the historical mean
ea = NaN(S+1,1); % the rolling OOS errors from the ols
ew = NaN(S+1,1); % the rolling OOS errors from the equal weights
eb = NaN(S+1,1); % the rolling OOS errors from the bma

if flag_bma==1

    for s=0:S   

        Ts = ini_win + s; % time for the 1-step forecast in rolling s

        Y = [rt(1:Ts-1),x(2:Ts)]; 
        X = [ones(Ts-1,1),x(1:Ts-1)];

        [w0,w1] = gen_bma_weights(Y,X,propsigma_beta); % bma weights

        f0 = mean(rt(1:Ts-1)); % forecast from the historical mean

        b = regress(rt(1:Ts-1), X);
        f1 = [1, x(Ts)]*b; % forecast from the ols

        f_ew = 0.5*f0 + 0.5*f1; % forecast from the naive forecast combination

        f_bma = w0*f0 + w1*f1; % forecast from the bma

        % index s+1 is due to s starting from 0
        en(s+1) = rt(Ts) - f0; % OOS errors from the historical mean
        ea(s+1) = rt(Ts) - f1; % OOS errors from the ols
        ew(s+1) = rt(Ts) - f_ew; % OOS errors from the naive forecast combination
        eb(s+1) = rt(Ts) - f_bma; % OOS errors from the bma
    end 
    
else
    
    for s=0:S   

        Ts = ini_win + s; % time for the 1-step forecast in rolling s

        Y = [rt(1:Ts-1),x(2:Ts)]; 
        X = [ones(Ts-1,1),x(1:Ts-1)];

        %%%%[w0,w1] = gen_bma_weights(Y,X,propsigma_beta); % bma weights

        f0 = mean(rt(1:Ts-1)); % forecast from the historical mean

        b = regress(rt(1:Ts-1), X);
        f1 = [1, x(Ts)]*b; % forecast from the ols

        f_ew = 0.5*f0 + 0.5*f1; % forecast from the naive forecast combination

        %%%%%f_bma = w0*f0 + w1*f1; % forecast from the bma

        % index s+1 is due to s starting from 0
        en(s+1) = rt(Ts) - f0; % OOS errors from the historical mean
        ea(s+1) = rt(Ts) - f1; % OOS errors from the ols
        ew(s+1) = rt(Ts) - f_ew; % OOS errors from the naive forecast combination
        %%%%%eb(s+1) = rt(Ts) - f_bma; % OOS errors from the bma
    end 
end