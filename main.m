%% Use bma weights to combine forecasts
% main program to combine forecasts from predictive and unpredictive models 

clear all

tic 

rng('default') % For reproducibility

%% retrieve data
data = xlsread('annual1.xlsx'); % obtain the original data

% find the rows with nan in book to market value (col 5 in data)
[rows, ~] = find(isnan(data(:,5))); % CHANGE to the relevant predictor, currently book to market value
% delete the rows with nan in book to market value
data(rows,:) = [];

year=data(:,1);
prc = data(:,2); % price
dvd = data(:,3); % dividend
x = data(:,5); % CHANGE to the relevant predictor, book to market value
rf = data(:,12); % risk free rate

propsigma_beta = 0.01; % set hyperparameter sigma_beta to: 0.0001, 0.001, 0.01, 0.1, 1, 10

%% generate the vector of rolling OOS errors
% construct log equity premium
rt = log((prc(2:end)+dvd(2:end))./prc(1:end-1))...
    - log(1+rf(2:end)); % note rt(1) refers to the log risk premium in period 2 and so on 
                        % note that size(x,1) = size(rt,1) + 1
x0 = x(1); % initial value of predictor

% set the initial window for training sample
ini_win = 20; % CHANGE when consider begin forecast in 1965 or in 1976
S = size(rt,1) - ini_win; % the out-of-sample evaluation periods is S+1
[en,ea,ew,eb] = gen_rolling_err(rt,x,ini_win,propsigma_beta,1);

%% calculate the OOS statistics

mse_en = mean(en.^2);
mse_ea = mean(ea.^2);
mse_ew = mean(ew.^2);
mse_eb = mean(eb.^2);

% the OOS R2; all compared with the historical mean model
R2_ea = 1 - mse_ea / mse_en;
R2_ew = 1 - mse_ew / mse_en;
R2_eb = 1 - mse_eb / mse_en;

% the adjusted OOS R2
adjR2_ea = 1 - (1 - R2_ea) * (S+1-1) / (S+1-2);
adjR2_ew = 1 - (1 - R2_ew) * (S+1-1) / (S+1-2);
adjR2_eb = 1 - (1 - R2_eb) * (S+1-1) / (S+1-2);

% the difference RMSE
drmse_ea = sqrt(mse_en) - sqrt(mse_ea);
drmse_ew = sqrt(mse_en) - sqrt(mse_ew);
drmse_eb = sqrt(mse_en) - sqrt(mse_eb);

% the MSE-F statistic
msef_ea = (S+1-1+1)*(mse_en - mse_ea)/(mse_ea);
msef_ew = (S+1-1+1)*(mse_en - mse_ew)/(mse_ew);
msef_eb = (S+1-1+1)*(mse_en - mse_eb)/(mse_eb);

%% Bootstrap the critical values for the MSE-F statistics
% use full sample to estimate the parameter in the NULL (no predictability)
% model

alpha_hat = mean(rt);
rt_fit = ones(size(rt))*alpha_hat; % rt fit in the 1st equation of NULL predictability model
u_hat = rt - rt_fit; % residual from rt_fit


b = regress(x(2:end),[ones(size(x(2:end))),x(1:end-1)]);
x_fit = [ones(size(x(2:end))),x(1:end-1)]*b; % x fit in the 2nd equation of NULL predictability model
v_hat = x(2:end) - x_fit; % residual from x_fit

resid = [u_hat, v_hat]; % residuals for bootstrap

% bootstrapped MSE-F statistics for ea and ew
msef_boot = bootstrp(10000,@(resid)bootfun(resid, rt_fit, x_fit, ini_win, x,propsigma_beta),resid);

msef_boot_ea = msef_boot(:,1);
msef_boot_ew = msef_boot(:,2);

% bootstrapped critical values
cv_90_ea = prctile(msef_boot_ea,90);
cv_90_ew = prctile(msef_boot_ew,90);

cv_95_ea = prctile(msef_boot_ea,95);
cv_95_ew = prctile(msef_boot_ew,95);

cv_99_ea = prctile(msef_boot_ea,99);
cv_99_ew = prctile(msef_boot_ew,99);

toc