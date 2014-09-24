function main_SHORTEST(varDir ,nameEXP , ctrlOther)
    simuTime=600; 
    sample=0.1;
    collect = 0; % set to 1 only when we found the correct starting point
    startTime = 200;
    if(checkAllFile() == 1)
        load 'CongestionWindow_node_0.txt'

        CongestionWindow_node_0(:,1) = round(CongestionWindow_node_0(:,1)*10)/10;
        while (collect==0)
           startTime=startTime+1;
           if (find(CongestionWindow_node_0(:,1)*10==(startTime))~=0)
               collect=1;
           end
        end

        % startTime=20;
        startTime=startTime/10;
        fprintf('\nPHASE 1: processing parameters CHAIN... \n\n')
        processParametersCHAIN_SHORTEST(startTime , simuTime, sample , varDir ,nameEXP ,ctrlOther ); 	end
end
