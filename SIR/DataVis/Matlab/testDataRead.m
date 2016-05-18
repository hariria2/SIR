tic
timestamp = '8-3-2015';
dataFolder = ['../../Data/data_multi_v','1','_',timestamp];

directoryfiles = dir(['../Data/', dataFolder]);
files = cell(1, length(directoryfiles)-2);
% for ii = 1:length(files)
%     files{ii} = directoryfiles(ii + 2).name;
% end
% loops  = length(files);
% 
% for ii = 1:loops
%     people = readtable([movieFolder,'/',files{ii}]);
% end   
%  

data = readtable('/Users/sahand/Research/SIR/Data/all.dat');
toc