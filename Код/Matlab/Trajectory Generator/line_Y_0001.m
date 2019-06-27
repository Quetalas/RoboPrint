t = -50:0.001:50;

X = 70 * ones(size(t));
Y = t;
Z = 430 * ones(size(t));
plot3(X, Y, Z)
grid on

dlmwrite('line_Y_0001.txt', [X', Y', Z'], ' ');