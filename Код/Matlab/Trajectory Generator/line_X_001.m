t = 0:0.01:800;

X = t;
Y = zeros(size(t));
Z = 430 * ones(size(t));
plot3(X, Y, Z)
grid on

dlmwrite('line_X_001.txt', [X', Y', Z'], ' ');