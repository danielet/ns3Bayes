function main(varDir ,nameEXP , ctrlOther)

    simuTime=600; 
    sample=0.1;
    collect = 0; % set to 1 only when we found the correct starting point
    startTime = 200;
    if(checkAllFile() == 1)
        load 'CongestionWindow_node_0.txt'
% %         for i = 1:length(CongestionWindow_node_0)
% %             num2str(CongestionWindow_node_0(i,1),'%.1f');
% %             CongestionWindow_node_0(i,1) = str2num(ans);
% %         end

        CongestionWindow_node_0(:,1) = round(CongestionWindow_node_0(:,1)*10)/10;
        while (collect==0)
           startTime=startTime+1;
           if (find(CongestionWindow_node_0(:,1)*10==(startTime))~=0)
               collect=1;
           end
        end

        % startTime=20;
        startTime=startTime/10;
        fprintf('\nPHASE 1: processing parameters H CHAIN... \n\n')
        processParametersCHAIN1(startTime , simuTime, sample , varDir ,nameEXP ,ctrlOther ); 

        startTime = 200;
        % startTime = startTime * 10;
        load 'CongestionWindow_node_5.txt'
% %         for i = 1:length(CongestionWindow_node_5)
% %             num2str(CongestionWindow_node_5(i,1),'%.1f');
% %             CongestionWindow_node_5(i,1) = str2num(ans);
% %         end

        CongestionWindow_node_5(:,1) = round(CongestionWindow_node_5(:,1)*10)/10;
        while (collect==0)
           startTime=startTime+1;
           if (find(CongestionWindow_node_5(:,1)*10==(startTime))~=0)
               collect=1;
           end
        end

        startTime=startTime/10;
        % startTime=20;
        fprintf('\nPHASE 1: processing parameters V CHAIN... \n\n')
        processParametersCHAIN2(startTime , simuTime, sample,varDir ,nameEXP , ctrlOther ); 
    end
end