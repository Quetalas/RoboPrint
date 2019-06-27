t = 0:0.001:800;

X = t;
Y = zeros(size(t));
Z = 430 * ones(size(t));
plot3(X, Y, Z)
grid on

dlmwrite('line_X_0001.txt', [X', Y', Z'], ' ');