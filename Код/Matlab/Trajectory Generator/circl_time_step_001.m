t = 0:0.001:2*pi;
radius = 50; % миллиметров
X = radius * cos(t);
Y = radius * sin(t);
Z = 430 * ones(size(t));
plot3(X, Y, Z)
grid on

dlmwrite('CTS0001.txt', [X', Y', Z'], ' ');