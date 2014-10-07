clc
close all
clear all
ctrOTHER = 1;
% global folders sub_folders gi gj
% ii=1;
folders=dir;
PATH = pwd;
for gi=1:size(folders,1)
    ii=1;
%    if strcmp(folders(gi,1).name(1),'N') % enter the nakagami folder
   	ctrlDir = 0;     
        if strcmp(folders(gi,1).name,'NEWNOBAYESRUN_nakagamiM5') % enter the nakagami folder
            pathDir=folders(gi,1).name;
            offM=1; % M=5 
            varDir='MNOBAYE5';
            varDir= strcat(PATH,'/', varDir) ;
	ctrlDir=1;
            mkdir(varDir);
            elseif strcmp(folders(gi,1).name,'NEWNOBAYESRUN_nakagamiM10')
                pathDir=folders(gi,1).name;
                offM=2; % M=10
                varDir='MNOBAYE10' ;
                varDir= strcat(PATH,'/', varDir) ;
                mkdir(varDir);
		ctrlDir=1;
                elseif strcmp(folders(gi,1).name,'NEWNOBAYESRUN_nakagamiM20')
                    pathDir=folders(gi,1).name;
                    offM=3; % M=20
                    varDir='MNOBAYE20' ;
                    varDir= strcat(PATH,'/', varDir) ;
                    mkdir(varDir);
		ctrlDir=1;
                    elseif strcmp(folders(gi,1).name,'NEWNOBAYESRUN_nakagamiM50')
                        pathDir=folders(gi,1).name;
                        offM=4; % M=50                       
                        varDir='MNOBAYE50' ;
                        varDir= strcat(PATH,'/', varDir) ;
                        mkdir(varDir);
                
			ctrlDir=1;
		        elseif strcmp(folders(gi,1).name,'NEWNOBAYESRUN_nakagamiM100')
                            pathDir=folders(gi,1).name;
                            offM=5; % M=100
                            varDir='MNOBAYE100' ;
                            varDir= strcat(PATH,'/', varDir) ;
                            mkdir(varDir);
				ctrlDir=1;
				
     end
%        end
 if(ctrlDir == 1)       
        cd(folders(gi,1).name)
        sub_folders=dir;
        for gj=1:size(sub_folders,1)
            if strcmp(sub_folders(gj,1).name(1),'s') % enter the seeds folder
                cd(sub_folders(gj,1).name)                
                disp(['Working on ' folders(gi,1).name '/'  sub_folders(gj,1).name]);
                unix('cp ../../main_SHORTEST.m .');   
                unix('cp ../../processParametersCHAIN_SHORTEST.m .');                   
                unix('cp ../../checkAllFile.m .');                   
                nameEXP=sprintf('EXP_%d', ii);
                varDir = fullfile(varDir);
                mkdir(varDir, nameEXP);
                tic
                main_SHORTEST( varDir ,nameEXP ,ctrOTHER); % execute the script for processing and testing 
                toc
                cd ..
                ii=ii+1;
            end  
        end
        cd ..        
    end
end
end
