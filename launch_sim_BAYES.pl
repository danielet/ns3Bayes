#!/usr/bin/perl 
# use warnings;

############################ Files ##############################################
$seeds_file_name = "/Volumes/DataMAC/ns3Bayes/script/seeds.txt";
# #################################################################################

open (TRACE, ">trace.txt") or die "cannot open file trace.txt $! \n";
# #Read the seeds that will be passed to the simulator for the different runs
open (SEEDS, "<$seeds_file_name") or die "cannot open seeds file $seeds_file_name $!\n";
# open (SEEDS, "<./script/seeds.txt") or die "cannot open seeds file $seeds_file_name $!\n";
@seed_lines = <SEEDS>;
# close(SEEDS);

$tot_seeds = 2;
$line_seed = 1;

$BAYES 	= 	1;
$MOVE 	=	1;
$NODES	=	9;

# 0 = UDP
# 1 = TCP
$TRAFFIC_TYPE = 1;


#HoldTC, Hello Frequency , TC Frequency
@OLSR_PARAM = (10 , 1.0 , 1.0);


@OLSR_PARAM_MOVE = (10 , 0.5 , 0.5);

# @nakagami =  (5, 10, 20, 50, 100);
@nakagami =  (5, 10, 20);
#@nakagami =  (50, 100);
# @times = (30, 110, 190 , 270, 350, 430 , 460, 540);
@times = (70, 170, 270, 370, 470, 570 , 670, 770);
# array schedule [30, 110, 190 ,270]
# @nakagami =  (50);



#BAYES PROBABILITY MOVE NODES
$percantageMove = 70;

if ($MOVE == 0)
{
	$nameEXPERIMENT = NEW_NOMOVE_TEST_OLSR
}
else
{
	$nameEXPERIMENT = NEW_MOVE
}

if ($TRAFFIC_TYPE == 0)
{
	$nameEXPERIMENT = $nameEXPERIMENT."_UDP"
}
else
{
	$nameEXPERIMENT = $nameEXPERIMENT."_TCP"
}

for ($i=0; $i<=$#nakagami; $i++) {
	$line_seed = 1;
	for ($seed_counter=0; $seed_counter<$tot_seeds; $seed_counter++) {
		
		# chop($seed_lines[$line_seed]);
		 $seedValue = $seed_lines[$line_seed];
		 print($seedValue);
		# $seedValue = 12;
		if( $BAYES == 1)
		{
			$run_subdir = "./".$nameEXPERIMENT."_nakagamiM".$nakagami[$i]."BAYES_PERC_".$percantageMove."/seed".$seedValue."/";
		}
		else
		{
			$run_subdir = "./".$nameEXPERIMENT."nakagamiM".$nakagami[$i]."/seed".$seedValue."/";
		}

		$olsrHoldTC = $OLSR_PARAM[0];
		$olsrHello = $OLSR_PARAM[1];
		$olsrTC = $OLSR_PARAM[2];
	
		system("mkdir -p $run_subdir");	

		if ($MOVE == 1)
		{
			$first_param 	= $times[0];
			$second_param 	= $times[1];	
			$thirst_param 	= $times[2];
			$fourth_param 	= $times[3];
			$fiveth_param 	= $times[4];
			$sixth_param 	= $times[5];
			$seventh_param 	= $times[6];
			$eigthth_param 	= $times[7];


			if( $BAYES == 1)
			{			
			
				$olsrHoldTC_MOVE = $OLSR_PARAM_MOVE[1];
				$olsrHello_MOVE = $OLSR_PARAM_MOVE[2];
				$olsrTC_MOVE = $OLSR_PARAM_MOVE[3];

				$command_line_args = "$seedValue $nakagami[$i] $NODES $MOVE $BAYES $TRAFFIC_TYPE $olsrHoldTC $olsrHello $olsrTC $first_param  
									 $second_param $thirst_param $fourth_param $fiveth_param $sixth_param $seventh_param $eigthth_param $percantageMove
									 $olsrHoldTC_MOVE $olsrHello_MOVE olsrTC_MOVE";
			
			}
			else
			{
			
				$command_line_args = "$seedValue $nakagami[$i] $NODES $MOVE $BAYES $TRAFFIC_TYPE $olsrHoldTC $olsrHello $olsrTC $first_param  $second_param $thirst_param $fourth_param $fiveth_param $sixth_param $seventh_param $eigthth_param ";
			
			}

			
			
		}else{


			$command_line_args = "$seedValue $nakagami[$i] $NODES $MOVE $BAYES $TRAFFIC_TYPE $olsrHoldTC $olsrHello $olsrTC ";
		}
		
		#MobileNewScenarioMirror
		# testNoMobileNewScenario
		print TRACE "./waf --run MobileNewScenarioMirror_Bayes --visualize' $command_line_args'\n";
		#Launch the simulation
		system("./waf --run 'MobileNewScenarioMirror_Bayes  $command_line_args' ") == 0 or die "start ns error: $?\n";
		#Move output files into the proper subdirectories
		system("mv *.txt $run_subdir");
		system("mv *.routes $run_subdir");
		system("mv *.pcap $run_subdir");
		system("mv *.flowmon $run_subdir");
		$line_seed++;
	}
} 		

close(TRACE);

