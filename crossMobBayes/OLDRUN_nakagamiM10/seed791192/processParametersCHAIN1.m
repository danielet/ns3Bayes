%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% THROUGHPUT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function processParametersCHAIN1(startTime , simuTime, sample )

datetime=datestr(now);
datetime=strrep(datetime,':','_'); %Replace colon with underscore
datetime=strrep(datetime,'-','_'); %Replace minus sign with underscore
datetime=strrep(datetime,' ','_'); %Replace space with underscore
datetime = strcat('chainH_',datetime);


load('ACK_received_by_node_0.txt')

ACK_received_by_node_0 = [ACK_received_by_node_0([2:length(ACK_received_by_node_0)],1),ACK_received_by_node_0([2:length(ACK_received_by_node_0)],2)];

for i = 1:length(ACK_received_by_node_0)
    num2str(ACK_received_by_node_0(i,1),'%.1f');
    ACK_received_by_node_0(i,1) = str2num(ans);
end

trg = startTime;
thr(1,1) = startTime;
thr(1,2) = 0;
thr(1,3) = 0;
counter = 2;
num_dig = 1;

while trg < ACK_received_by_node_0(length(ACK_received_by_node_0),1) 
    thr(counter, 1) = trg;
    if ~isempty(find(ACK_received_by_node_0(:,1) == trg,1))
       thr(counter, 2) = ACK_received_by_node_0(find(ACK_received_by_node_0(:,1) == trg,1),2);
    else
       thr(counter, 2) = thr(counter-1, 2);
    end
    thr(counter,3) = ((thr(counter,2) - thr(counter-1,2))*8)/sample; 
    counter = counter + 1;
    trg = trg + sample;
    trg = round(trg*(10^num_dig))/(10^num_dig);
end


mobility_vectorH = zeros(length(thr(:,1)),2);
mobility_vectorH(:,1) = thr(:,1);
N=100;
for ii=0:(60-1)
    if((ii>0 && ii < 6) || (ii>=17 && ii <= 22)|| (ii>=26 && ii <= 30) || (ii>=41 && ii <= 46) )
        mobility_vectorH(1+(N*ii):(N*(ii+1))-1, 2) = ones(1,N-1);
    end    
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ROUTING TABLES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load routing_tables_1.txt
load routing_tables_2.txt
load routing_tables_3.txt
load routing_tables_4.txt
load routing_tables_5.txt
% load routing_tables_6.txt

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CNG WIN
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load 'CongestionWindow_node_0.txt'

for i = 1:length(CongestionWindow_node_0)
    num2str(CongestionWindow_node_0(i,1),'%.1f');
    CongestionWindow_node_0(i,1) = str2num(ans);
end

trg = startTime;
counter = 1;
num_dig = 1;

while trg < CongestionWindow_node_0(length(CongestionWindow_node_0),1) 
    cong_win(counter, 1) = trg;
    if ~isempty(find(CongestionWindow_node_0(:,1) == trg,1))
       cong_win(counter, 2) = CongestionWindow_node_0(find(CongestionWindow_node_0(:,1) == trg,1),2);
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
load 'MAC_Queue_4.txt'
% load 'MAC_Queue_5.txt'
% load 'MAC_Queue_6.txt'

for i = 1:length(MAC_Queue_0)
    num2str(MAC_Queue_0(i,1),'%.1f');
    MAC_Queue_0(i,1) = str2num(ans);
end

for i = 1:length(MAC_Queue_1)
    num2str(MAC_Queue_1(i,1),'%.1f');
    MAC_Queue_1(i,1) = str2num(ans);
end

for i = 1:length(MAC_Queue_2)
    num2str(MAC_Queue_2(i,1),'%.1f');
    MAC_Queue_2(i,1) = str2num(ans);
end

for i = 1:length(MAC_Queue_3)
    num2str(MAC_Queue_3(i,1),'%.1f');
    MAC_Queue_3(i,1) = str2num(ans);
end

for i = 1:length(MAC_Queue_4)
    num2str(MAC_Queue_4(i,1),'%.1f');
    MAC_Queue_4(i,1) = str2num(ans);
end

