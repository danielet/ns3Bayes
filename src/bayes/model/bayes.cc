/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Marco Mezzavilla  <mezzavil@dei.unipd.it>
 */

#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/assert.h"
#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/dca-txop.h"
#include "ns3/olsr-header.h"
#include "ns3/olsr-state.h"
#include "ns3/olsr-repositories.h"
#include "ns3/olsr-routing-protocol.h"
#include "ns3/enum.h"
#include "ns3/trace-source-accessor.h"
#include <math.h>


#include "ns3/log.h"
#include "bayes.h"

NS_LOG_COMPONENT_DEFINE ("Bayes");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Bayes);
double * MATRIX_1;
double * MATRIX_2;
double * MATRIX_3;
double * MATRIX_4;
double * ComputationPosteriori(double * x_values );

std::map<int , FILE *> Bayes::FILE_DIC_BayesCtrl;
TypeId 
Bayes::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Bayes")
    .SetParent<Object> ()
    .AddConstructor<Bayes> ()
  ;
  return tid;
}

Bayes::Bayes ()
  : m_corruptedIPtables(false),
	m_discovery(false),
	m_stability(false),	
	m_corr(false),	
	m_mob(false),
    m_M(0),
    m_sample(0),
    m_start(0),
    m_stop(0),
    m_nodes(0),
    m_tx_vector(0),
    m_retx_vector(0),
	m_olsr_vector(0),
	percentageMove(51),
	HoldTc_BAYES(10),
	HelloInterval_BAYES(0.5),
	TCInterval_BAYES(0.5)
{
}

Bayes::~Bayes()
{
	m_tx_vector.clear();
	m_retx_vector.clear();	
	m_olsr_vector.clear();	
	m_tx_retx.clear();
	m_tx_retx_temp.clear();
	// mac_queue.cleat();
}

