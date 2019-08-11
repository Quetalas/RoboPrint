function [ pos ] = DecodeMDEK_coords2( data )
        
        x_pos = findstr(data,'x:');
        y_pos = findstr(data,'y:');
        z_pos = findstr(data,'z:');
        qf_pos = findstr(data,'qf');
        x = str2double(data(x_pos + 2:y_pos-1));
        y = str2double(data(y_pos + 2:z_pos-1));
        z = str2double(data(z_pos + 2:qf_pos-1));
        pos = [x;y;z];
end

