
 close all
 clear all
 clc
 
 disp('Настройка портов')
 
 obj1 = instrfind('Type', 'serial', 'Port', 'COM5', 'Tag', '');
 if isempty(obj1)
     obj1 = serial('COM5');
 else
     fclose(obj1);
     obj1 = obj1(1);
 end
 set(obj1, 'InputBufferSize', 256, 'OutputBufferSize', 256, 'BaudRate', 115200);
 fopen(obj1);
 
fwrite(obj1, 13);
fwrite(obj1, 13);
 
  for k = 1:10
      fscanf(obj1);
 end
 fwrite(obj1, 13);
 fwrite(obj1, 13);
 
 q = 1;
 

while 1    
        fprintf(obj1,'apg')
        fscanf(obj1);
        data1 = fscanf(obj1);
        navidata(q,1) = cellstr(data1);
        q = q + 1;
        pause(0.09)
end
