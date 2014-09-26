%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% THROUGHPUT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function processParametersCHAIN_SHORTEST(startTime , simuTime, sample,varDir ,nameEXP , ctrOTHER )

    datetime=datestr(now);
    datetime=strrep(datetime,':','_'); %Replace colon with underscore
    datetime=strrep(datetime,'-','_'); %Replace minus sign with underscore
    datetime=strrep(datetime,' ','_'); %Replace space with underscore
    datetime = strcat('ChainH_',datetime);


    load('ACK_received_by_node_0.txt')

    ACK_received_by_node_0 = [ACK_received_by_node_0([2:length(ACK_received_by_node_0)],1),ACK_received_by_node_0([2:length(ACK_received_by_node_0)],2)];




    ACK_received_by_node_0(:,1) = round(ACK_received_by_node_0(:,1)*10)/10;
    trg = startTime;
    thr(1,1) = startTime;
    thr(1,2) = 0;
    thr(1,3) = 0;
    counter = 2;
    num_dig = 1;



    while trg < ACK_received_by_node_0(length(ACK_received_by_node_0),1) 
        thr(counter, 1) = trg;
        index = find(ACK_received_by_node_0(:,1) == trg);
        if ~isempty(index)
            thr(counter, 2) = ACK_received_by_node_0(index(1),2);
        else
            if(counter == 1)
                thr(counter, 2) = 0;
            else
                thr(counter, 2) = thr(counter-1, 2);
            end
        end
        thr(counter,3) = ((thr(counter,2) - thr(counter-1,2))*8)/sample; 
        counter = counter + 1;
        trg = trg + sample;
        trg = round(trg*(10^num_dig))/(10^num_dig);
    end

    mobility_vectorH = zeros(length(thr(:,1)),2);
    mobility_vectorH(:,1) = thr(:,1);
    N=100;
    for ii=0:(50-1)
        if((ii >= 1 && ii <=5) || (ii >= 9 && ii <= 13) ||(ii >= 17 && ii <= 21)|| (ii >= 25 && ii <= 29) || (ii >= 33 && ii <= 37) || (ii >= 41 && ii <= 45) )
            mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
        end    
    end

    wheresave=strcat(varDir,'/',nameEXP,'/');
    datetime = strcat(wheresave,datetime);
    if(ctrOTHER ==1)

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % ROUTING TABLES
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        load routing_tables_1.txt
        load routing_tables_2.txt
        load routing_tables_3.txt
        load routing_tables_4.txt       
        

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % CNG WIN
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        load 'CongestionWindow_node_0.txt'