void 
Bayes::Setup (uint64_t M, double sampleTime, uint64_t n_nodes, std::vector<Ptr<YansWifiPhy> > tx_vector, std::vector<Ptr<DcaTxop> > retx_vector, 
				std::vector<Ptr<olsr::RoutingProtocol> > olsr_vector , int percentageMoveTmp,
				double HoldTc_BAYES_tmp, double HelloInterval_BAYES_tmp, double TCInterval_BAYES_tmp)
{
// { std::vector<Ptr<DcaTxop> > mac_queueTmp , 
	//NAKAGAMI VALUE
    m_M = M;       
	m_sample = sampleTime;
	m_nodes = n_nodes;
	m_tx_vector = tx_vector;
	m_retx_vector = retx_vector;
	m_olsr_vector = olsr_vector;
	m_corruptedIPtables = false;
	m_discovery = false;

	// mac_queue = mac_queueTmp;
	percentageMove = percentageMoveTmp;
	NS_LOG_UNCOND("BAYES " << Simulator::Now().GetSeconds());			
	for (uint64_t i = 0; i < n_nodes; i++)
	{
		m_tx_retx.push_back(std::pair<int,int>(0,0));
		m_tx_retx_temp.push_back(std::pair<int,int>(0,0));
	}
	
	switch (m_M){
		case 5:{
			double MATRIX_1_TMP[14]		=	{0.644932, 0.757057, 0.038374, 0.037307, 0.026415, 0.023538, 0.032134, 0.038797, 0.232891, 0.132271, 0.024975, 0.010819, 0.000280, 0.000210};
			double MATRIX_2_TMP[10] 	= 	{0.694167, 0.802742, 0.004264, 0.003980, 0.035606, 0.021979, 0.132746, 0.063896, 0.129338, 0.098124};
			double MATRIX_3_TMP[8]		=	{0.490368, 0.805328, 0.230449, 0.142504, 0.131004, 0.034319, 0.148179, 0.017849};
			double MATRIX_4_TMP[10]		=	{0.554386, 0.675416, 0.375417, 0.282246, 0.050134, 0.032218, 0.018263, 0.009880, 0.001800, 0.000240};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);

		break;
		}
		case 10:{
			double MATRIX_1_TMP[14]		=	{0.526672, 0.687822, 0.028279, 0.032078, 0.041016, 0.034138, 0.063700, 0.045057, 0.259745, 0.165778, 0.080164, 0.034688, 0.000425, 0.000440};
			double MATRIX_2_TMP[10] 	= 	{0.569023, 0.732236, 0.009474, 0.007010, 0.114592, 0.053067, 0.241736, 0.119313, 0.064710, 0.081985};
			double MATRIX_3_TMP[8]		=	{0.481820, 0.825917, 0.176684, 0.142924, 0.134753, 0.022649, 0.206743, 0.008510};
			double MATRIX_4_TMP[10]		=	{0.411519, 0.567072, 0.445185, 0.340013, 0.083339, 0.055677, 0.052551, 0.034088, 0.007407, 0.003150};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
		break;
		}
		case 20:
		{
			double MATRIX_1_TMP[14]		=	{0.433948, 0.626266, 0.020704, 0.027508, 0.067820, 0.030128, 0.115737, 0.036137, 0.246639, 0.192687, 0.114535, 0.085854, 0.000617, 0.001420};
			double MATRIX_2_TMP[10] 	= 	{0.476570, 0.664770, 0.020456, 0.007080, 0.222218, 0.095384, 0.249311, 0.170140, 0.031373, 0.057557};
			double MATRIX_3_TMP[8]		=	{0.478518, 0.886425, 0.173293, 0.090926, 0.097071, 0.015589, 0.251118, 0.007060};
			double MATRIX_4_TMP[10]		=	{0.289672, 0.489906, 0.478802, 0.335683, 0.088654, 0.064917, 0.102280, 0.073616, 0.040592, 0.035878};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
		break;
		}
		case 50:{
			double MATRIX_1_TMP[14]		=	{0.399362, 0.591659, 0.017975, 0.024268, 0.088555, 0.036437, 0.140816, 0.038727, 0.225747, 0.188937, 0.126281, 0.117962, 0.001264, 0.002010};
			double MATRIX_2_TMP[10] 	= 	{0.445923, 0.628572, 0.029983, 0.010299, 0.267587, 0.139622, 0.230461, 0.168340, 0.026007, 0.048917};
			double MATRIX_3_TMP[8]		=	{0.490728, 0.933143, 0.122860, 0.052988, 0.082525, 0.007230, 0.303886, 0.006640};
			double MATRIX_4_TMP[10]		=	{0.234167, 0.434208, 0.489164, 0.342643, 0.058054, 0.052307, 0.072325, 0.066947, 0.146290, 0.103895};
			
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
		break;
		}
		case 100:
		{
			double MATRIX_1_TMP[14]		=	{0.376283, 0.591599, 0.015414, 0.024128, 0.101381, 0.030978, 0.138846, 0.031858, 0.226560, 0.183357, 0.139840, 0.136110, 0.001675, 0.001970};
			double MATRIX_2_TMP[10] 	= 	{0.424260, 0.626902, 0.034034, 0.009709, 0.277878, 0.152231, 0.234185, 0.164480, 0.029618, 0.044127};
			double MATRIX_3_TMP[8]		=	{0.513106, 0.952242, 0.126433, 0.036559, 0.072206, 0.006840, 0.288255, 0.004360};
			double MATRIX_4_TMP[10]		=	{0.204112, 0.424359, 0.495972, 0.337163, 0.056944, 0.052077, 0.064967, 0.057577, 0.178005, 0.128824};
			
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
		break;
		}		
	}

}

void 
Bayes::BayesIntervention (double start, double stop)
{
    m_start=start;
    m_stop=stop;        
	for (double t = start; t < stop; t = t + m_sample) 
    {
		Simulator::Schedule(Seconds(t),&Bayes::Collect,this); 
        Simulator::Schedule(Seconds(t),&Bayes::CheckTables,this);          
    }
}

double
Bayes::GetStartTime(void)
{
  return m_start;        
}



void 
Bayes::CheckTables()
{
     m_corruptedIPtables = false;
     for (uint64_t i = 0; i < m_nodes; i++)
	{				
	    std::map<Ipv4Address, olsr::RoutingTableEntry> check;
            check=m_olsr_vector.at(i)->GetTable(); 
            uint64_t counter=0;

            for (std::map<Ipv4Address, olsr::RoutingTableEntry>::const_iterator iter = check.begin ();
                  iter != check.end (); iter++)
            {
                counter++;
            }               
            if (counter < m_nodes-1)	
            {
                m_corruptedIPtables = true;
                break;
            }  
	}
}

void 
Bayes::ForceTopologyDiscovery()
{
	for (uint64_t i = 0; i < m_nodes; i++)
	{		
				//m_olsr_vector.at(i)->SetAttribute("HoldHello", TimeValue(Seconds(10)));                  
		        m_olsr_vector.at(i)->SetAttribute("HoldTc", TimeValue(Seconds(10)));                  
				m_olsr_vector.at(i)->SetAttribute("HelloInterval", TimeValue(Seconds(0.5))); 
				m_olsr_vector.at(i)->SetAttribute("TcInterval", TimeValue(Seconds(0.5)));         
                m_olsr_vector.at(i)->HelloTimerStop();                  
                m_olsr_vector.at(i)->TcTimerStop();
	}   
}

