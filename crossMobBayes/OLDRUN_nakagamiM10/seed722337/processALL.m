clc
close all
clear all

%global folders sub_folders gi gj
% ii=1;
folders=dir;
for gi=1:size(folders,1)
    ii=1;
    if strcmp(folders(gi,1).name(1),'N') % enter the nakagami folder
        cd(folders(gi,1).name)
        sub_folders=dir;
        
        for gj=1:size(sub_folders,1)
            if strcmp(sub_folders(gj,1).name(1),'s') % enter the seeds folder
                cd(sub_folders(gj,1).name)                
                disp(['Working on ' folders(gi,1).name sub_folders(gj,1).name]);
                unix('cp ../../*.m .');                            
                tic
                main(); % execute the script for processing and testing 
                toc
                cd ..
                ii=ii+1;
            end  
        end
 %        if ii==11
 %           clear sub_folders
 %          cd ..
 %           cd ..
  %       end
        
    end
end