% for i = 1:length(MAC_Queue_5)
%     num2str(MAC_Queue_5(i,1),'%.1f');
%     MAC_Queue_5(i,1) = str2num(ans);
% end
% 
% for i = 1:length(MAC_Queue_6)
%     num2str(MAC_Queue_6(i,1),'%.1f');
%     MAC_Queue_6(i,1) = str2num(ans);
% end

trg = startTime;
counter = 1;
num_dig = 1;

%%%% 0 %%%%
while trg < MAC_Queue_0(length(MAC_Queue_0),1) 
    q0(counter, 1) = trg;
    if ~isempty(find(MAC_Queue_0(:,1) == trg,1))
       q0(counter, 2) = MAC_Queue_0(find(MAC_Queue_0(:,1) == trg,1),2);
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
    if ~isempty(find(MAC_Queue_1(:,1) == trg,1))
       q1(counter, 2) = MAC_Queue_1(find(MAC_Queue_1(:,1) == trg,1),2);
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
    if ~isempty(find(MAC_Queue_2(:,1) == trg,1))
       q2(counter, 2) = MAC_Queue_2(find(MAC_Queue_2(:,1) == trg,1),2);
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
    if ~isempty(find(MAC_Queue_3(:,1) == trg,1))
       q3(counter, 2) = MAC_Queue_3(find(MAC_Queue_3(:,1) == trg,1),2);
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

%%%% 4 %%%%
while trg < MAC_Queue_4(length(MAC_Queue_4),1) 
    q4(counter, 1) = trg;
    if ~isempty(find(MAC_Queue_4(:,1) == trg,1))
       q4(counter, 2) = MAC_Queue_4(find(MAC_Queue_4(:,1) == trg,1),2);
    else
       if(counter ==1)
            q4(counter, 2) = MAC_Queue_4(1,2);
        else
           q4(counter, 2) = q4(counter-1, 2);
        end
    end
    counter = counter + 1;
    trg = trg + sample;
    trg = round(trg*(10^num_dig))/(10^num_dig);
end

trg = startTime;
counter = 1;
num_dig = 1;

%%%% 5 %%%%
% while trg < MAC_Queue_5(length(MAC_Queue_5),1) 
%     q5(counter, 1) = trg;
%     if ~isempty(find(MAC_Queue_5(:,1) == trg,1))
%        q5(counter, 2) = MAC_Queue_5(find(MAC_Queue_5(:,1) == trg,1),2);
%     else
%        q5(counter, 2) = q5(counter-1, 2);
%     end
%     counter = counter + 1;
%     trg = trg + sample;
%     trg = round(trg*(10^num_dig))/(10^num_dig);
% end
% 
% trg = startTime;
% counter = 1;
% num_dig = 1;
% 
% %%%% 6 %%%%
% while trg < MAC_Queue_6(length(MAC_Queue_6),1) 
%     q6(counter, 1) = trg;
%     if ~isempty(find(MAC_Queue_6(:,1) == trg,1))
%        q6(counter, 2) = MAC_Queue_6(find(MAC_Queue_6(:,1) == trg,1),2);
%     else
%        q6(counter, 2) = q6(counter-1, 2);
%     end
%     counter = counter + 1;
%     trg = trg + sample;
%     trg = round(trg*(10^num_dig))/(10^num_dig);
% end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CNTWIN
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
load 'ContentionWindow_node_0.txt'
load 'ContentionWindow_node_1.txt'
load 'ContentionWindow_node_2.txt'
load 'ContentionWindow_node_3.txt'
load 'ContentionWindow_node_4.txt'
% load 'ContentionWindow_node_5.txt'

for i = 1:length(ContentionWindow_node_0)
    num2str(ContentionWindow_node_0(i,1),'%.1f');
    ContentionWindow_node_0(i,1) = str2num(ans);
end

for i = 1:length(ContentionWindow_node_1)
    num2str(ContentionWindow_node_1(i,1),'%.1f');
    ContentionWindow_node_1(i,1) = str2num(ans);
end

for i = 1:length(ContentionWindow_node_2)
    num2str(ContentionWindow_node_2(i,1),'%.1f');
    ContentionWindow_node_2(i,1) = str2num(ans);