bool 
Bayes::CorruptedTopology()
{
     return m_corruptedIPtables;
}

void 
Bayes::Collect(void)
{
		m_mob = false;
		uint32_t i ;
		double x_values[4];
		// int bayesCheck;
		
		for ( i = 0; i < m_nodes; i++){

		

			// bayesCheck = 0;
			m_tx_retx.at(i).first	=	m_tx_vector.at(i)->GetTxPackets()		- 	m_tx_retx_temp.at(i).first ;
			m_tx_retx.at(i).second =	m_retx_vector.at(i)->GetNumberRetx()	- 	m_tx_retx_temp.at(i).second;
			
			x_values[0]=m_tx_retx.at(i).first;
			x_values[1]=m_tx_retx.at(i).second;
			
			m_tx_retx_temp.at(i).first = m_tx_vector.at(i)->GetTxPackets();
			m_tx_retx_temp.at(i).second = m_retx_vector.at(i)->GetNumberRetx();

			//devo aggiungere hook per recuperare i valori della coda

			std::map<Ipv4Address, olsr::RoutingTableEntry> check;
			check=m_olsr_vector.at(i)->GetTable(); 
			uint32_t counter = check.size();
			// for (std::map<Ipv4Address, olsr::RoutingTableEntry>::const_iterator iter = check.begin (); iter != check.end (); iter++)
			// {
		 //        counter++;
			// }
			// printf("Counter: %d \n" , counter);
			if(counter > 8)
				counter = 8;

			x_values[2]	=  8 - counter;
		
			// x_values[3]	= 1;
			

			//QUESTO POTREBBE CREARE PROBLEMI IN FASE DI RETRIEVING DELLE CODE
			Ptr<WifiMacQueue> m_queue = m_retx_vector.at(i)->GetQueue();
			
			// NS_LOG_UNCOND("BAYES " << Simulator::Now().GetSeconds() << " " << m_queue->GetSize());			
			x_values[3]	= m_queue->GetSize();
			


			// NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << x_values[3]);			
			//FUNZIONE DI CALCOLO DELLE PROBABILITA'
			double * mobilityProbability = ComputationPosteriori(x_values);
		
			if (Simulator::Now().GetSeconds()>m_start)
			{
			// check whether we need to increase the rate of topology discovery messages		        		        
					
				        if ((mobilityProbability[1] * 100) >= percentageMove )          
						{
							// NS_LOG_UNCOND(Simulator::Now().GetSeconds()<<" Node "<< i << " " << (mobilityProbability[1] * 100) << "tx:"<<x_values[0]<<"retx:"<<x_values[1]<<"t:"<<x_values[2] <<" Q:" << x_values[3]);					
					        if (!m_discovery)
					        {


						        // Bayes::ForceTopologyDiscovery();   
						        m_olsr_vector.at(i)->SetAttribute("HoldTc", TimeValue(Seconds(HoldTc_BAYES)));                  
								m_olsr_vector.at(i)->SetAttribute("HelloInterval", TimeValue(Seconds(HelloInterval_BAYES))); 
								m_olsr_vector.at(i)->SetAttribute("TcInterval", TimeValue(Seconds(TCInterval_BAYES)));         
				                m_olsr_vector.at(i)->HelloTimerStop();                  
				                m_olsr_vector.at(i)->TcTimerStop();

								if(FILE_DIC_BayesCtrl.find(i) == FILE_DIC_BayesCtrl.end())
								{
									FILE* log_file;
									char* fname = (char*)malloc(sizeof(char) * 255);  
									memset(fname, 0, sizeof(char) * 255);
									sprintf(fname, "BAYES_CHECK_by_node_%d.txt", i);

									log_file = fopen(fname, "w+");
									FILE_DIC_BayesCtrl[i] = log_file;
									if(fname)
									  free(fname);
									fprintf(log_file, "%f\t %f \n", Simulator::Now().GetSeconds(), mobilityProbability[1]);
									fflush(log_file);
								}
								else
								{
									FILE * log_file = FILE_DIC_BayesCtrl.at(i);
									fprintf(log_file, "%f\t %f \n", Simulator::Now().GetSeconds(), mobilityProbability[1] );
									fflush(log_file);
								}




					        }
					        m_mob 		= true;                                   
					        m_discovery = true;
					        m_stability = false;
					        m_corr 		= false;	
					        // bayesCheck = 1;	

					        break;		
			        	}
			        	// else
			        	// NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << " " <<(mobilityProbability[0] * 100) << " " << percentageMove);			

			}//if	

		// MATTEO
		  

		}//for
		
		//CHECKED IF THERE WAS MOBILITY IF YES STEP AWAY
		//OTHERWIRSE CHECK IF THE TABLES ARE CORRUPTED	

		if (!m_mob)
		{
			// if (!m_corruptedIPtables)
			// {
				if (!m_stability)
				{
					//NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << " no mobility prediction, relax the OLSR messages"); 
					for (uint64_t i = 0; i < m_nodes; i++)
					{	
						//m_olsr_vector.at(i)->SetAttribute("HoldHello", TimeValue(Seconds(40)));        			
						m_olsr_vector.at(i)->SetAttribute("HoldTc", TimeValue(Seconds(10)));        			
						m_olsr_vector.at(i)->SetAttribute("HelloInterval", TimeValue(Seconds(2))); 
						m_olsr_vector.at(i)->SetAttribute("TcInterval", TimeValue(Seconds(2)));              
						m_olsr_vector.at(i)->HelloTimerStop();                  
						m_olsr_vector.at(i)->TcTimerStop();    
					}
					m_stability = true;
					m_discovery = false;	
					m_corr = false;
				}
			// }
		}		
}



