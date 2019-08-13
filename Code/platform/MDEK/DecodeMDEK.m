function [R,Sats] = DecodeMDEK( data )
%Summary of this function goes here
%   Detailed explanation goes here
% 
% DW1 = 45AE
% DW2 = 9318
% DW3 = CD12
% DW4 = D4BB
% DW5 = 0820
% DW6 = D31F
% DW7 = 08AD
% DW8 = CA9A
% DW9 = C630
% DW10 = 8187

        A = findstr(data,'[');
        B = findstr(data,']=');
        C = findstr(data,' ');
        N1 = length(A);
        N2 = length(B);
%         if N1 == N2
            N = N2;
            Sats = zeros(3,N);
            R = zeros(N,1);
            for i = 1:N
%                 Sats(i) = whichone(data((A(i)-4):(A(i)-1)));
                Sats(:,i) = str2num(data(A(i):B(i)));
                R(i) = str2num(data((B(i)+2):C(i)));
            end       
%         else
%             Sats = zeros(N1,1);
%             R = zeros(N1,1);
%         end
        
       
                
    
end

function [ n ] = whichone( str )
switch str
    case '45AE'
        n = 1;
    case '9318'
        n = 2;
    case 'CD12'
        n = 3;
    case 'D4BB'
        n = 4;
    case '0820'
        n = 5;
    case 'D31F'
        n = 6;
    case '08AD'
        n = 7;
    case 'CA9A'
        n = 8;
    case 'C630'
        n = 9;
    case '8187'
        n = 10;
    otherwise
        n = 0;
end
end

