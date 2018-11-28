%% function used in bootstrap to generate MSE-F statistics including the one from the bma
function [msef_boot] = bootfun_bma(resid, rt_fit, x_fit, ini_win, x,propsigma_beta)

% INPUTS:
%       resid: resid for bootstrap
%       rt_fit: fitted rt
%       x_fit: fitted x
%       ini_win: initial window for the training sample
%       x: predictor, use to generate initial observation
%       propsigma_beta: hyperparameter sigma_beta used in the bma

% OUTPUTS:
%       msef_boot_ea: the bootstrapped MSE-F statistics from the ols
%       msef_boot_ew: the bootstrapped MSE-F statistics from equal weight
%       msef_boot_eb: the bootstrapped MSE-F statistics from the bma

rt = rt_fit + resid(:,1); % rt constructed by the bootstrapped residuals

S = size(rt,1) - ini_win; % the out-of-sample evaluation periods is S+1

x0 = randsample(x,1); % random draw to set the initial observation
x = x_fit + resid(:,2); % x(2:end) constructed by the bootstrapped residuals
x = [x0; x]; % with selected initial observation

% to calculate en, ea, ew, eb from the bootstrapped sample
[en, ea, ew, eb] = gen_rolling_err(rt,x,ini_win,propsigma_beta,1);

%% calculate the OOS statistics

mse_en = mean(en.^2);
mse_ea = mean(ea.^2);
mse_ew = mean(ew.^2);
mse_eb = mean(eb.^2);

% the MSE-F statistic
msef_boot_ea = (S+1-1+1)*(mse_en - mse_ea)/(mse_ea);
msef_boot_ew = (S+1-1+1)*(mse_en - mse_ew)/(mse_ew);
msef_boot_eb = (S+1-1+1)*(mse_en - mse_eb)/(mse_eb);

msef_boot = [msef_boot_ea, msef_boot_ew, msef_boot_eb];