%         for i = 1:length(CongestionWindow_node_0)
%             num2str(CongestionWindow_node_0(i,1),'%.1f');
%             CongestionWindow_node_0(i,1) = str2num(ans);
%         end
        CongestionWindow_node_0(:,1) = round(CongestionWindow_node_0(:,1)*10)/10;
        trg = startTime;
        counter = 1;
        num_dig = 1;

        while trg < CongestionWindow_node_0(length(CongestionWindow_node_0),1) 
            cong_win(counter, 1) = trg;
            index = find(CongestionWindow_node_0(:,1) == trg);
            if ~isempty(index)
               cong_win(counter, 2) = CongestionWindow_node_0(index(1),2);
            else
               if(counter == 1)
                   cong_win(counter, 2) = CongestionWindow_node_0(1,2);

               else
                   cong_win(counter, 2) = cong_win(counter-1, 2);
               end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        % trg = startTime;
        % counter = 1;
        % num_dig = 1;




        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % MacQUEUE_size
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        load 'MAC_Queue_0.txt'
        load 'MAC_Queue_1.txt'
        load 'MAC_Queue_2.txt'
        load 'MAC_Queue_3.txt'       
        MAC_Queue_0(:,1) = round(MAC_Queue_0(:,1)*10)/10;
        MAC_Queue_1(:,1) = round(MAC_Queue_1(:,1)*10)/10;
        MAC_Queue_2(:,1) = round(MAC_Queue_2(:,1)*10)/10;
        MAC_Queue_3(:,1) = round(MAC_Queue_3(:,1)*10)/10;



        

        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 0 %%%%
        while trg < MAC_Queue_0(length(MAC_Queue_0),1) 
            q0(counter, 1) = trg;
            index =find(MAC_Queue_0(:,1) == trg);
            if ~isempty(index)
               q0(counter, 2) = MAC_Queue_0(index(1),2);
            else
                if(counter ==1)
                    q0(counter, 2) = MAC_Queue_0(1,2);
                else
                   q0(counter, 2) = q0(counter-1, 2);
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end


        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 1 %%%%
        while trg < MAC_Queue_1(length(MAC_Queue_1),1) 
            q1(counter, 1) = trg;
            index =find(MAC_Queue_1(:,1) == trg);
            if ~isempty(index)
               q1(counter, 2) = MAC_Queue_1(index(1),2);
            else
               if(counter ==1)
                    q1(counter, 2) = MAC_Queue_1(1,2);
                else
                   q1(counter, 2) = q1(counter-1, 2);
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 2 %%%%
        while trg < MAC_Queue_2(length(MAC_Queue_2),1) 
            q2(counter, 1) = trg;
            index =find(MAC_Queue_2(:,1) == trg);
            if ~isempty(find(index))
               q2(counter, 2) = MAC_Queue_2(index(1),2);
            else
               if(counter ==1)
                    q2(counter, 2) = MAC_Queue_2(1,2);
                else
                   q2(counter, 2) = q2(counter-1, 2);
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 3 %%%%
        while trg < MAC_Queue_3(length(MAC_Queue_3),1) 
            q3(counter, 1) = trg;
            index =find(MAC_Queue_3(:,1) == trg);
            if ~isempty(index)
               q3(counter, 2) = MAC_Queue_3(index(1),2);
            else
               if(counter ==1)
                    q3(counter, 2) = MAC_Queue_3(1,2);
                else
                   q3(counter, 2) = q3(counter-1, 2);
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        trg = startTime;
        counter = 1;
        num_dig = 1;



        


        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % CNTWIN
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        load 'ContentionWindow_node_0.txt'
        load 'ContentionWindow_node_1.txt'
        load 'ContentionWindow_node_2.txt'
        load 'ContentionWindow_node_3.txt'       
        
        ContentionWindow_node_0(:,1) = round(ContentionWindow_node_0(:,1)*10)/10;
        ContentionWindow_node_1(:,1) = round(ContentionWindow_node_1(:,1)*10)/10;
        ContentionWindow_node_2(:,1) = round(ContentionWindow_node_2(:,1)*10)/10;
        ContentionWindow_node_3(:,1) = round(ContentionWindow_node_3(:,1)*10)/10;




        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 1 %%%%
        while trg < ContentionWindow_node_0(length(ContentionWindow_node_0),1) 
            cont_win1(counter, 1) = trg;
            index =find(ContentionWindow_node_0(:,1) == trg);
            if ~isempty(index)
               cont_win1(counter, 2)    = ContentionWindow_node_0(index(1),2);
               cont_win1_p(counter , 2) = sum(ContentionWindow_node_0(index,2))/length(index);
               cont_win1_e(counter , 2) = sum(ContentionWindow_node_0(index,2));
               cont_win1_m(counter , 2) = mean(ContentionWindow_node_0(index,2));
               cont_win1_s(counter , 2) = std(ContentionWindow_node_0(index,2));
               
            else
                if(counter==1)
                    cont_win1(counter, 2) = ContentionWindow_node_0(1,2);
                    cont_win1_p(counter , 2) = 15;
                    cont_win1_e(counter , 2) = 15;
                    cont_win1_m(counter , 2) = 15;
                    cont_win1_s(counter , 2) = 15;
                else
                    cont_win1(counter, 2) = cont_win1(counter-1, 2);
                    cont_win1_p(counter , 2) = 15;
                    cont_win1_e(counter , 2) = 15;
                    cont_win1_m(counter , 2) = 15;
                    cont_win1_s(counter , 2) = 15;
                    
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 2 %%%%
        while trg < ContentionWindow_node_1(length(ContentionWindow_node_1),1) 
            cont_win2(counter, 1) = trg;
            index =find(ContentionWindow_node_1(:,1) == trg);
            if ~isempty(index)
               cont_win2(counter, 2) = ContentionWindow_node_1(index(1),2);
               cont_win2_p(counter , 2) = sum(ContentionWindow_node_1(index,2))/length(index);
               cont_win2_e(counter , 2) = sum(ContentionWindow_node_1(index,2));
               cont_win2_m(counter , 2) = mean(ContentionWindow_node_1(index,2));
               cont_win2_s(counter , 2) = std(ContentionWindow_node_1(index,2));
            else
                if(counter==1)
                    cont_win2(counter, 2) = ContentionWindow_node_1(1,2);
                    cont_win2_p(counter , 2) = 15;
                    cont_win2_e(counter , 2) = 15;
                    cont_win2_m(counter , 2) = 15;
                    cont_win2_s(counter , 2) = 15;
                else
                    cont_win2(counter, 2) = cont_win2(counter-1, 2);
                    cont_win2_p(counter , 2) = 15;
                    cont_win2_e(counter , 2) = 15;
                    cont_win2_m(counter , 2) = 15;
                    cont_win2_s(counter , 2) = 15;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 3 %%%%
        while trg < ContentionWindow_node_2(length(ContentionWindow_node_2),1) 
            cont_win3(counter, 1) = trg;
            index =find(ContentionWindow_node_2(:,1) == trg);
            if ~isempty(index)
               cont_win3(counter, 2) = ContentionWindow_node_2(index(1),2);
               cont_win3_p(counter , 2) = sum(ContentionWindow_node_2(index,2))/length(index);
               cont_win3_e(counter , 2) = sum(ContentionWindow_node_2(index,2));
               cont_win3_m(counter , 2) = mean(ContentionWindow_node_2(index,2));
               cont_win3_s(counter , 2) = std(ContentionWindow_node_2(index,2));
            else
                if(counter==1)
                    cont_win3(counter, 2) = ContentionWindow_node_2(1,2);
                    cont_win3_p(counter , 2) = 15;
                    cont_win3_e(counter , 2) = 15;
                    cont_win3_m(counter , 2) = 15;
                    cont_win3_s(counter , 2) = 15;
                else
                    cont_win3(counter, 2) = cont_win3(counter-1, 2);
                    cont_win3_p(counter , 2) = 15;
                    cont_win3_e(counter , 2) = 15;
                    cont_win3_m(counter , 2) = 15;
                    cont_win3_s(counter , 2) = 15;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 4 %%%%
        while trg < ContentionWindow_node_3(length(ContentionWindow_node_3),1) 
            cont_win4(counter, 1) = trg;
            index =find(ContentionWindow_node_3(:,1) == trg);
            if ~isempty(index)
               cont_win4(counter, 2) = ContentionWindow_node_3(index(1),2);
               cont_win4_p(counter , 2) = sum(ContentionWindow_node_3(index,2))/length(index);
               cont_win4_e(counter , 2) = sum(ContentionWindow_node_3(index,2));
               cont_win4_m(counter , 2) = mean(ContentionWindow_node_3(index,2));
               cont_win4_s(counter , 2) = std(ContentionWindow_node_3(index,2));
            else
               if(counter==1)
                    cont_win4(counter, 2) = ContentionWindow_node_3(1,2);
                    cont_win4_p(counter , 2) = 15;
                    cont_win4_e(counter , 2) = 15;
                    cont_win4_m(counter , 2) = 15;
                    cont_win4_s(counter , 2) = 15;
                else
                    cont_win4(counter, 2) = cont_win4(counter-1, 2);
                    cont_win4_p(counter , 2) = 15;
                    cont_win4_e(counter , 2) = 15;
                    cont_win4_m(counter , 2) = 15;
                    cont_win4_s(counter , 2) = 15;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

       

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % MAC TX
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        load 'Node_0_MAC_TX.txt'
        load 'Node_1_MAC_TX.txt'
        load 'Node_2_MAC_TX.txt'
        load 'Node_3_MAC_TX.txt'       
       
        Node_0_MAC_TX(:,1) = round(Node_0_MAC_TX(:,1)*10)/10;
        Node_1_MAC_TX(:,1) = round(Node_1_MAC_TX(:,1)*10)/10;
        Node_2_MAC_TX(:,1) = round(Node_2_MAC_TX(:,1)*10)/10;
        Node_3_MAC_TX(:,1) = round(Node_3_MAC_TX(:,1)*10)/10;


        compare = [startTime:sample:simuTime];

        for i = 1:length(compare)
            MAC_TX1(i,1) = compare(i);
            if ~isempty(find(Node_0_MAC_TX(:,1) == compare(i)))
                MAC_TX1(i,2) = length(find(Node_0_MAC_TX(:,1) == compare(i)));
            else
                MAC_TX1(i,2) = 0;
            end
        end

        for i = 1:length(compare)
            MAC_TX2(i,1) = compare(i);
            if ~isempty(find(Node_1_MAC_TX(:,1) == compare(i)))
                MAC_TX2(i,2) = length(find(Node_1_MAC_TX(:,1) == compare(i)));
            else
                MAC_TX2(i,2) = 0;
            end
        end

        for i = 1:length(compare)
            MAC_TX3(i,1) = compare(i);
            if ~isempty(find(Node_2_MAC_TX(:,1) == compare(i)))
                MAC_TX3(i,2) = length(find(Node_2_MAC_TX(:,1) == compare(i)));
            else
                MAC_TX3(i,2) = 0;
            end
        end

        for i = 1:length(compare)
            MAC_TX4(i,1) = compare(i);
            if ~isempty(find(Node_3_MAC_TX(:,1) == compare(i)))
                MAC_TX4(i,2) = length(find(Node_3_MAC_TX(:,1) == compare(i)));
            else
                MAC_TX4(i,2) = 0;
            end
        end




        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % MAC RETX
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        load 'Node_0_MAC_RETX.txt'
        load 'Node_1_MAC_RETX.txt'
        load 'Node_2_MAC_RETX.txt'
        load 'Node_3_MAC_RETX.txt'
        
        Node_0_MAC_RETX(:,1) = round(Node_0_MAC_RETX(:,1)*10)/10;
        Node_1_MAC_RETX(:,1) = round(Node_1_MAC_RETX(:,1)*10)/10;
        Node_2_MAC_RETX(:,1) = round(Node_2_MAC_RETX(:,1)*10)/10;
        Node_3_MAC_RETX(:,1) = round(Node_3_MAC_RETX(:,1)*10)/10;





        for i = 1:length(compare)
            MAC_RETX1(i,1) = compare(i);
            if ~isempty(find(Node_0_MAC_RETX(:,1) == compare(i)))
                MAC_RETX1(i,2) = length(find(Node_0_MAC_RETX == compare(i)));
            else
                MAC_RETX1(i,2) = 0;
            end
        end

        for i = 1:length(compare)
            MAC_RETX2(i,1) = compare(i);
            if ~isempty(find(Node_1_MAC_RETX(:,1) == compare(i)))
                MAC_RETX2(i,2) = length(find(Node_1_MAC_RETX == compare(i)));
            else
                MAC_RETX2(i,2) = 0;
            end
        end

        for i = 1:length(compare)
            MAC_RETX3(i,1) = compare(i);
            if ~isempty(find(Node_2_MAC_RETX(:,1) == compare(i)))
                MAC_RETX3(i,2) = length(find(Node_2_MAC_RETX == compare(i)));
            else
                MAC_RETX3(i,2) = 0;
            end
        end

        for i = 1:length(compare)
            MAC_RETX4(i,1) = compare(i);
            if ~isempty(find(Node_3_MAC_RETX(:,1) == compare(i)))
                MAC_RETX4(i,2) = length(find(Node_3_MAC_RETX == compare(i)));
            else
                MAC_RETX4(i,2) = 0;
            end
        end


    %Build a name for the matfile
        save(datetime , 'thr', 'cong_win','mobility_vectorH','-regexp' , 'MAC_*' , 'cont_*' , 'q*', 'routing_*' );
    else
        save(datetime , 'thr', 'mobility_vectorH');
    end


end