end

for i = 1:length(ContentionWindow_node_3)
    num2str(ContentionWindow_node_3(i,1),'%.1f');
    ContentionWindow_node_3(i,1) = str2num(ans);
end

for i = 1:length(ContentionWindow_node_4)
    num2str(ContentionWindow_node_4(i,1),'%.1f');
    ContentionWindow_node_4(i,1) = str2num(ans);
end

% for i = 1:length(ContentionWindow_node_5)
%     num2str(ContentionWindow_node_5(i,1),'%.1f');
%     ContentionWindow_node_5(i,1) = str2num(ans);
% end

trg = startTime;
counter = 1;
num_dig = 1;

%%%% 1 %%%%
while trg < ContentionWindow_node_0(length(ContentionWindow_node_0),1) 
    cont_win1(counter, 1) = trg;
    if ~isempty(find(ContentionWindow_node_0(:,1) == trg,1))
       cont_win1(counter, 2) = ContentionWindow_node_0(find(ContentionWindow_node_0(:,1) == trg,1),2);
    else
       if(counter==1)
             cont_win1(counter, 2) = ContentionWindow_node_0(1,2);
        else
            cont_win1(counter, 2) = cont_win1(counter-1, 2);
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
    if ~isempty(find(ContentionWindow_node_1(:,1) == trg,1))
       cont_win2(counter, 2) = ContentionWindow_node_1(find(ContentionWindow_node_1(:,1) == trg,1),2);
    else
       if(counter==1)
             cont_win2(counter, 2) = ContentionWindow_node_1(1,2);
        else
            cont_win2(counter, 2) = cont_win2(counter-1, 2);
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
    if ~isempty(find(ContentionWindow_node_2(:,1) == trg,1))
       cont_win3(counter, 2) = ContentionWindow_node_2(find(ContentionWindow_node_2(:,1) == trg,1),2);
    else
       if(counter==1)
             cont_win3(counter, 2) = ContentionWindow_node_2(1,2);
        else
            cont_win3(counter, 2) = cont_win3(counter-1, 2);
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
    if ~isempty(find(ContentionWindow_node_3(:,1) == trg,1))
       cont_win4(counter, 2) = ContentionWindow_node_3(find(ContentionWindow_node_3(:,1) == trg,1),2);
    else
       if(counter==1)
             cont_win4(counter, 2) = ContentionWindow_node_3(1,2);
        else
            cont_win4(counter, 2) = cont_win4(counter-1, 2);
        end
    end
    counter = counter + 1;
    trg = trg + sample;
    trg = round(trg*(10^num_dig))/(10^num_dig);
end

trg = startTime;
counter = 1;
num_dig = 1;

%%%% 5 %%%%
while trg < ContentionWindow_node_4(length(ContentionWindow_node_4),1) 
    cont_win5(counter, 1) = trg;
    if ~isempty(find(ContentionWindow_node_4(:,1) == trg,1))
       cont_win5(counter, 2) = ContentionWindow_node_4(find(ContentionWindow_node_4(:,1) == trg,1),2);
    else
       if(counter==1)
             cont_win5(counter, 2) = ContentionWindow_node_4(1,2);
        else
            cont_win5(counter, 2) = cont_win5(counter-1, 2);
        end
    end
    counter = counter + 1;
    trg = trg + sample;
    trg = round(trg*(10^num_dig))/(10^num_dig);
end

% trg = startTime;
% counter = 1;
% num_dig = 1;

% %%%% 6 %%%%
% while trg < ContentionWindow_node_5(length(ContentionWindow_node_5),1) 
%     cont_win6(counter, 1) = trg;
%     if ~isempty(find(ContentionWindow_node_5(:,1) == trg,1))
%        cont_win6(counter, 2) = ContentionWindow_node_5(find(ContentionWindow_node_5(:,1) == trg,1),2);
%     else
%        cont_win6(counter, 2) = cont_win6(counter-1, 2);
%     end
%     counter = counter + 1;
%     trg = trg + sample;
%     trg = round(trg*(10^num_dig))/(10^num_dig);
% end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MAC TX
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load 'Node_0_MAC_TX.txt'
load 'Node_1_MAC_TX.txt'
load 'Node_2_MAC_TX.txt'
load 'Node_3_MAC_TX.txt'
load 'Node_4_MAC_TX.txt'
% load 'Node_5_MAC_TX.txt'

