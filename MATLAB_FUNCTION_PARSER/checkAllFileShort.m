function [check]=checkAllFileShort()
   check=1;
    arrayFILE={'CongestionWindow_node_0.txt',...
     'routing_tables_1.txt','routing_tables_2.txt','routing_tables_3.txt','routing_tables_4.txt',...
     'MAC_Queue_0.txt','MAC_Queue_1.txt','MAC_Queue_2.txt','MAC_Queue_3.txt',...
     'ContentionWindow_node_0.txt','ContentionWindow_node_1.txt','ContentionWindow_node_2.txt','ContentionWindow_node_3.txt',...
     'Node_0_MAC_TX.txt','Node_1_MAC_TX.txt','Node_2_MAC_TX.txt','Node_3_MAC_TX.txt',...
     'Node_0_MAC_RETX.txt','Node_1_MAC_RETX.txt','Node_2_MAC_RETX.txt','Node_3_MAC_RETX.txt'};

    for ii=1:size(arrayFILE,2)
        if(exist(arrayFILE{ii},'file')==0)
            check=0;
            break
        end
    end

end
