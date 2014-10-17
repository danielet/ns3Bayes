clc
close all
clear all

folds=dir;
TPUT=zeros(1,5);
stdTPUT=zeros(1,5);
ctrlMATRIX = 1;
Hop = 4;
for gi=1:size(folds,1)
    MATRIX=[];
    offM=0;
    if strcmp(folds(gi,1).name,'M5_LEARN_MOVE') % enter the nakagami folder
        pathDir=folds(gi,1).name;        
        nameMatrix='../MOBILE_RESULTS_CROSS/MATRIX_BN_5.mat';    
%     time=54.1:0.1:950;
        offM=1; % M=5 
    elseif strcmp(folds(gi,1).name,'M10_LEARN_MOVE')
        pathDir=folds(gi,1).name;
        offM=2; % M=10
        nameMatrix='../MOBILE_RESULTS_CROSS/MATRIX_BN_10.mat';
        elseif strcmp(folds(gi,1).name,'M20_LEARN_MOVE')
            pathDir=folds(gi,1).name;
            offM=3; % M=20
            nameMatrix='../MOBILE_RESULTS_CROSS/MATRIX_BN_20.mat';
            elseif strcmp(folds(gi,1).name,'M50_LEARN_MOVE')
                pathDir=folds(gi,1).name;
                offM=4; % M=50           
                nameMatrix='../MOBILE_RESULTS_CROSS/MATRIX_BN_50.mat';
                elseif strcmp(folds(gi,1).name,'M100_LEARN_MOVE')
                    pathDir=folds(gi,1).name;
                    offM=5; % M=100
                    nameMatrix='../MOBILE_RESULTS_CROSS/MATRIX_BN_100.mat';
    end

    if(offM > 0)
        offM
        cd(pathDir)
%         pathDir
        sub_folds=dir;
        tputs=zeros(1,size(sub_folds,1)-2);
        subf=1;
        meanTHR_H =0;
        count_H =0;
        meanTHR_V =0;
        count_V =0;
        ctrlEnter=0;
        timeStart=[];
        for gj=1:size(sub_folds,1)
            if strcmp(sub_folds(gj,1).name(1),'E') % enter the seeds folder
                cd(sub_folds(gj,1).name)       
                params=dir;
                sub_folds(gj,1).name               
                iter=1;
                while(iter <= size(params,1) )
                    if(length(params(iter,1).name)>3)
                        %HERE I WILL ENTER INSIDE THE EXP
                        ctrlEnter=0;
                        if ((strcmp(params(iter,1).name(1:6) , 'ChainH')==1))
                            exp_data=load(params(iter,1).name);
                        else
                            if ((strcmp(params(iter,1).name(1:6) , 'ChainV')==1))
                                exp_data_v=load(params(iter,1).name);
                            end
                        end                                            
                    end
                    iter = iter+1;
                end
