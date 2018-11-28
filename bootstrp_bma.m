%% obtain the bootstrapped crtical values for the bma method 

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

% construct log equity premium
rt = log((prc(2:end)+dvd(2:end))./prc(1:end-1))...
    - log(1+rf(2:end)); % note rt(1) refers to the log risk premium in period 2 and so on 
                        % note that size(x,1) = size(rt,1) + 1
              
% set the initial window for training sample
ini_win = 20; % CHANGE when consider begin forecast in 1965 or in 1976

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

% bootstrapped MSE-F statistics for bma
msef_boot = bootstrp(400,@(resid)bootfun_bma(resid, rt_fit, x_fit, ini_win, x,propsigma_beta),resid);

msef_boot_ea = msef_boot(:,1);
msef_boot_ew = msef_boot(:,2);
msef_boot_bma = msef_boot(:,3);

% bootstrapped critical values
cv_90_bma = prctile(msef_boot_bma,90);
cv_90_ea = prctile(msef_boot_ea,90);
cv_90_ew = prctile(msef_boot_ew,90);

cv_95_bma = prctile(msef_boot_bma,95);
cv_95_ea = prctile(msef_boot_ea,95);
cv_95_ew = prctile(msef_boot_ew,95);

cv_99_bma = prctile(msef_boot_bma,99);
cv_99_ea = prctile(msef_boot_ea,99);
cv_99_ew = prctile(msef_boot_ew,99);

toc