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
			double MATRIX_1_TMP[14]		=	{0.741139, 0.786205, 0.033860, 0.036945, 0.034421, 0.028583, 0.079541, 0.060649, 0.101820, 0.080074, 0.009145, 0.007409, 0.000075, 0.000133};
			double MATRIX_2_TMP[10] 	= 	{0.784512, 0.831631, 0.052958, 0.040069, 0.070009, 0.055221, 0.085436, 0.068043, 0.007085, 0.005035};
			double MATRIX_3_TMP[8]		=	{0.912898, 0.944010, 0.010230, 0.004644, 0.019586, 0.006765, 0.057286, 0.044580};
			double MATRIX_4_TMP[10]		=	{0.919340, 0.939798, 0.035860, 0.026254, 0.031460, 0.024593, 0.012760, 0.008997, 0.000581, 0.000359};
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
			double MATRIX_1_TMP[14]		=	{0.666567, 0.683721, 0.027439, 0.029691, 0.050379, 0.046758, 0.107610, 0.101950, 0.118807, 0.111778, 0.028981, 0.025904, 0.000217, 0.000197};
			double MATRIX_2_TMP[10] 	= 	{0.709216, 0.727760, 0.103689, 0.095405, 0.116611, 0.109522, 0.067258, 0.064348, 0.003227, 0.002965};
			double MATRIX_3_TMP[8]		=	{0.909802, 0.942632, 0.007752, 0.002311, 0.019604, 0.005279, 0.062842, 0.049777};
			double MATRIX_4_TMP[10]		=	{0.867451, 0.887586, 0.047806, 0.040288, 0.049875, 0.042523, 0.031175, 0.026482, 0.003692, 0.003121};
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
			double MATRIX_1_TMP[14]		=	{0.612152, 0.616253, 0.023744, 0.024279, 0.073376, 0.064487, 0.126039, 0.124918, 0.116577, 0.119747, 0.047578, 0.049850, 0.000535, 0.000467};
			double MATRIX_2_TMP[10] 	= 	{0.658645, 0.661342, 0.162277, 0.152715, 0.134897, 0.138322, 0.042392, 0.045812, 0.001788, 0.001809};
			double MATRIX_3_TMP[8]		=	{0.909510, 0.946169, 0.004369, 0.001714, 0.015713, 0.002927, 0.070408, 0.049190};
			double MATRIX_4_TMP[10]		=	{0.831206, 0.845088, 0.044684, 0.040679, 0.049555, 0.045015, 0.052144, 0.047971, 0.022411, 0.021247};
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
			double MATRIX_1_TMP[14]		=	{0.562509, 0.574924, 0.019697, 0.021253, 0.108980, 0.087046, 0.132951, 0.138194, 0.114473, 0.116759, 0.060663, 0.061113, 0.000726, 0.000711};
			double MATRIX_2_TMP[10] 	= 	{0.619566, 0.625717, 0.213729, 0.203600, 0.136662, 0.138734, 0.028186, 0.030447, 0.001858, 0.001502};
			double MATRIX_3_TMP[8]		=	{0.908195, 0.948296, 0.002587, 0.000851, 0.015018, 0.001667, 0.074201, 0.049187};
			double MATRIX_4_TMP[10]		=	{0.809194, 0.826736, 0.040514, 0.037549, 0.036496, 0.035155, 0.042276, 0.038958, 0.071521, 0.061602};

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
			double MATRIX_1_TMP[14]		=	{0.530450, 0.570867, 0.017271, 0.020958, 0.126078, 0.098903, 0.147096, 0.133280, 0.113308, 0.108807, 0.064939, 0.066389, 0.000858, 0.000797};
			double MATRIX_2_TMP[10] 	= 	{0.591613, 0.626676, 0.247134, 0.215806, 0.136236, 0.131325, 0.023493, 0.024695, 0.001525, 0.001498};
			double MATRIX_3_TMP[8]		=	{0.911877, 0.951639, 0.002535, 0.000337, 0.014021, 0.001911, 0.071568, 0.046114};
			double MATRIX_4_TMP[10]		=	{0.801876, 0.820917, 0.041661, 0.037409, 0.035064, 0.033279, 0.037126, 0.035577, 0.084273, 0.072818};
			
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
					// NS_LOG_UNCOND(Simulator::Now().GetSeconds()<<" Node "<< i << " " << (mobilityProbability[0] * 100) << "tx:"<<x_values[0]<<"retx:"<<x_values[1]<<"t:"<<x_values[2] <<" Q:" << x_values[3]);			
				        if ((mobilityProbability[1] * 100) >= percentageMove )          
						{
							
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







