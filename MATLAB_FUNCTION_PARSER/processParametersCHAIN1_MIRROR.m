%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% THROUGHPUT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function processParametersCHAIN1_MIRROR(startTime , simuTime, sample,varDir ,nameEXP , ctrOTHER )

    datetime=datestr(now);
    datetime=strrep(datetime,':','_'); %Replace colon with underscore
    datetime=strrep(datetime,'-','_'); %Replace minus sign with underscore
    datetime=strrep(datetime,' ','_'); %Replace space with underscore
    datetime = strcat('ChainH_',datetime);
    
    wheresave=strcat(varDir,'/',nameEXP,'/');
    datetime = strcat(wheresave,datetime);

    load('ACK_received_by_node_0.txt')

    ACK_received_by_node_0 = [ACK_received_by_node_0([2:length(ACK_received_by_node_0)],1),ACK_received_by_node_0([2:length(ACK_received_by_node_0)],2)];

%     for i = 1:length(ACK_received_by_node_0)
%         tmp = num2str(ACK_received_by_node_0(i,1),'%.1f');
%         ACK_received_by_node_0(i,1) = str2num(tmp);
%     end


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
%         thr(counter,3) = ((thr(counter,2) - thr(counter-1,2))*8)/sample; %%???? non serve dividere per sample
        thr(counter,3) = ((thr(counter,2) - thr(counter-1,2))*8)/sample; %%???? non serve dividere per sample
        counter = counter + 1;
        trg = trg + sample;
        trg = round(trg*(10^num_dig))/(10^num_dig);
    end

    
    if(trg < simuTime - sample)
        while (trg < simuTime - sample) 
            pause
            thr(counter, 1) = trg;
            thr(counter, 3) = 0;
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end
    end
    
    
    mobility_vectorH = zeros(length(thr(:,1)),2);
