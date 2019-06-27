t = 0:0.001:100;
radius = 50; % миллиметров
X = radius * cos(t);
Y = radius * sin(t);
Z = 430 * ones(size(t));

for i = 2:length(t)
    if (i < length(t) / 2 )
        Z(i) = Z(i-1) - 0.001;
    elseif (i > length(t) / 2 )
        Z(i) = Z(i-1) + 0.001;
    end
end

plot3(X, Y, Z)
grid on

dlmwrite('cylindr_time_step_0001.txt', [X', Y', Z'], ' ');