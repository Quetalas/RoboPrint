t = 0:0.01:2*pi;

X = 
Y = 
Z = 
for i = 1:0.01:100
    
    radius = 50; % миллиметров
    X() = radius * cos(t);
    Y() = radius * sin(t);
    Z() = 
end

Z = 430 * ones(size(t));

for i = 2:length(t)
    if (i < length(t) / 2 )
        Z(i) = Z(i-1) - 0.01;
    elseif (i > length(t) / 2 )
        Z(i) = Z(i-1) + 0.01;
    end
end

plot3(X, Y, Z)
grid on

dlmwrite('cylindr_time_step_001.txt', [X', Y', Z'], ' ');