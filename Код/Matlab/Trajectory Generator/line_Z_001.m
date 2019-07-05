t = 430:-2:250;

X = 70 * ones(size(t));
Y = zeros(size(t));
Z = t;
plot3(X, Y, Z)
grid on

dlmwrite('lineZ2.txt', [X', Y', Z'], ' ');