 com_decawave_left = 'COM18';
 
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
 fscanf(obj1);
 fscanf(obj1);
 
InitPos = [2.013;2.007]; 

while 1%k <= 200   
        data1 = fscanf(obj1);
        Ranges(1,:,k) = DecodeMDEK(data1);
        
        pos_afterfilter(k) = NavSolver_D(Ranges_Sats(2,:,k),Ranges_Sats(1,:,k),InitPos);
   k = k + 1;
    pause(0.0001)
end