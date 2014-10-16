clc
close all
clear all
ctrOTHER = 1;
% global folders sub_folders gi gj
% ii=1;
folders=dir;
PATH = pwd;
percentage = 90;



M5 = sprintf('NEW_MOVEnakagamiM5');
M10 = sprintf('NEW_MOVEnakagamiM10');
M20 = sprintf('NEW_MOVEnakagamiM20');
M50 = sprintf('NEW_MOVEnakagamiM50');
M100 = sprintf('NEW_MOVEnakagamiM100');

% M5 = sprintf('NEW_MOVE_nakagamiM5BAYES_PERC_%d', percentage);
% M10 = sprintf('NEW_MOVE_nakagamiM10BAYES_PERC_%d', percentage);
% M20 = sprintf('NEW_MOVE_nakagamiM20BAYES_PERC_%d', percentage);
% M50 = sprintf('NEW_MOVE_nakagamiM50BAYES_PERC_%d', percentage);
% M100 = sprintf('NEW_MOVE_nakagamiM100BAYES_PERC_%d', percentage);

% M5 = sprintf('NEWNOBAYESRUN_nakagamiM5');
% M10 = sprintf('NEWNOBAYESRUN_nakagamiM10');
% M20 = sprintf('NEWNOBAYESRUN_nakagamiM20');
% M50 = sprintf('NEWNOBAYESRUN_nakagamiM50');
% M100 = sprintf('NEWNOBAYESRUN_nakagamiM100');


% M5 = sprintf('NEW_NOMOVEnakagamiM5');
% M10 = sprintf('NEW_NOMOVEnakagamiM10');
% M20 = sprintf('NEW_NOMOVEnakagamiM20');
% M50 = sprintf('NEW_NOMOVEnakagamiM50');
% M100 = sprintf('NEW_NOMOVEnakagamiM100');

% M5_SAVE     ='M5_NOMOVE_NOBAYES_%d';
% M10_SAVE    ='M10_NOMOVE_NOBAYES_%d';
% M20_SAVE    ='M20_NOMOVE_NOBAYES_%d';
% M50_SAVE    ='M50_NOMOVE_NOBAYES_%d';
% M100_SAVE   ='M100_NOMOVE_NOBAYES_%d';

M5_SAVE     ='M5_BAYES_%d';
M10_SAVE    ='M10_BAYES_%d';
M20_SAVE    ='M20_BAYES_%d';
M50_SAVE    ='M50_BAYES_%d';
M100_SAVE   ='M100_BAYES_%d';

% M5_SAVE     ='M5_NOBAYES_%d';
% M10_SAVE    ='M10_NOBAYES_%d';
% M20_SAVE    ='M20_NOBAYES_%d';
% M50_SAVE    ='M50_NOBAYES_%d';
% M100_SAVE   ='M100_NOBAYES_%d';

for gi=1:size(folders,1)
    ii=1;
    correctFolder = 0;
        
    if strcmp(folders(gi,1).name,M5) % enter the nakagami folder
        pathDir=folders(gi,1).name;
        offM=1; % M=5 
%         varDir=sprintf('M5_BAYES_MOVE_%d' , percentage);
        varDir=sprintf(M5_SAVE , percentage);
        varDir= strcat(PATH,'/', varDir) ;
        mkdir(varDir);
	correctFolder=1;
        elseif strcmp(folders(gi,1).name,M10)
            pathDir=folders(gi,1).name;
            offM=2; % M=10
%             varDir=sprintf('M10_BAYES_MOVE_%d' , percentage);
            varDir=sprintf(M10_SAVE , percentage);
            varDir= strcat(PATH,'/', varDir) ;
            mkdir(varDir);
		correctFolder=1;
            elseif strcmp(folders(gi,1).name,M20)
                pathDir=folders(gi,1).name;
                offM=3; % M=20
                varDir=sprintf(M20_SAVE , percentage);
%                 varDir=sprintf('M20_BAYES_MOVE_%d' , percentage);
                varDir= strcat(PATH,'/', varDir) ;
		correctFolder=1;
		mkdir(varDir);
                elseif strcmp(folders(gi,1).name,M50)
                    pathDir=folders(gi,1).name;
                    offM=4; % M=50                       
%                     varDir=sprintf('M50_BAYES_MOVE_%d' , percentage);
                    varDir=sprintf(M50_SAVE , percentage);
                    varDir= strcat(PATH,'/', varDir) ;
                    mkdir(varDir);
			correctFolder=1;
		elseif strcmp(folders(gi,1).name,M100)
                        pathDir=folders(gi,1).name;
                        offM=5; % M=100
                        varDir=sprintf(M100_SAVE , percentage);
%                         varDir=sprintf('M100_BAYES_MOVE_%d' , percentage);
                        varDir= strcat(PATH,'/', varDir) ;
                        mkdir(varDir);
			correctFolder=1;
%     end
    end
    
    if(correctFolder ==1)
        cd(folders(gi,1).name)
        sub_folders=dir;
        for gj=1:size(sub_folders,1)
            if strcmp(sub_folders(gj,1).name(1),'s') % enter the seeds folder
                cd(sub_folders(gj,1).name)                
                disp(['Working on ' folders(gi,1).name '/'  sub_folders(gj,1).name]);
                unix('cp ../../mainMirror.m .');   
                unix('cp ../../checkAllFile.m .');   
                unix('cp ../../processParametersCHAIN1_MIRROR.m .');   
                unix('cp ../../processParametersCHAIN2_MIRROR.m .');   
                nameEXP=sprintf('EXP_%d', ii);
                varDir = fullfile(varDir);
                mkdir(varDir, nameEXP);
                tic
                mainMirror( varDir ,nameEXP ,ctrOTHER); % execute the script for processing and testing 
                toc
                cd ..
                ii=ii+1;
            end  
        end
        cd ..        
    end
end