double * ComputationPosteriori(double * x_values )
{

	int ii, quantize;
	double normPosterior;
	double *posterior = (double *)malloc(sizeof(double) * 2);

	int indexRow , indexColumn 	=	0;
	double level_mactx[6] 		= 	{0.5000,    1.5000,   25.5000,   50.5000,   75.5000,  100.5000};
	double level_retx[4] 		= 	{0.5000,    6.5000,   12.5000,   24.5000};
	double level_table[3] 		=	{0.5, 1.5, 2.5};
	double level_queue[4] 		=	{1.5, 3.5, 7.5, 15.5};
	

	posterior[0]=0.5513;
	posterior[1]=1-posterior[0];
	for (ii	=	0; ii < 4 ; ii++)
	{
		switch (ii){
			case 0:
				quantize=0;
				while(x_values[ii] > level_mactx[quantize] && quantize < 6 ){
					quantize=quantize+1;
				}
				if(quantize > 0)
					indexRow =  (quantize*2)-2;
				else
					indexRow = 0;

				indexColumn = indexRow+1;
				posterior[0] = posterior[0] * MATRIX_1[indexRow];
				posterior[1] = posterior[1] * MATRIX_1[indexColumn];
				// printf("TX: %f %f \n" , posterior[0],posterior[1]);
				// printf("TX: INDEX:%d %f %f \n" , indexRow ,MATRIX_1[indexRow],MATRIX_1[indexColumn]);
			break;
			case 1:
				quantize=0;
				while(x_values[ii] > level_retx[quantize] && quantize < 4 ){
					quantize=quantize+1;
				}
				if(quantize > 0)
					indexRow =  (quantize*2)-2;
				else
					indexRow = 0;

				indexColumn = indexRow+1;
				posterior[0] = posterior[0] * MATRIX_2[indexRow];
				posterior[1] = posterior[1] * MATRIX_2[indexColumn];
				// printf("RT: %f %f \n" , posterior[0],posterior[1]);
				// printf("RT: %f %f \n" , MATRIX_2[indexRow],MATRIX_2[indexColumn]);
			break;

			case 2:
				quantize=0;
				while(x_values[ii] > level_table[quantize] && quantize < 3){
					quantize=quantize+1;
				}
				if(quantize > 0)
					indexRow =  (quantize*2)-2;
				else
					indexRow = 0;
				indexColumn = indexRow+1;
				posterior[0] = posterior[0] * MATRIX_3[indexRow];
				posterior[1] = posterior[1] * MATRIX_3[indexColumn];
				// printf("TA: %f %f \n" , MATRIX_3[indexRow],MATRIX_3[indexColumn]);			
			break;

			case 3:
				quantize=0;
				while(x_values[ii] > level_queue[quantize] && quantize < 4){
					quantize=quantize+1;
				}
				if(quantize > 0)
					indexRow =  (quantize*2)-2;
				else
					indexRow = 0;
				indexColumn = indexRow+1;
				posterior[0] = posterior[0] * MATRIX_4[indexRow];
				posterior[1] = posterior[1] * MATRIX_4[indexColumn];
				// printf("Q: %f %f \n" , posterior[0],posterior[1]);
				// printf("RT: %f %f \n" , MATRIX_4[indexRow],MATRIX_4[indexColumn]);
			break;
		
		}//switch
		
	}//for

	normPosterior 	= posterior[0] + posterior[1];
	posterior[0] 	= posterior[0] / normPosterior;
	posterior[1] 	= posterior[1] / normPosterior;
	return posterior;
}

} // namespace ns3







