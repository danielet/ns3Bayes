clc
close all
clear all
ctrOTHER = 1;
% global folders sub_folders gi gj
% ii=1;
folders=dir;
PATH = pwd;

percetange =75;

m5=sprintf('NEWBAYESRUN_nakagamiM5PERC%d' , percetange);
m10=sprintf('NEWBAYESRUN_nakagamiM10PERC%d' , percetange);
m20=sprintf('NEWBAYESRUN_nakagamiM20PERC%d' , percetange);
m50=sprintf('NEWBAYESRUN_nakagamiM50PERC%d' , percetange);
m100=sprintf('NEWBAYESRUN_nakagamiM100PERC%d' , percetange);
for gi=1:size(folders,1)
    ii=1;
    
        correctFolder =0;
        if strcmp(folders(gi,1).name, m5) % enter the nakagami folder
            pathDir=folders(gi,1).name;
            offM=1; % M=5 
            varDir=sprintf('M5_%d' , percetange);
            varDir= strcat(PATH,'/', varDir) ;
            correctFolder =1;
            mkdir(varDir);
            elseif strcmp(folders(gi,1).name,m10)
                pathDir=folders(gi,1).name;
                offM=2; % M=10
                varDir=sprintf('M10_%d' , percetange);
                varDir= strcat(PATH,'/', varDir) ;
                correctFolder =1;
                mkdir(varDir);
                elseif strcmp(folders(gi,1).name,m20)
                    pathDir=folders(gi,1).name;
                    offM=3; % M=20
                    varDir=sprintf('M20_%d' , percetange);
                    varDir= strcat(PATH,'/', varDir) ;
                    correctFolder =1;
                    mkdir(varDir);
                    elseif strcmp(folders(gi,1).name,m50)
                        pathDir=folders(gi,1).name;
                        offM=4; % M=50                       
                        varDir=sprintf('M50_%d' , percetange);
                        varDir= strcat(PATH,'/', varDir) ;
                        correctFolder =1;
                        mkdir(varDir);
                        elseif strcmp(folders(gi,1).name,m100)
                            pathDir=folders(gi,1).name;
                            offM=5; % M=100
                            varDir=sprintf('M100_%d' , percetange);
                            varDir= strcat(PATH,'/', varDir) ;
                            correctFolder =1;
                            mkdir(varDir);
%     end
        end
        if(correctFolder ==1)
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
