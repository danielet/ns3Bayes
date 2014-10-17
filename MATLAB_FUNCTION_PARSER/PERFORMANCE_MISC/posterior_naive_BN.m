% GQ, 15/08/2014
%
% function for FL
%
% This file calculates the posterior given the values of the conditioning
% variables
% INPUTS:
%       x =                 vector with the values of the cond variables
%       prior =             prior over y
%       cond_prob =         cell with all the p(x_j|y)
%       
% OUTPUTS
%       posterior =         the posterior probability, dim(y) 

function posterior = posterior_naive_BN(x,prior,cond_prob) 

% dimensionality check
if (~(length(cond_prob)) == length(x))
    fprintf('There is a dimensionality mismatch in the evidence vector');
end

% we start
posterior = prior;

for jj = 1:length(x)
    
            %s_posterior = size(posterior)
            %s_to_add = size(cond_prob{jj}(x(jj),:))
    
    % I take p(x_jj=ev|y)
    cond_prob{jj}(x(jj),:)
    posterior = posterior .*cond_prob{jj}(x(jj),:)';

end

% re- normalization
posterior = posterior / sum(posterior);