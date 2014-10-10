#!/usr/bin/perl -w
use warnings;

############################ Files ##############################################
$seeds_file_name = "./script/seeds.txt";
#################################################################################

open (TRACE, ">trace.txt") or die "cannot open file trace.txt $! \n";
#Read the seeds that will be passed to the simulator for the different runs
open (SEEDS, "<$seeds_file_name") or die "cannot open seeds file $seeds_file_name $!\n";
@seed_lines = <SEEDS>;
close(SEEDS);

$tot_seeds = 10;
$line_seed = 1;

$BAYES 	= 	1;
$MOVE 	=	1;
$NODES	=	9;

@nakagami =  (5, 10, 20, 50, 100);
# @nakagami =  (5);
# @times = (30, 110, 190 , 270, 350, 430 , 460, 540);
@times = (70, 170, 270 , 370, 470, 570 , 670, 770);
# array schedule [30, 110, 190 ,270]
# @nakagami =  (50);

$percantageMove = 80;
for ($i=0; $i<=$#nakagami; $i++) {
	$line_seed = 1;
	for ($seed_counter=0; $seed_counter<$tot_seeds; $seed_counter++) {
		
		chop($seed_lines[$line_seed]);

		if( $BAYES == 1)
		{
			$run_subdir = "./NEWBAYESRUN_nakagamiM".$nakagami[$i]."_PERC_".$percantageMove."/seed".$seed_lines[$line_seed]."/";
		}
		else
		{
			$run_subdir = "./NEWNOBAYESRUN_nakagamiM".$nakagami[$i]."/seed".$seed_lines[$line_seed]."/";	
		}

		
	
		system("mkdir -p $run_subdir");	

		if ($MOVE == 1){
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
				$command_line_args = "$seed_lines[$line_seed] $nakagami[$i] $NODES $MOVE $BAYES $first_param  $second_param $thirst_param $fourth_param $fiveth_param $sixth_param $seventh_param $eigthth_param $percantageMove";
			}
			else{
				$command_line_args = "$seed_lines[$line_seed] $nakagami[$i] $NODES $MOVE $BAYES $first_param  $second_param $thirst_param $fourth_param $fiveth_param $sixth_param $seventh_param $eigthth_param ";
			}

			
			
		}else{
			$command_line_args = "$seed_lines[$line_seed] $nakagami[$i] $NODES $MOVE $BAYES";
		}
		
		#MobileNewScenarioMirror
		# testNoMobileNewScenario
		print TRACE "./waf --run MobileNewScenarioMirror_Bayes' $command_line_args'\n";
		#Launch the simulation
		system("./waf --run 'MobileNewScenarioMirror_Bayes  $command_line_args' ") == 0 or die "start ns error: $?\n";
		#Move output files into the proper subdirectories
		system("mv *.txt $run_subdir");
		system("mv *.routes $run_subdir");

		$line_seed++;
	}
} 		

close(TRACE);