%                 disp('QUI')
                countRow=0;

                
                timeStart = [timeStart,exp_data.thr(1,1) ,exp_data_v.thr(1,1) ];
                


                if(ctrlMATRIX == 1 )

                    switch offM
                    case 1
                        time=53.1:0.1:950;
                    case 2
                        time=54.1:0.1:950;
                    case 3
                        time=51.3:0.1:950;
                    case 4
                        time=50.1:0.1:950;
                    case 5
                        time=51.1:0.1:950;
                    end
                
                    


                    for ii=1:length(time)
                        %%
                        %NODE 1
                        NodeID=1;
                        countRow= countRow  + 1;

                        index = find(exp_data.thr(:,1) == time(ii));
                        if(~isempty(index))
                            index=index(1);
                            MATRIX_TMP(countRow,1)    =   exp_data.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data.MAC_TX1(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data.MAC_RETX1(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data.q0(index,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data.routing_tables_1(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data.cont_win1(index,2);

                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end

                                % @times = (70, 170, 
                                % 270 , 370, 
                                % 470, 570 , 
                                % 670, 770);
                        if(    (time(ii) >= 70 &&  time(ii) < 120) || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))
                                    MATRIX_TMP(countRow,8)    = 1;
                                    MATRIX_TMP(countRow,9)    = 0;
        %                                             mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
                        else
                                    MATRIX_TMP(countRow,8)    = 0;
                                    MATRIX_TMP(countRow,9)    = 0;
                        end   



                        %%
                        %NODE 2
                        countRow= countRow  + 1;
                        NodeID=NodeID       + 1;
                        if(~isempty(index))
                            MATRIX_TMP(countRow,1)    =   exp_data.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data.MAC_TX2(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data.MAC_RETX2(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data.q1(index,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data.routing_tables_2(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data.cont_win2(index,2);

                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end

                        if(    (time(ii) >= 70 && time(ii) < 120)  || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))

                                MATRIX_TMP(countRow,8)    = 1;

                                if((time(ii) >= 70 && time(ii) < 120)   || (time(ii) >= 170 && time(ii) < 220) ||...
                                   (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770  && time(ii) < 820))
                                    MATRIX_TMP(countRow,9)    = 1 ;
                                else
                                    MATRIX_TMP(countRow,9)    = 0 ;
                                end

                        else
                                MATRIX_TMP(countRow,8)    = 0;
                                MATRIX_TMP(countRow,9)    = 0;
                        end    

                        %%
                        %NODE 3
                        countRow= countRow  + 1;
                        NodeID=NodeID       + 1;
                        if(~isempty(index))
                            MATRIX_TMP(countRow,1)    =   exp_data.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data.MAC_TX3(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data.MAC_RETX3(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data.q2(ii,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data.routing_tables_3(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data.cont_win3(index,2);
                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end
                        if(    (time(ii) >= 70 && time(ii) < 120)  || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))
                                MATRIX_TMP(countRow,8)    = 1;

                                if((time(ii)>= 70 && ii <= 120) || (time(ii) >= 770 && time(ii) <= 820))
                                    MATRIX_TMP(countRow,9)    = 1;
                                else
                                    MATRIX_TMP(countRow,9)    = 0;
                                end
                        else
                                MATRIX_TMP(countRow,8)    = 0;
                                MATRIX_TMP(countRow,9)    = 0;
                        end    
                        %%
                        %NODE 4
                        countRow= countRow  + 1;
                        NodeID=NodeID       + 1;

                        if(~isempty(index))
                            MATRIX_TMP(countRow,1)    =   exp_data.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data.MAC_TX4(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data.MAC_RETX4(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data.q3(index,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data.routing_tables_4(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data.cont_win4(index,2);
                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end


                        if(    (time(ii) >= 70 && time(ii) < 120)  || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))
                                MATRIX_TMP(countRow,8)    = 1;
                                if((time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420 ) ||...
                                   (time(ii) >= 470 && time(ii) < 520) || (time(ii)  >= 570 && time(ii) < 620) )
                                    MATRIX_TMP(countRow,9)    = 1;
                                else
                                    MATRIX_TMP(countRow,9)    = 0 ;
                                end
    %                                             mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
                        else
                                MATRIX_TMP(countRow,8)    = 0;
                                MATRIX_TMP(countRow,9)    = 0;
                        end   



                        %%
                        %NODE 5   

                        countRow= countRow  + 1;
                        NodeID=NodeID       + 1;
                        if(~isempty(index))
                            MATRIX_TMP(countRow,1)    =   exp_data.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data.MAC_TX5(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data.MAC_RETX5(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data.q4(index,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data.routing_tables_5(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data.cont_win5(index,2);
                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end
                        if(    (time(ii) >= 70 && time(ii) < 120)  || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))
                                MATRIX_TMP(countRow,8)    = 1;
                                MATRIX_TMP(countRow,9)    = 0;
    %                                             mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
                        else
                                MATRIX_TMP(countRow,8)    = 0;
                                MATRIX_TMP(countRow,9)    = 0;
                        end   



    %%
    %START VERTICAL CHAIN


                        %%
                        %NODE 6   

                        countRow= countRow  + 1;
                        NodeID=NodeID       + 1;
                        index = find(exp_data_v.thr(:,1) == time(ii));
                        if(~isempty(index))
                            MATRIX_TMP(countRow,1)    =   exp_data_v.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data_v.MAC_TX6(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data_v.MAC_RETX6(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data_v.q5(index,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data_v.routing_tables_6(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data_v.cont_win6(index,2);
                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end
                        if(    (time(ii) >= 70 && time(ii) < 120)  || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))
                                MATRIX_TMP(countRow,8)    = 1;
                                MATRIX_TMP(countRow,9)    = 0;
    %                                             mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
                        else
                                MATRIX_TMP(countRow,8)    = 0;
                                MATRIX_TMP(countRow,9)    = 0;
                        end   

                        %%
                        %NODE 7   

                        countRow= countRow  + 1;
                        NodeID=NodeID       + 1;
                        index = find(exp_data_v.thr(:,1) == time(ii));
                        if(~isempty(index))
                            MATRIX_TMP(countRow,1)    =   exp_data_v.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data_v.MAC_TX7(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data_v.MAC_RETX7(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data_v.q6(index,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data_v.routing_tables_7(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data_v.cont_win7(index,2);

                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end


                        if(    (time(ii) >= 70 && time(ii) < 120)  || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))
                                MATRIX_TMP(countRow,8)    = 1;
                                if((time(ii) >= 170 && time(ii) < 220) || (time(ii) >= 270  && time(ii) < 320) || ...
                                   (time(ii) >= 570 && time(ii) < 620) || (time(ii) >= 670  && time(ii) <= 720))
                                MATRIX_TMP(countRow,9)    = 1;
                                else
                                    MATRIX_TMP(countRow,9)    = 0 ;
                                end

    %                                             mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
                        else
                                MATRIX_TMP(countRow,8)    = 0;
                                MATRIX_TMP(countRow,9)    = 0;
                        end   


                        %%
                        %NODE 8   

                        countRow= countRow  + 1;
                        NodeID=NodeID       + 1;
                        index = find(exp_data_v.thr(:,1) == time(ii));
                        if(~isempty(index))
                            MATRIX_TMP(countRow,1)    =   exp_data_v.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data_v.MAC_TX8(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data_v.MAC_RETX8(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data_v.q7(index,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data_v.routing_tables_8(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data_v.cont_win8(index,2);
                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end


                        if(    (time(ii) >= 70 && time(ii) < 120)  || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))
                                MATRIX_TMP(countRow,8)    = 1;
                                if((time(ii) >= 370 && time(ii) < 420) || (time(ii) >= 470 && time(ii) <= 520))
                                    MATRIX_TMP(countRow,9)    = 1;
                                else
                                    MATRIX_TMP(countRow,9)    = 0 ;
                                end
    %                                             mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
                        else
                                MATRIX_TMP(countRow,8)    = 0;
                                MATRIX_TMP(countRow,9)    = 0;
                        end   

                        %%
                        %NODE 9   

                        countRow= countRow  + 1;
                        NodeID=NodeID       + 1;
                        index = find(exp_data_v.thr(:,1) == time(ii));
                        if(~isempty(index))
                            MATRIX_TMP(countRow,1)    =   exp_data_v.thr(index,1);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   exp_data_v.MAC_TX9(index,2);
                            MATRIX_TMP(countRow,4)    =   exp_data_v.MAC_RETX9(index,2);
                            MATRIX_TMP(countRow,5)    =   exp_data_v.q8(index,2);
                            MATRIX_TMP(countRow,6)    =   Hop - exp_data_v.routing_tables_8(index,2);
                            MATRIX_TMP(countRow,7)    =   exp_data_v.cont_win8(index,2);
                        else
                            MATRIX_TMP(countRow,1)    =   time(ii);
                            MATRIX_TMP(countRow,3)    =   NodeID;
                            MATRIX_TMP(countRow,3)    =   0;
                            MATRIX_TMP(countRow,4)    =   0;
                            MATRIX_TMP(countRow,5)    =   0;
                            MATRIX_TMP(countRow,6)    =   0;
                            MATRIX_TMP(countRow,7)    =   0;
                        end
                        if(    (time(ii) >= 70 && time(ii) < 120)  || (time(ii) >= 170 && time(ii) < 220) || ...
                               (time(ii) >= 270 && time(ii) < 320) || (time(ii) >= 370 && time(ii) < 420) || ...
                               (time(ii) >= 470 && time(ii) < 520) || (time(ii) >= 570 && time(ii) < 620) || ...
                               (time(ii) >= 670 && time(ii) < 720) || (time(ii) >= 770 && time(ii) < 820))
                                MATRIX_TMP(countRow,8)    = 1;
                                MATRIX_TMP(countRow,9)    = 0;
    %                                             mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
                        else
                                MATRIX_TMP(countRow,8)    = 0;
                                MATRIX_TMP(countRow,9)    = 0;
                        end   

                        ctrlEnter=1;                                       
                    end
                    MATRIX=[MATRIX ; MATRIX_TMP];                                                           
                end                
                cd ..
            end  
%             timeStart
%             pause

            if(ctrlEnter==1)
                save(nameMatrix,'MATRIX');  
                ctrlEnter = 0;
            end
              
        end
%         timeStart
%         max(timeStart)
        disp('ESCO')
        cd ..   
    end
end
% end


