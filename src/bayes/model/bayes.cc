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
			double MATRIX_1_TMP[14]		=	{0.7729,    0.0356,    0.0264,    0.0647,    0.0921,    0.0082,    0.0001,    0.7895,    0.0371,    0.0261,    0.0634,    0.0765,    0.0073,    0.0001};
			double MATRIX_2_TMP[10] 	= 	{0.8168,    0.0423,    0.0607,    0.0743,    0.0059,    0.8345,    0.0398,    0.0533,    0.0668,    0.0057};
			double MATRIX_3_TMP[8]		=	{0.9158,    0.0092,    0.0161,    0.0588,    0.9470,    0.0054,    0.0072,    0.0404};
			double MATRIX_4_TMP[10]		=	{0.9278,    0.0322,    0.0289,    0.0107,    0.0004,    0.9411,    0.0259,    0.0244,    0.0083,    0.0003};
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
			double MATRIX_1_TMP[14]		=	{0.6815,    0.0282,    0.0424,    0.1012,    0.1177,    0.0287,    0.0001,    0.6935,    0.0299,    0.0405,    0.0980,    0.1107,    0.0273,    0.0002};
			double MATRIX_2_TMP[10] 	= 	{0.7231,    0.0947,    0.1121,    0.0665,    0.0035,    0.7360,    0.0904,    0.1062,    0.0641,    0.0034};
			double MATRIX_3_TMP[8]		=	{0.9091,    0.0072,    0.0178,    0.0659,    0.9419,    0.0022,    0.0062,    0.0498};
			double MATRIX_4_TMP[10]		=	{0.8727,    0.0455,    0.0479,    0.0300,    0.0039,    0.8930,    0.0381,    0.0403,    0.0256,    0.0029};
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
			double MATRIX_1_TMP[14]		=	{0.6263,    0.0244,    0.0710,    0.1221,    0.1099,    0.0458,    0.0004,    0.6284,    0.0250,    0.0618,    0.1235,    0.1143,    0.0465,    0.0005};
			double MATRIX_2_TMP[10] 	= 	{0.6721,    0.1543,    0.1292,    0.0422,    0.0021,    0.6727,    0.1495,    0.1326,    0.0438,    0.0014};
			double MATRIX_3_TMP[8]		=	{0.9079,    0.0047,    0.0164,    0.0710,    0.9493,    0.0019,    0.0031,    0.0457};
			double MATRIX_4_TMP[10]		=	{0.8407,    0.0427,    0.0464,    0.0487,    0.0215,    0.8528,    0.0392,    0.0429,    0.0458,    0.0193};
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
			double MATRIX_1_TMP[14]		=	{0.5775,    0.0206,    0.0982,    0.1312,    0.1118,    0.0601,    0.0007,    0.5882,    0.0217,    0.0790,    0.1329,    0.1145,    0.0629,    0.0008};
			double MATRIX_2_TMP[10] 	= 	{0.6312,    0.2048,    0.1351,    0.0279,    0.0010,    0.6368,    0.1934,    0.1381,    0.0302,    0.0016};
			double MATRIX_3_TMP[8]		=	{0.9102,    0.0021,    0.0162,    0.0716,    0.9497,    0.0006,    0.0023,    0.0473};
			double MATRIX_4_TMP[10]		=	{0.8142,    0.0391,    0.0369,    0.0436,    0.0662,    0.8313,    0.0355,    0.0343,    0.0397,    0.0592};

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
			double MATRIX_1_TMP[14]		=	{0.5747,    0.0201,    0.0990,    0.1284,    0.1086,    0.0682,    0.0010,    0.5885,    0.0217,    0.0811,    0.1313,    0.1067,    0.0697,    0.0011};
			double MATRIX_2_TMP[10] 	= 	{0.6302,    0.2139,    0.1305,    0.0242,    0.0012,    0.6382,    0.2024,    0.1322,    0.0263,    0.0009};
			double MATRIX_3_TMP[8]		=	{0.9097,    0.0030,    0.0144,    0.0729,    0.9493,    0.0009,    0.0015,    0.0483};
			double MATRIX_4_TMP[10]		=	{0.8082,    0.0385,    0.0337,    0.0363,    0.0833,    0.8234,    0.0337,    0.0313,    0.0355,    0.0761};
			
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
		int bayesCheck;
		
		for ( i = 0; i < m_nodes; i++){

		

			bayesCheck = 0;
			m_tx_retx.at(i).first	=	m_tx_vector.at(i)->GetTxPackets()		- 	m_tx_retx_temp.at(i).first ;
			m_tx_retx.at(i).second =	m_retx_vector.at(i)->GetNumberRetx()	- 	m_tx_retx_temp.at(i).second;
			
			x_values[0]=m_tx_retx.at(i).first;
			x_values[1]=m_tx_retx.at(i).second;
			
			m_tx_retx_temp.at(i).first = m_tx_vector.at(i)->GetTxPackets();
			m_tx_retx_temp.at(i).second = m_retx_vector.at(i)->GetNumberRetx();

			//devo aggiungere hook per recuperare i valori della coda

			std::map<Ipv4Address, olsr::RoutingTableEntry> check;
			check=m_olsr_vector.at(i)->GetTable(); 
			uint32_t counter = 0;
			for (std::map<Ipv4Address, olsr::RoutingTableEntry>::const_iterator iter = check.begin (); iter != check.end (); iter++)
			{
		        counter++;
			}
			// printf("Counter: %d \n" , counter);
			if(counter > 8)
				counter =8;

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
					NS_LOG_UNCOND(Simulator::Now().GetSeconds()<<" Node "<< i << " " << (mobilityProbability[0] * 100) << "tx:"<<x_values[0]<<"retx:"<<x_values[1]<<"t:"<<x_values[2] <<" Q:" << x_values[3]);			
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
					        bayesCheck = 1;			
			        	}
			        	// else
			        	// NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << " " <<(mobilityProbability[0] * 100) << " " << percentageMove);			

			}//if	

		//MATTEO
		  // if(FILE_DIC_BayesCtrl.find(i) == FILE_DIC_BayesCtrl.end())
		  // {
		  //   FILE* log_file;
		  //   char* fname = (char*)malloc(sizeof(char) * 255);  
		  //   memset(fname, 0, sizeof(char) * 255);
		  //   sprintf(fname, "BAYES_CHECK_by_node_%d.txt", i);
		    
		  //   log_file = fopen(fname, "w+");
		  //   FILE_DIC_BayesCtrl[i] = log_file;
		  //   if(fname)
		  //     free(fname);
		  //     fprintf(log_file, "%f\t %d \n", Simulator::Now().GetSeconds(), bayesCheck);
		  //     fflush(log_file);
		  //   }
		  //   else
		  //   {
		  //     FILE * log_file = FILE_DIC_BayesCtrl.at(i);
		  //     fprintf(log_file, "%f\t %d \n", Simulator::Now().GetSeconds(), bayesCheck );
		  //     fflush(log_file);
		  //   }

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







