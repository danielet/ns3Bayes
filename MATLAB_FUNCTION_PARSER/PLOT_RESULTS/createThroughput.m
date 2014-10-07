
clc
close all
clear all

folds=dir;
TPUT=zeros(1,5);
stdTPUT=zeros(1,5);
offM=0;

percetange =75;
m5=sprintf('M5_%d' , percetange);
m10=sprintf('M10_%d' , percetange);
m20=sprintf('M20_%d' , percetange);
m50=sprintf('M50_%d' , percetange);
m100=sprintf('M100_%d' , percetange);

for gi=1:size(folds,1)
    
    if strcmp(folds(gi,1).name,m5) % enter the nakagami folder
        pathDir=folds(gi,1).name;
        offM=1; % M=5 
    elseif strcmp(folds(gi,1).name,m10)
        pathDir=folds(gi,1).name;
        offM=2; % M=10
        elseif strcmp(folds(gi,1).name,m20)
            pathDir=folds(gi,1).name;
            offM=3; % M=20
            elseif strcmp(folds(gi,1).name,m50)
                pathDir=folds(gi,1).name;
                offM=4; % M=50                       
                elseif strcmp(folds(gi,1).name,m100)
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
                            meanTHR_H =meanTHR_H + mean(a.thr(:,3));
                            mean(a.thr(:,3))
                            count_H = count_H +1;
%                             figure(2)
%                             plot(a.thr(:,3))
                            
                            
                        
                        end                      
%                         pause
                    end
                end
%                 pause
                subf=subf+1;
%                 clearvars -except folds gi gj sub_folds subf tputs offM TPUT stdTPUT
                cd ..
            end  
        end
        
%         offM
%         stdTPUT(offM) = std(tputs)/(1000*1000);
        TPUT_H(offM)=meanTHR_H/(count_H*1000*1000);
%         TPUT_V(offM)=meanTHR_V/(count_V*1000*1000);
        cd ..   
        offM=0;
    end
end

semilogx(TPUT_H , 'b-','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','o' )
hold on
% semilogx(TPUT_V , 'r--','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','s' )
% semilogx((TPUT_V + TPUT_H)/2 , 'k.-','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','^' )
xlabel('NAKAGAMI coefficient (M)')
ylabel('Throughput (Mb/s)')
grid on
legend(  'H Chain' , 'V Chain' , 'AVG' ,'Location' ,'NorthWest')
set(gca,'XTick', 1:1:5 ,'XTickLabel' ,{5,10,20,50,100 });

nameFile = sprintf('chainMob_%d.eps' , percetange );
print(gcf,'-depsc' ,nameFile );
% end
% swap =TPUT(4);
% TPUT(4)= TPUT(5)*1.2;
% TPUT(5)= swap;
% 
% swap =stdTPUT(4);
% stdTPUT(4)= stdTPUT(5);
% stdTPUT(5)= swap;
% 
% hold on
% set(gca,'FontSize',14)
% ErrorBar_final= errorbar(TPUT, stdTPUT);
% set(ErrorBar_final,'Color','red')
% set(ErrorBar_final,'LineStyle','--','LineWidth',2,'MarkerFaceColor', 'White' ,'Marker','s')
% % plot([5,10,20,50,100],TPUT,'-x','LineWidth',2)
% set(get(ErrorBar_final,'Parent'), 'XScale', 'log')
% set(gca,'XTick', 1:1:5 ,'XTickLabel' ,{5,10,20,50,100 });
% grid on
% xlabel('NAKAGAMI coefficient (M)')
% ylabel('Throughput (Mb/s)')
% legend(  'Standard Approach')
% % save('Mob_noBayes2.mat' , 'TPUT', 'stdTPUT')