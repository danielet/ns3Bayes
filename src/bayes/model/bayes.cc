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
	percentageMove(51)
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
				std::vector<Ptr<olsr::RoutingProtocol> > olsr_vector , int percentageMoveTmp)
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
			double MATRIX_1_TMP[14]		=	{0.732706, 0.831023, 0.030755, 0.027404, 0.018422, 0.018227, 0.021915, 0.026632, 0.176794, 0.089847, 0.019262, 0.006816, 0.000147, 0.000050};
			double MATRIX_2_TMP[10] 	= 	{0.771197, 0.865216, 0.003231, 0.002794, 0.025959, 0.016277, 0.098633, 0.042982, 0.098504, 0.066876};
			double MATRIX_3_TMP[8]		=	{0.614794, 0.856814, 0.174041, 0.097309, 0.099220, 0.032760, 0.111945, 0.013116};
			double MATRIX_4_TMP[10]		=	{0.672425, 0.775923, 0.275821, 0.194100, 0.036990, 0.022716, 0.013453, 0.007083, 0.001311, 0.000178};
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
			double MATRIX_1_TMP[14]		=	{0.629738, 0.777128, 0.022120, 0.022919, 0.033242, 0.022929, 0.053329, 0.030794, 0.200604, 0.119066, 0.060692, 0.026869, 0.000277, 0.000295};
			double MATRIX_2_TMP[10] 	= 	{0.663499, 0.808621, 0.007571, 0.004490, 0.091164, 0.036809, 0.187238, 0.086817, 0.050035, 0.058488};
			double MATRIX_3_TMP[8]		=	{0.607698, 0.886942, 0.136579, 0.089458, 0.098592, 0.017615, 0.157132, 0.005985};
			double MATRIX_4_TMP[10]		=	{0.546714, 0.696248, 0.344735, 0.235449, 0.063341, 0.039959, 0.039306, 0.025564, 0.005904, 0.002780};
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
			double MATRIX_1_TMP[14]		=	{0.537716, 0.727065, 0.016785, 0.020944, 0.056327, 0.022624, 0.100370, 0.026669, 0.197177, 0.138985, 0.091076, 0.062658, 0.000549, 0.001055};
			double MATRIX_2_TMP[10] 	= 	{0.572555, 0.756722, 0.017719, 0.005330, 0.183354, 0.069933, 0.200909, 0.123231, 0.025294, 0.041284};
			double MATRIX_3_TMP[8]		=	{0.597734, 0.912622, 0.130667, 0.072764, 0.076404, 0.009965, 0.195195, 0.004650};
			double MATRIX_4_TMP[10]		=	{0.434797, 0.621744, 0.382429, 0.249529, 0.068556, 0.047549, 0.080099, 0.054314, 0.034120, 0.026864};
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
			double MATRIX_1_TMP[14]		=	{0.531253, 0.708210, 0.013272, 0.016379, 0.072466, 0.025554, 0.109793, 0.024009, 0.175139, 0.138160, 0.097117, 0.086497, 0.000960, 0.001190};
			double MATRIX_2_TMP[10] 	= 	{0.567994, 0.733998, 0.023683, 0.006985, 0.207475, 0.097137, 0.180260, 0.123056, 0.020548, 0.035784};
			double MATRIX_3_TMP[8]		=	{0.634046, 0.946511, 0.088415, 0.041839, 0.059219, 0.007260, 0.218321, 0.004390};
			double MATRIX_4_TMP[10]		=	{0.418220, 0.606235, 0.370937, 0.236114, 0.044083, 0.037264, 0.054259, 0.047264, 0.112502, 0.073123};
			
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
			double MATRIX_1_TMP[14]		=	{0.503910, 0.708705, 0.010829, 0.016774, 0.082074, 0.027059, 0.125624, 0.027859, 0.174312, 0.128481, 0.102258, 0.089767, 0.000994, 0.001355};
			double MATRIX_2_TMP[10] 	= 	{0.541013, 0.735423, 0.027839, 0.007825, 0.227602, 0.105582, 0.182465, 0.115027, 0.021062, 0.033579};
			double MATRIX_3_TMP[8]		=	{0.652935, 0.975235, 0.086379, 0.017030, 0.053113, 0.004545, 0.207572, 0.003190};
			double MATRIX_4_TMP[10]		=	{0.384786, 0.604280, 0.390577, 0.237949, 0.043290, 0.036034, 0.048248, 0.041879, 0.133100, 0.079858};
			
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
				m_olsr_vector.at(i)->SetAttribute("HelloInterval", TimeValue(Seconds(0.2))); 
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
							NS_LOG_UNCOND(Simulator::Now().GetSeconds()<<" Node "<< i << " " << (mobilityProbability[1] * 100) << "tx:"<<x_values[0]<<"retx:"<<x_values[1]<<"t:"<<x_values[2] <<" Q:" << x_values[3]);					
					        if (!m_discovery)
					        {
						        Bayes::ForceTopologyDiscovery();   
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
						m_olsr_vector.at(i)->SetAttribute("HoldTc", TimeValue(Seconds(100)));        			
						m_olsr_vector.at(i)->SetAttribute("HelloInterval", TimeValue(Seconds(2))); 
						m_olsr_vector.at(i)->SetAttribute("TcInterval", TimeValue(Seconds(5)));              
						m_olsr_vector.at(i)->HelloTimerStop();                  
						m_olsr_vector.at(i)->TcTimerStop();    
					}
					m_stability = true;
					m_discovery = false;	
					m_corr = false;
				}
			// }
		}
		// else
		// {
		// 	NS_LOG_UNCOND(Simulator::Now().GetSeconds()<<" Node "<< i << " " << (mobilityProbability[0] * 100) << " " <<(mobilityProbability[1] * 100) << " " << percentageMove );			
		// }
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







