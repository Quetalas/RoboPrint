com_decawave_left = 'COM5';
 
 obj1 = instrfind('Type', 'serial', 'Port', com_decawave_left, 'Tag', '');
             if isempty(obj1)
                 obj1 = serial(com_decawave_left,'BaudRate',115200,'InputBufferSize', 1*256,'OutputBufferSize', 1*256);
             else
                 fclose(obj1);
                 obj1 = serial(com_decawave_left,'BaudRate',115200,'InputBufferSize', 1*256,'OutputBufferSize', 1*256);
             end
             fopen(obj1)
 
 fwrite(obj1, 13);
 fwrite(obj1, 13);
 for k = 1:10
     fscanf(obj1);
 end
 
fprintf(obj1,'les')

InitPos = [2.013;2.007]; 
q = 1;
while 1%k <= 200   
   data1 = fscanf(obj1);
   navidata(q,1) = cellstr(data1);
   q = q + 1;
  %pause(0.0001)
end

