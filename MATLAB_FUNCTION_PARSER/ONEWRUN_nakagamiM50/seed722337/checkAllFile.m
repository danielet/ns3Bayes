function [check]=checkAllFile()
check=1;
    arrayFILE={'CongestionWindow_node_0.txt', 'CongestionWindow_node_5.txt' ,...
     'routing_tables_1.txt','routing_tables_2.txt','routing_tables_3.txt','routing_tables_4.txt','routing_tables_5.txt',...
     'routing_tables_6.txt','routing_tables_7.txt','routing_tables_8.txt','routing_tables_8.txt',...
     'MAC_Queue_0.txt','MAC_Queue_1.txt','MAC_Queue_2.txt','MAC_Queue_3.txt','MAC_Queue_4.txt',...
     'MAC_Queue_5.txt','MAC_Queue_6.txt','MAC_Queue_7.txt','MAC_Queue_8.txt',...
     'ContentionWindow_node_0.txt','ContentionWindow_node_1.txt','ContentionWindow_node_2.txt','ContentionWindow_node_3.txt','ContentionWindow_node_4.txt',...
     'ContentionWindow_node_5.txt','ContentionWindow_node_6.txt','ContentionWindow_node_7.txt','ContentionWindow_node_8.txt',...
     'Node_0_MAC_TX.txt','Node_1_MAC_TX.txt','Node_2_MAC_TX.txt','Node_3_MAC_TX.txt','Node_4_MAC_TX.txt',...
     'Node_5_MAC_TX.txt','Node_6_MAC_TX.txt','Node_7_MAC_TX.txt','Node_8_MAC_TX.txt',...
     'Node_0_MAC_RETX.txt','Node_1_MAC_RETX.txt','Node_2_MAC_RETX.txt','Node_3_MAC_RETX.txt','Node_4_MAC_RETX.txt',...
     'Node_5_MAC_RETX.txt','Node_6_MAC_RETX.txt','Node_7_MAC_RETX.txt','Node_8_MAC_RETX.txt'
     };

    for ii=1:size(arrayFILE,2)
        if(exist(arrayFILE{ii},'file')==0)
            check=0;
            break
        end
    end

end
