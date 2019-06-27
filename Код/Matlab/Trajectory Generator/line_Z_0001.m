t = 430:-0.001:250;

X = 70 * ones(size(t));
Y = zeros(size(t));
Z = t;
plot3(X, Y, Z)
grid on

dlmwrite('line_Z_0001.txt', [X', Y', Z'], ' ');