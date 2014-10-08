%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% THROUGHPUT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function processParametersCHAIN2(startTime , simuTime, sample )

datetime=datestr(now);
datetime=strrep(datetime,':','_'); %Replace colon with underscore
datetime=strrep(datetime,'-','_'); %Replace minus sign with underscore
datetime=strrep(datetime,' ','_'); %Replace space with underscore
datetime = strcat('chainV_',datetime);

load('ACK_received_by_node_5.txt')

ACK_received_by_node_5 = [ACK_received_by_node_5([2:length(ACK_received_by_node_5)],1),ACK_received_by_node_5([2:length(ACK_received_by_node_5)],2)];

for i = 1:length(ACK_received_by_node_5)
    num2str(ACK_received_by_node_5(i,1),'%.1f');
    ACK_received_by_node_5(i,1) = str2num(ans);
end

trg = startTime;
thr(1,1) = startTime;
thr(1,2) = 0;
thr(1,3) = 0;
counter = 2;
num_dig = 1;

while trg < ACK_received_by_node_5(length(ACK_received_by_node_5),1) 
    thr(counter, 1) = trg;
    if ~isempty(find(ACK_received_by_node_5(:,1) == trg,1))
       thr(counter, 2) = ACK_received_by_node_5(find(ACK_received_by_node_5(:,1) == trg,1),2);
    else
       thr(counter, 2) = thr(counter-1, 2);
    end
    thr(counter,3) = ((thr(counter,2) - thr(counter-1,2))*8)/sample; 
    counter = counter + 1;
    trg = trg + sample;
    trg = round(trg*(10^num_dig))/(10^num_dig);
end


mobility_vectorV = zeros(length(thr(:,1)),2);
mobility_vectorV(:,1) = thr(:,1);


N=100;
for ii=0:(60-1)
    if((ii>9 && ii < 14) || (ii>=33 && ii <= 38)  )
        mobility_vectorV(1+(N*ii):(N*(ii+1))-1,2) = ones(1,N-1);
    end    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ROUTING TABLES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load routing_tables_6.txt
load routing_tables_7.txt
load routing_tables_8.txt
load routing_tables_9.txt

% load routing_tables_6.txt

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CNG WIN
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load 'CongestionWindow_node_5.txt'

for i = 1:length(CongestionWindow_node_5)
    num2str(CongestionWindow_node_5(i,1),'%.1f');
    CongestionWindow_node_5(i,1) = str2num(ans);
end

trg = startTime;
counter = 1;
num_dig = 1;

while trg < CongestionWindow_node_5(length(CongestionWindow_node_5),1) 
    cong_win(counter, 1) = trg;
    if ~isempty(find(CongestionWindow_node_5(:,1) == trg,1))
       cong_win(counter, 2) = CongestionWindow_node_5(find(CongestionWindow_node_5(:,1) == trg,1),2);
    else
       if(counter == 1)
           cong_win(counter, 2) = CongestionWindow_node_5(1,2);
           
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

load 'MAC_Queue_5.txt'
load 'MAC_Queue_6.txt'
load 'MAC_Queue_7.txt'
load 'MAC_Queue_8.txt'

for i = 1:length(MAC_Queue_5)
    num2str(MAC_Queue_5(i,1),'%.1f');
    MAC_Queue_5(i,1) = str2num(ans);
end

for i = 1:length(MAC_Queue_6)
    num2str(MAC_Queue_6(i,1),'%.1f');
    MAC_Queue_6(i,1) = str2num(ans);
end

for i = 1:length(MAC_Queue_7)
    num2str(MAC_Queue_7(i,1),'%.1f');
    MAC_Queue_7(i,1) = str2num(ans);
end

for i = 1:length(MAC_Queue_8)
    num2str(MAC_Queue_8(i,1),'%.1f');
    MAC_Queue_8(i,1) = str2num(ans);
end




trg = startTime;
counter = 1;
num_dig = 1;