for i = 1:length(Node_0_MAC_TX)
    num2str(Node_0_MAC_TX(i,1),'%.1f');
    Node_0_MAC_TX(i,1) = str2num(ans);
end

for i = 1:length(Node_1_MAC_TX)
    num2str(Node_1_MAC_TX(i,1),'%.1f');
    Node_1_MAC_TX(i,1) = str2num(ans);
end

for i = 1:length(Node_2_MAC_TX)
    num2str(Node_2_MAC_TX(i,1),'%.1f');
    Node_2_MAC_TX(i,1) = str2num(ans);
end

for i = 1:length(Node_3_MAC_TX)
    num2str(Node_3_MAC_TX(i,1),'%.1f');
    Node_3_MAC_TX(i,1) = str2num(ans);
end

for i = 1:length(Node_4_MAC_TX)
    num2str(Node_4_MAC_TX(i,1),'%.1f');
    Node_4_MAC_TX(i,1) = str2num(ans);
end

% for i = 1:length(Node_5_MAC_TX)
%     num2str(Node_5_MAC_TX(i,1),'%.1f');
%     Node_5_MAC_TX(i,1) = str2num(ans);
% end

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

% for i = 1:length(compare)
%     MAC_TX6(i,1) = compare(i);
%     if ~isempty(find(Node_5_MAC_TX(:,1) == compare(i)))
%         MAC_TX6(i,2) = length(find(Node_5_MAC_TX(:,1) == compare(i)));
%     else
%         MAC_TX6(i,2) = 0;
%     end
% end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MAC RETX
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load 'Node_0_MAC_RETX.txt'
load 'Node_1_MAC_RETX.txt'
load 'Node_2_MAC_RETX.txt'
load 'Node_3_MAC_RETX.txt'
load 'Node_4_MAC_RETX.txt'
% load 'Node_5_MAC_RETX.txt'

for i = 1:length(Node_0_MAC_RETX)
    num2str(Node_0_MAC_RETX(i,1),'%.1f');
    Node_0_MAC_RETX(i,1) = str2num(ans);
end

for i = 1:length(Node_1_MAC_RETX)
    num2str(Node_1_MAC_RETX(i,1),'%.1f');
    Node_1_MAC_RETX(i,1) = str2num(ans);
end

for i = 1:length(Node_2_MAC_RETX)
    num2str(Node_2_MAC_RETX(i,1),'%.1f');
    Node_2_MAC_RETX(i,1) = str2num(ans);
end

for i = 1:length(Node_3_MAC_RETX)
    num2str(Node_3_MAC_RETX(i,1),'%.1f');
    Node_3_MAC_RETX(i,1) = str2num(ans);
end

for i = 1:length(Node_4_MAC_RETX)
    num2str(Node_4_MAC_RETX(i,1),'%.1f');
    Node_4_MAC_RETX(i,1) = str2num(ans);
end

% for i = 1:length(Node_5_MAC_RETX)
%     num2str(Node_5_MAC_RETX(i,1),'%.1f');
%     Node_5_MAC_RETX(i,1) = str2num(ans);
% end
 

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

% for i = 1:length(compare)
%     MAC_RETX6(i,1) = compare(i);
%     if ~isempty(find(Node_5_MAC_RETX(:,1) == compare(i)))
%         MAC_RETX6(i,2) = length(find(Node_5_MAC_RETX == compare(i)));
%     else
%         MAC_RETX6(i,2) = 0;
%     end
% end

%Build a name for the matfile





% datetime=datestr(now);
% datetime=strrep(datetime,':','_'); %Replace colon with underscore
% datetime=strrep(datetime,'-','_'); %Replace minus sign with underscore
% datetime=strrep(datetime,' ','_'); %Replace space with underscore
% strcat('chainH_',datetime);
save(datetime , 'thr', 'cong_win','mobility_vectorH','-regexp' , 'MAC_*' , 'cont_*' , 'q*', 'routing_*' );

end