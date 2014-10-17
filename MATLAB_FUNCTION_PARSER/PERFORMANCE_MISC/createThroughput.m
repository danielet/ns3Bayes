clc
close all
clear all

folds=dir;
TPUT=zeros(1,5);
stdTPUT=zeros(1,5);
offM=0;
percentage=90;
% M5 = sprintf('M5_BAYES_MOVE_%d', percentage);
% M10 = sprintf('M10_BAYES_MOVE_%d', percentage);
% M20 = sprintf('M20_BAYES_MOVE_%d', percentage);
% M50 = sprintf('M50_BAYES_MOVE_%d', percentage);
% M100 = sprintf('M100_BAYES_MOVE_%d', percentage);

% M5 = sprintf('M5_NOMOVE_NOBAYES_%d', percentage);
% M10 = sprintf('M10_NOMOVE_NOBAYES_%d', percentage);
% M20 = sprintf('M20_NOMOVE_NOBAYES_%d', percentage);
% M50 = sprintf('M50_NOMOVE_NOBAYES_%d', percentage);
% M100 = sprintf('M100_NOMOVE_NOBAYES_%d', percentage);

M5 = sprintf('M5_LEARN_MOVE');
M10 = sprintf('M10_LEARN_MOVE');
M20 = sprintf('M20_LEARN_MOVE');
M50 = sprintf('M50_LEARN_MOVE');
M100 = sprintf('M100_LEARN_MOVE');


% M5 = sprintf('M5_BAYES_%d', percentage);
% M10 = sprintf('M10_BAYES_%d', percentage);
% M20 = sprintf('M20_BAYES_%d', percentage);
% M50 = sprintf('M50_BAYES_%d', percentage);
% M100 = sprintf('M100_BAYES_%d', percentage);

% M5 = sprintf('M5_NOBAYES_%d', percentage);
% M10 = sprintf('M10_NOBAYES_%d', percentage);
% M20 = sprintf('M20_NOBAYES_%d', percentage);
% M50 = sprintf('M50_NOBAYES_%d', percentage);
% M100 = sprintf('M100_NOBAYES_%d', percentage);

for gi=1:size(folds,1)
    
    if strcmp(folds(gi,1).name, M5) % enter the nakagami folder
        pathDir=folds(gi,1).name;
        offM=1; % M=5 
    elseif strcmp(folds(gi,1).name,M10)
        pathDir=folds(gi,1).name;
        offM=2; % M=10
        elseif strcmp(folds(gi,1).name,M20)
            pathDir=folds(gi,1).name;
            offM=3; % M=20
            elseif strcmp(folds(gi,1).name,M50)
                pathDir=folds(gi,1).name;
                offM=4; % M=50                       
                elseif strcmp(folds(gi,1).name,M100)
                    pathDir=folds(gi,1).name;
                    offM=5; % M=100
%     end
    end

    if(offM > 0)
        cd(pathDir)
        pathDir
        sub_folds=dir;
        tputs=zeros(1,size(sub_folds,1)-2);
        subf=1;
        meanTHR_H =0;
        count_H =0;
        meanTHR_V =0;
        count_V =0;
        
        for gj=1:size(sub_folds,1)
            if strcmp(sub_folds(gj,1).name(1),'E') % enter the seeds folder
                cd(sub_folds(gj,1).name)       
%                     sub_folds(gj,1).name
%                     folds(gi,1).name
%                 disp(['Working on ' folds(gi,1).name sub_folds(gj,1).names]);

                params=dir;
                sub_folds(gj,1).name
                for iter=1:size(params,1)                                                          
                    if(length(params(iter,1).name)>3)
                        if ((strcmp(params(iter,1).name(1:6) , 'ChainH')==1))                            
                            a=load(params(iter,1).name);
                            tputs(subf)=mean(a.thr(:,3));
                            figure(1)
                            subplot(2,1,1)
                            plot(a.thr(:,3), 'b-')
%                             hold on
                            index= find(a.thr(:,3) < 0);
                            a.thr(index,3)= 0;
                            meanTHR_H =meanTHR_H + (mean(a.thr(:,3)))
                            mean(a.thr(:,3))
                            count_H = count_H +1;
%                             figure(2)
%                             plot(a.thr(:,3))
                            
                            
                        else
                            if ((strcmp(params(iter,1).name(1:6) , 'ChainV')==1))
                                a=load(params(iter,1).name);
%                                 tputs(subf)=mean(a.thr(:,3));
                                subplot(2,1,2)
                                plot(a.thr(:,3) , 'r')
%                                 hold off
                                index= find(a.thr(:,3) < 0);
                                a.thr(index,3)= 0;
                                meanTHR_V = meanTHR_V + mean(a.thr(:,3))
                                count_V = count_V +1;
%                                 pause
%                                 figure(3)
%                                 plot(a.thr(:,3))
                            end
                        end                      
%                         pause

                        
                    end
                end  
                    
                                                        
                
                subf=subf+1;
%                 clearvars -except folds gi gj sub_folds subf tputs offM TPUT stdTPUT
                cd ..
            end 
            
        end
%         pause
%         offM
%         stdTPUT(offM) = std(tputs)/(1000*1000);
        TPUT_H(offM)=meanTHR_H/(count_H*1000*1000);       
        TPUT_V(offM)=meanTHR_V/(count_V*1000*1000);
        cd ..   
        offM=0;
    end
end
figure(2)
% semilogx(TPUT_H , 'b-','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','o' )
hold on
% semilogx(TPUT_V , 'r--','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','s' )
semilogx((TPUT_V + TPUT_H)/2 , 'k.-','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','s' )
xlabel('NAKAGAMI coefficient (M)')
ylabel('Throughput (Mb/s)')
grid on
% legend(  'H Chain' , 'V Chain' , 'AVG' ,'Location' ,'NorthWest')
legend( 'AVG' ,'Location' ,'NorthWest')
set(gca,'XTick', 1:1:5 ,'XTickLabel' ,{5,10,20,50,100 });
title('RTO')

nameFig = sprintf('Move_Bayes_perc%d_2', percentage);

print(gcf,'-depsc' , nameFig);
