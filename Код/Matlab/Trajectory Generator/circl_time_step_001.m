t = 0:0.01:100;
radius = 50; % миллиметров
X = radius * cos(t);
Y = radius * sin(t);
Z = 430 * ones(size(t));
plot3(X, Y, Z)
grid on

dlmwrite('circl_time_step_001.txt', [X', Y', Z'], ' ');