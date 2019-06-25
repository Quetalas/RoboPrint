FileID = fopen('TEST1.txt','w')

t = 0:0.001:2*pi;
x = 300*cos(t)+500;
y = 70*sin(2*t)+115;
z = 80*cos(2*t)+380;
X  = round([x' y' z']*1000);
% z = 440;
for i = 1:length(t)
    fprintf(FileID, '%d %d %d\r\n', X(i,1), X(i,2), X(i,3));
end
fclose(FileID)
plot3(x,y,z,'*')
grid on
axis([0 1500 0 230 0 500]) 
daspect([1 1 1])
hold on
plot3(x(1),y(1),z(1),'*g')