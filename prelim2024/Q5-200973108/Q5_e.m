%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                           PRELIMS 2024                        %%%%%
%%%%                      Student Id : 200973108                   %%%%%                                         
%%%%                       Question-5: Part (e)                    %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Define the Legendre polynomials
P = {@(x) 1, @(x) x, @(x) (3*x.^2 - 1)/2, @(x) (5*x.^3 - 3*x)/2, @(x) (35*x.^4 - 30*x.^2 + 3)/8};

% Define the original function f(x)
f = @(x) exp(-x.^2) .* sin(x - 1) + 1;

% Number of Legendre polynomials
n = 5;

% Define the objective function W
objective = @(c) compute_W(c, f, P);

% Initial guess for coefficients c_i
initial_guess = zeros(n, 1);

% Minimize W using fminunc and complex step differentiation
[c, W] = fminunc(objective, initial_guess);

% Display the coefficients c_i
disp('Optimized coefficients c_i:');
for i = 1:length(c)
    disp(['c', num2str(i-1), ' = ', num2str(c(i))]);
end

function [W, grad] = compute_W(c, f, P)
    % Compute the objective function W and its gradient given coefficients c_i
    n = numel(c);
 
    grad = zeros(n, 1);
    h = 1e-8; % Small perturbation for complex step differentiation
    for i = 1:n
        % Perturb the coefficient c_i using complex step differentiation
        c_perturbed = c;
        c_perturbed(i) = c_perturbed(i) + 1i * h;
        
        % Compute W with the perturbed coefficient
        W_perturbed = compute_W_value(c_perturbed, f, P);
        
        % Compute the gradient using complex step differentiation
        grad_i = imag(W_perturbed) / h;
        grad(i) = grad_i;
    end
    % Compute the objective function value for the original coefficients
    W = compute_W_value(c, f, P);
end

function W = compute_W_value(c, f, P)
    % Function to compute the objective function W given coefficients c_i
    n = numel(c);
    W = 0;
    for i = 1:n
        integrand = @(x) (f(x) - c(i) * P{i}(x)).^2;
        W = W + integral(integrand, -1, 1);
    end
end


%%
% coefficients obtained from previous section.
c = [0.4478, 0.2809, 0.4839, -0.1365, -0.1127];

% Define the Legendre polynomial approximation function P4(x)
P4_function = @(x) sum(c .* cellfun(@(P_i) P_i(x), P));

% Generate x values and compute P4(x) values
x_values = linspace(-1, 1, 400);
P4_values = arrayfun(P4_function, x_values);

% Plot P4(x) and f(x)
plot(x_values, P4_values, 'k', 'LineWidth', 2); hold on;
f_values = f(x_values);
plot(x_values, f_values, 'r', 'LineWidth', 2);
xlabel('x', 'FontSize', 16);
ylabel('P4(x), f(x)' , 'FontSize', 16);
legend('P4(x)', 'f(x)', 'Fontsize', 14);
title('Comparison of P4(x) and f(x)');
grid on;
grid minor;
hold off;

