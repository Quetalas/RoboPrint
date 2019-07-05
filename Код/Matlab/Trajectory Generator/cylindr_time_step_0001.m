t = 0:0.001:2*pi;
radius = 50; % миллиметров
X = radius * cos(t);
Y = radius * sin(t);
Z = 430 * ones(size(t));

for i = 2:length(t)
    if (i < length(t) / 2 )
        Z(i) = Z(i-1) - 0.06;
    elseif (i >= length(t) / 2 )
        Z(i) = Z(i-1) + 0.06;
    end
end

plot3(X, Y, Z)
grid on

dlmwrite('CYTS0001.txt', [X', Y', Z'], ' ');