%     mobility_vectorH(:,1) = thr(:,1);
%     N=100;
%     for ii=0:(80-1)
%         if((ii>=1 && ii <= 5) || (ii>=17 && ii <= 21)||  (ii>=41 && ii <= 45) ||  (ii>=57 && ii <=61))
%             mobility_vectorH(1+(N*ii):(N*(ii+1)), 2) = ones(1,N);
%         end    
%     end



    
    if(ctrOTHER ==1)

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % ROUTING TABLES
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%         load routing_tables_1.txt
        inputfile = fopen('routing_tables_1.txt');
        routing_tables_1 = textscan(inputfile,'%f%d%d%d%s%s%s%s%s%s%s%s%s', 'delimiter', '\t');
        fclose(inputfile)
        
        inputfile = fopen('routing_tables_2.txt');
        routing_tables_2 = textscan(inputfile,'%f%d%d%d%s%s%s%s%s%s%s%s%s', 'delimiter', '\t');
        fclose(inputfile)
        
        
        inputfile = fopen('routing_tables_3.txt');
        routing_tables_3 = textscan(inputfile,'%f%d%d%d%s%s%s%s%s%s%s%s%s', 'delimiter', '\t');
        fclose(inputfile)
        
        
        inputfile = fopen('routing_tables_4.txt');
        routing_tables_4 = textscan(inputfile,'%f%d%d%d%s%s%s%s%s%s%s%s%s', 'delimiter', '\t');
        fclose(inputfile)
        
        
        inputfile = fopen('routing_tables_5.txt');
        routing_tables_5 = textscan(inputfile,'%f%d%d%d%s%s%s%s%s%s%s%s%s', 'delimiter', '\t');
        fclose(inputfile)

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

        
        if(trg < simuTime - sample)
            while (trg < simuTime - sample)
                cong_win(counter, 1) = trg;
                cong_win(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
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
        load 'MAC_Queue_4.txt'
        % load 'MAC_Queue_5.txt'
        % load 'MAC_Queue_6.txt'
        MAC_Queue_0(:,1) = round(MAC_Queue_0(:,1)*10)/10;
%         for i = 1:length(MAC_Queue_0)
%             num2str(MAC_Queue_0(i,1),'%.1f');
%             MAC_Queue_0(i,1) = str2num(ans);
%         end
        MAC_Queue_1(:,1) = round(MAC_Queue_1(:,1)*10)/10;
%         for i = 1:length(MAC_Queue_1)
%             num2str(MAC_Queue_1(i,1),'%.1f');
%             MAC_Queue_1(i,1) = str2num(ans);
    %     end
        MAC_Queue_2(:,1) = round(MAC_Queue_2(:,1)*10)/10;
%         for i = 1:length(MAC_Queue_2)
%             num2str(MAC_Queue_2(i,1),'%.1f');
%             MAC_Queue_2(i,1) = str2num(ans);
%         end
        MAC_Queue_3(:,1) = round(MAC_Queue_3(:,1)*10)/10;
%         for i = 1:length(MAC_Queue_3)
%             num2str(MAC_Queue_3(i,1),'%.1f');
%             MAC_Queue_3(i,1) = str2num(ans);
%         end
        MAC_Queue_4(:,1) = round(MAC_Queue_4(:,1)*10)/10;
%         for i = 1:length(MAC_Queue_4)
%             num2str(MAC_Queue_4(i,1),'%.1f');
%             MAC_Queue_4(i,1) = str2num(ans);
%         end

        

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
%                    q0(counter, 2) = q0(counter-1, 2);
                   q0(counter, 2) = 0;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end


        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
                q0(counter, 1) = trg;
                q0(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
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
%                    q1(counter, 2) =q1(counter-1, 2);
                     q1(counter, 2) = 0;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
%                 q1(counter, 2) = 1;
                q1(counter, 1) = trg;
                q1(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
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
%                    q2(counter, 2) = q2(counter-1, 2);
                   q2(counter, 2) = 0;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
%                 q2(counter, 2) = 1;
                q2(counter, 1) = trg;
                q2(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
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
               if(counter == 1)
                    q3(counter, 2) = MAC_Queue_3(1,2);
                else
%                    q3(counter, 2) = q3(counter-1, 2);
                    q3(counter, 2) = 0;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
%                 q3(counter, 2) = 1;
                q3(counter, 1) = trg;
                q3(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
        end
        
        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 4 %%%%
        while trg < MAC_Queue_4(length(MAC_Queue_4),1) 
            q4(counter, 1) = trg;
            index =find(MAC_Queue_4(:,1) == trg);
            if ~isempty(index)
               q4(counter, 2) = MAC_Queue_4(index(1),2);
            else
               if(counter ==1)
                    q4(counter, 2) = MAC_Queue_4(1,2);
                else
%                    q4(counter, 2) = q4(counter-1, 2);
                    q4(counter, 2) = 0;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
%                 q4(counter, 2) = 1;
                q4(counter, 1) = trg;
                q4(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
        end
        


        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % CNTWIN
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        load 'ContentionWindow_node_0.txt'
        load 'ContentionWindow_node_1.txt'
        load 'ContentionWindow_node_2.txt'
        load 'ContentionWindow_node_3.txt'
        load 'ContentionWindow_node_4.txt'
        % load 'ContentionWindow_node_5.txt'
        ContentionWindow_node_0(:,1) = round(ContentionWindow_node_0(:,1)*10)/10;
%         for i = 1:length(ContentionWindow_node_0)
%             num2str(ContentionWindow_node_0(i,1),'%.1f');
%             ContentionWindow_node_0(i,1) = str2num(ans);
%         end
        ContentionWindow_node_1(:,1) = round(ContentionWindow_node_1(:,1)*10)/10;
%         for i = 1:length(ContentionWindow_node_1)
%             num2str(ContentionWindow_node_1(i,1),'%.1f');
%             ContentionWindow_node_1(i,1) = str2num(ans);
%         end
        ContentionWindow_node_2(:,1) = round(ContentionWindow_node_2(:,1)*10)/10;
%         for i = 1:length(ContentionWindow_node_2)
%             num2str(ContentionWindow_node_2(i,1),'%.1f');
%             ContentionWindow_node_2(i,1) = str2num(ans);
%         end
        ContentionWindow_node_3(:,1) = round(ContentionWindow_node_3(:,1)*10)/10;
%         for i = 1:length(ContentionWindow_node_3)
%             num2str(ContentionWindow_node_3(i,1),'%.1f');
%             ContentionWindow_node_3(i,1) = str2num(ans);
%         end
        ContentionWindow_node_4(:,1) = round(ContentionWindow_node_4(:,1)*10)/10;
%         for i = 1:length(ContentionWindow_node_4)
%             num2str(ContentionWindow_node_4(i,1),'%.1f');
%             ContentionWindow_node_4(i,1) = str2num(ans);
%         end



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
               cont_win1_e(counter , 2) = sum(ContentionWindow_node_0(index,2))/length(index);
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

        
        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
                cont_win1(counter, 1) = trg;
                cont_win1(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
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
               cont_win2_e(counter , 2) = sum(ContentionWindow_node_1(index,2))/length(index);
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

        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
                cont_win2(counter, 1) = trg;
                cont_win2(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
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
               cont_win3_e(counter , 2) = sum(ContentionWindow_node_2(index,2))/length(index);
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

        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
                cont_win3(counter, 1) = trg;
                cont_win3(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
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
               cont_win4_e(counter , 2) = sum(ContentionWindow_node_3(index,2))/length(index);
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

        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
                cont_win4(counter, 1) = trg;
                cont_win4(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
        end
        
        trg = startTime;
        counter = 1;
        num_dig = 1;

        %%%% 5 %%%%
        while trg < ContentionWindow_node_4(length(ContentionWindow_node_4),1) 
            cont_win5(counter, 1) = trg;
            index =find(ContentionWindow_node_4(:,1) == trg);
            if ~isempty(index)
               cont_win5(counter, 2) = ContentionWindow_node_4(index(1),2);
               cont_win5_p(counter , 2) = sum(ContentionWindow_node_4(index,2))/length(index);
               cont_win5_e(counter , 2) = sum(ContentionWindow_node_4(index,2))/length(index);
               cont_win5_m(counter , 2) = mean(ContentionWindow_node_4(index,2));
               cont_win5_s(counter , 2) = std(ContentionWindow_node_4(index,2));
            else
                if(counter==1)
                    cont_win5(counter, 2) = ContentionWindow_node_4(1,2);
                    cont_win5_p(counter , 2) = 15;
                    cont_win5_e(counter , 2) = 15;
                    cont_win5_m(counter , 2) = 15;
                    cont_win5_s(counter , 2) = 15;
                else
                    cont_win5(counter, 2) = cont_win5(counter-1, 2);
                    cont_win5_p(counter , 2) = 15;
                    cont_win5_e(counter , 2) = 15;
                    cont_win5_m(counter , 2) = 15;
                    cont_win5_s(counter , 2) = 15;
                end
            end
            counter = counter + 1;
            trg = trg + sample;
            trg = round(trg*(10^num_dig))/(10^num_dig);
        end

        if(trg < simuTime - sample)
            while (trg < simuTime - sample) 
                cont_win5(counter, 1) = trg;
                cont_win5(counter, 2) = 0;
                counter = counter + 1;
                trg = trg + sample;
                trg = round(trg*(10^num_dig))/(10^num_dig);
            end
        end


        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % MAC TX
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        load 'Node_0_MAC_TX.txt'
        load 'Node_1_MAC_TX.txt'
        load 'Node_2_MAC_TX.txt'
        load 'Node_3_MAC_TX.txt'
        load 'Node_4_MAC_TX.txt'
        % load 'Node_5_MAC_TX.txt'
        Node_0_MAC_TX(:,1) = round(Node_0_MAC_TX(:,1)*10)/10;
%         for i = 1:length(Node_0_MAC_TX)
%             num2str(Node_0_MAC_TX(i,1),'%.1f');
%             Node_0_MAC_TX(i,1) = str2num(ans);
%         end
        Node_1_MAC_TX(:,1) = round(Node_1_MAC_TX(:,1)*10)/10;
%         for i = 1:length(Node_1_MAC_TX)
%             num2str(Node_1_MAC_TX(i,1),'%.1f');
%             Node_1_MAC_TX(i,1) = str2num(ans);
%         end
        Node_2_MAC_TX(:,1) = round(Node_2_MAC_TX(:,1)*10)/10;
%         for i = 1:length(Node_2_MAC_TX)
%             num2str(Node_2_MAC_TX(i,1),'%.1f');
%             Node_2_MAC_TX(i,1) = str2num(ans);
%         end
        Node_3_MAC_TX(:,1) = round(Node_3_MAC_TX(:,1)*10)/10;
%         for i = 1:length(Node_3_MAC_TX)
%             num2str(Node_3_MAC_TX(i,1),'%.1f');
%             Node_3_MAC_TX(i,1) = str2num(ans);
%         end
        Node_4_MAC_TX(:,1) = round(Node_4_MAC_TX(:,1)*10)/10;
%         for i = 1:length(Node_4_MAC_TX)
%             num2str(Node_4_MAC_TX(i,1),'%.1f');
%             Node_4_MAC_TX(i,1) = str2num(ans);
%         end



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

        for i = 1:length(compare)
            MAC_TX5(i,1) = compare(i);
            if ~isempty(find(Node_4_MAC_TX(:,1) == compare(i)))
                MAC_TX5(i,2) = length(find(Node_4_MAC_TX(:,1) == compare(i)));
            else
                MAC_TX5(i,2) = 0;
            end
        end




        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % MAC RETX
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        load 'Node_0_MAC_RETX.txt'
        load 'Node_1_MAC_RETX.txt'
        load 'Node_2_MAC_RETX.txt'
        load 'Node_3_MAC_RETX.txt'
        load 'Node_4_MAC_RETX.txt'
        % load 'Node_5_MAC_RETX.txt'
        Node_0_MAC_RETX(:,1) = round(Node_0_MAC_RETX(:,1)*10)/10;
%         for i = 1:length(Node_0_MAC_RETX)
%             num2str(Node_0_MAC_RETX(i,1),'%.1f');
%             Node_0_MAC_RETX(i,1) = str2num(ans);
%         end
        Node_1_MAC_RETX(:,1) = round(Node_1_MAC_RETX(:,1)*10)/10;
%         for i = 1:length(Node_1_MAC_RETX)
%             num2str(Node_1_MAC_RETX(i,1),'%.1f');
%             Node_1_MAC_RETX(i,1) = str2num(ans);
%         end
        Node_2_MAC_RETX(:,1) = round(Node_2_MAC_RETX(:,1)*10)/10;
%         for i = 1:length(Node_2_MAC_RETX)
%             num2str(Node_2_MAC_RETX(i,1),'%.1f');
%             Node_2_MAC_RETX(i,1) = str2num(ans);
%         end
        Node_3_MAC_RETX(:,1) = round(Node_3_MAC_RETX(:,1)*10)/10;
%         for i = 1:length(Node_3_MAC_RETX)
%             num2str(Node_3_MAC_RETX(i,1),'%.1f');
%             Node_3_MAC_RETX(i,1) = str2num(ans);
%         end
        Node_4_MAC_RETX(:,1) = round(Node_4_MAC_RETX(:,1)*10)/10;
%         for i = 1:length(Node_4_MAC_RETX)
%             num2str(Node_4_MAC_RETX(i,1),'%.1f');
%             Node_4_MAC_RETX(i,1) = str2num(ans);
%         end



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

        for i = 1:length(compare)
            MAC_RETX5(i,1) = compare(i);
            if ~isempty(find(Node_4_MAC_RETX(:,1) == compare(i)))
                MAC_RETX5(i,2) = length(find(Node_4_MAC_RETX == compare(i)));
            else
                MAC_RETX5(i,2) = 0;
            end
        end



    %Build a name for the matfile
        save(datetime , 'thr', 'cong_win','mobility_vectorH','-regexp' , 'MAC_*' , 'cont_*' , 'q*', 'routing_*' );
    else
        save(datetime , 'thr', 'mobility_vectorH');
    end


end