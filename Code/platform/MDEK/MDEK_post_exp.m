
clear all
close all
clc

load('apg_log_1.mat')
Post_1 = navidata;
Post_1(1:5) = [];

load('les_log_1.mat')
Post_2 = navidata;
Post_2(1:2) = [];

Initial_Position = [2.013; 2.007; 1.101];

for k = 1:length(Post_2)
    PreDecodeData = char(Post_2(k,1));
    [Ranges, Anchors_Positions] = DecodeMDEK(PreDecodeData);   
    R(:,k) = Ranges(1:3);
    [Coordinates] = NavSolver_D(Anchors_Positions, Ranges, Initial_Position);
    UserCoordinates(:,k) = Coordinates;
end

for k = 1:length(Post_1)
    UP(:,k) = DecodeMDEK_coords2( Post_1{k} );
end