%%%% 0 %%%%
while trg < MAC_Queue_5(length(MAC_Queue_5),1) 
    q5(counter, 1) = trg;
    if ~isempty(find(MAC_Queue_5(:,1) == trg,1))
       q5(counter, 2) = MAC_Queue_5(find(MAC_Queue_5(:,1) == trg,1),2);
    else
       if(counter ==1)
            q5(counter, 2) = MAC_Queue_5(1,2);
        else
           q5(counter, 2) = q5(counter-1, 2);
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
while trg < MAC_Queue_6(length(MAC_Queue_6),1) 
    q6(counter, 1) = trg;
    if ~isempty(find(MAC_Queue_6(:,1) == trg,1))
       q6(counter, 2) = MAC_Queue_6(find(MAC_Queue_6(:,1) == trg,1),2);
    else
       if(counter ==1)
            q6(counter, 2) = MAC_Queue_6(1,2);
        else
           q6(counter, 2) = q6(counter-1, 2);
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
while trg < MAC_Queue_7(length(MAC_Queue_7),1) 
    q7(counter, 1) = trg;
    if ~isempty(find(MAC_Queue_7(:,1) == trg,1))
       q7(counter, 2) = MAC_Queue_7(find(MAC_Queue_7(:,1) == trg,1),2);
    else
       if(counter ==1)
            q7(counter, 2) = MAC_Queue_7(1,2);
        else
           q7(counter, 2) = q7(counter-1, 2);
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
while trg < MAC_Queue_8(length(MAC_Queue_8),1) 
    q8(counter, 1) = trg;
    if ~isempty(find(MAC_Queue_8(:,1) == trg,1))
       q8(counter, 2) = MAC_Queue_8(find(MAC_Queue_8(:,1) == trg,1),2);
    else
       if(counter ==1)
            q8(counter, 2) = MAC_Queue_8(1,2);
        else
           q8(counter, 2) = q8(counter-1, 2);
        end
    end
    counter = counter + 1;
    trg = trg + sample;
    trg = round(trg*(10^num_dig))/(10^num_dig);
end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CNTWIN
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
load 'ContentionWindow_node_5.txt'
load 'ContentionWindow_node_6.txt'
load 'ContentionWindow_node_7.txt'
load 'ContentionWindow_node_8.txt'

% load 'ContentionWindow_node_5.txt'

for i = 1:length(ContentionWindow_node_5)
    num2str(ContentionWindow_node_5(i,1),'%.1f');
    ContentionWindow_node_5(i,1) = str2num(ans);
end

for i = 1:length(ContentionWindow_node_6)
    num2str(ContentionWindow_node_6(i,1),'%.1f');
    ContentionWindow_node_6(i,1) = str2num(ans);
end

for i = 1:length(ContentionWindow_node_7)
    num2str(ContentionWindow_node_7(i,1),'%.1f');
    ContentionWindow_node_7(i,1) = str2num(ans);
end

for i = 1:length(ContentionWindow_node_8)
    num2str(ContentionWindow_node_8(i,1),'%.1f');
    ContentionWindow_node_8(i,1) = str2num(ans);
end



trg = startTime;
counter = 1;
num_dig = 1;

%%%% 1 %%%%
while trg < ContentionWindow_node_5(length(ContentionWindow_node_5),1) 
    cont_win5(counter, 1) = trg;
    if ~isempty(find(ContentionWindow_node_5(:,1) == trg,1))
       cont_win5(counter, 2) = ContentionWindow_node_5(find(ContentionWindow_node_5(:,1) == trg,1),2);
    else
       if(counter==1)
             cont_win5(counter, 2) = ContentionWindow_node_5(1,2);
        else
            cont_win5(counter, 2) = cont_win5(counter-1, 2);
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
while trg < ContentionWindow_node_6(length(ContentionWindow_node_6),1) 
    cont_win6(counter, 1) = trg;
    if ~isempty(find(ContentionWindow_node_6(:,1) == trg,1))
       cont_win6(counter, 2) = ContentionWindow_node_6(find(ContentionWindow_node_6(:,1) == trg,1),2);
    else
       if(counter==1)
             cont_win6(counter, 2) = ContentionWindow_node_6(1,2);
        else
            cont_win6(counter, 2) = cont_win6(counter-1, 2);
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
while trg < ContentionWindow_node_7(length(ContentionWindow_node_7),1) 
    cont_win7(counter, 1) = trg;
    if ~isempty(find(ContentionWindow_node_7(:,1) == trg,1))
       cont_win7(counter, 2) = ContentionWindow_node_7(find(ContentionWindow_node_7(:,1) == trg,1),2);
    else
        if(counter==1)
             cont_win7(counter, 2) = ContentionWindow_node_7(1,2);
        else
            cont_win7(counter, 2) = cont_win7(counter-1, 2);
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
while trg < ContentionWindow_node_8(length(ContentionWindow_node_8),1) 
    cont_win8(counter, 1) = trg;
    if ~isempty(find(ContentionWindow_node_8(:,1) == trg,1))
       cont_win8(counter, 2) = ContentionWindow_node_8(find(ContentionWindow_node_8(:,1) == trg,1),2);
    else
       if(counter==1)
             cont_win8(counter, 2) = ContentionWindow_node_8(1,2);
        else
            cont_win8(counter, 2) = cont_win8(counter-1, 2);
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
% MAC TX
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



load 'Node_5_MAC_TX.txt'
load 'Node_6_MAC_TX.txt'
load 'Node_7_MAC_TX.txt'
load 'Node_8_MAC_TX.txt'

for i = 1:length(Node_5_MAC_TX)
    num2str(Node_5_MAC_TX(i,1),'%.1f');
    Node_5_MAC_TX(i,1) = str2num(ans);
end

for i = 1:length(Node_6_MAC_TX)
    num2str(Node_6_MAC_TX(i,1),'%.1f');
    Node_6_MAC_TX(i,1) = str2num(ans);
end

for i = 1:length(Node_7_MAC_TX)
    num2str(Node_7_MAC_TX(i,1),'%.1f');
    Node_7_MAC_TX(i,1) = str2num(ans);
end

for i = 1:length(Node_8_MAC_TX)
    num2str(Node_8_MAC_TX(i,1),'%.1f');
    Node_8_MAC_TX(i,1) = str2num(ans);
end



compare = [startTime:sample:simuTime];

for i = 1:length(compare)
    MAC_TX6(i,1) = compare(i);
    if ~isempty(find(Node_5_MAC_TX(:,1) == compare(i)))
        MAC_TX6(i,2) = length(find(Node_5_MAC_TX(:,1) == compare(i)));
    else
        MAC_TX6(i,2) = 0;
    end
end

for i = 1:length(compare)
    MAC_TX7(i,1) = compare(i);
    if ~isempty(find(Node_6_MAC_TX(:,1) == compare(i)))
        MAC_TX7(i,2) = length(find(Node_6_MAC_TX(:,1) == compare(i)));
    else
        MAC_TX7(i,2) = 0;
    end
end

for i = 1:length(compare)
    MAC_TX8(i,1) = compare(i);
    if ~isempty(find(Node_7_MAC_TX(:,1) == compare(i)))
        MAC_TX8(i,2) = length(find(Node_7_MAC_TX(:,1) == compare(i)));
    else
        MAC_TX8(i,2) = 0;
    end
end

for i = 1:length(compare)
    MAC_TX9(i,1) = compare(i);
    if ~isempty(find(Node_8_MAC_TX(:,1) == compare(i)))
        MAC_TX9(i,2) = length(find(Node_8_MAC_TX(:,1) == compare(i)));
    else
        MAC_TX9(i,2) = 0;
    end
end




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MAC RETX
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load 'Node_5_MAC_RETX.txt'
load 'Node_6_MAC_RETX.txt'
load 'Node_7_MAC_RETX.txt'
load 'Node_8_MAC_RETX.txt'


for i = 1:length(Node_5_MAC_RETX)
    num2str(Node_5_MAC_RETX(i,1),'%.1f');
    Node_5_MAC_RETX(i,1) = str2num(ans);
end

for i = 1:length(Node_6_MAC_RETX)
    num2str(Node_6_MAC_RETX(i,1),'%.1f');
    Node_6_MAC_RETX(i,1) = str2num(ans);
end

for i = 1:length(Node_7_MAC_RETX)
    num2str(Node_7_MAC_RETX(i,1),'%.1f');
    Node_7_MAC_RETX(i,1) = str2num(ans);
end

for i = 1:length(Node_8_MAC_RETX)
    num2str(Node_8_MAC_RETX(i,1),'%.1f');
    Node_8_MAC_RETX(i,1) = str2num(ans);
end


 
for i = 1:length(compare)
    MAC_RETX6(i,1) = compare(i);
    if ~isempty(find(Node_5_MAC_RETX(:,1) == compare(i)))
        MAC_RETX6(i,2) = length(find(Node_5_MAC_RETX == compare(i)));
    else
        MAC_RETX6(i,2) = 0;
    end
end

for i = 1:length(compare)
    MAC_RETX7(i,1) = compare(i);
    if ~isempty(find(Node_6_MAC_RETX(:,1) == compare(i)))
        MAC_RETX7(i,2) = length(find(Node_6_MAC_RETX == compare(i)));
    else
        MAC_RETX7(i,2) = 0;
    end
end

for i = 1:length(compare)
    MAC_RETX8(i,1) = compare(i);
    if ~isempty(find(Node_7_MAC_RETX(:,1) == compare(i)))
        MAC_RETX8(i,2) = length(find(Node_7_MAC_RETX == compare(i)));
    else
        MAC_RETX8(i,2) = 0;
    end
end

for i = 1:length(compare)
    MAC_RETX9(i,1) = compare(i);
    if ~isempty(find(Node_8_MAC_RETX(:,1) == compare(i)))
        MAC_RETX9(i,2) = length(find(Node_8_MAC_RETX == compare(i)));
    else
        MAC_RETX9(i,2) = 0;
    end
end




%Build a name for the matfile


save(datetime , 'thr', 'cong_win', 'mobility_vectorV','-regexp' , 'MAC_*' , 'cont_*' , 'q*', 'routing_*' );

end