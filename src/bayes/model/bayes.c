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
	mac_queue(0),
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
	mac_queue.cleat();
}

void 
Bayes::Setup (uint64_t M, double sampleTime, uint64_t n_nodes, std::vector<Ptr<YansWifiPhy> > tx_vector, std::vector<Ptr<DcaTxop> > retx_vector, std::vector<Ptr<olsr::RoutingProtocol> > olsr_vector , int percentageMoveTmp)
{
	//NAKAGAMI VALUE
    m_M = M;       
	m_sample = sampleTime;
	m_nodes = n_nodes;
	m_tx_vector = tx_vector;
	m_retx_vector = retx_vector;
	m_olsr_vector = olsr_vector;
	m_corruptedIPtables = false;
	m_discovery = false;
	percentageMove = percentageMoveTmp;
	for (uint64_t i = 0; i < n_nodes; i++)
	{
		m_tx_retx.push_back(std::pair<int,int>(0,0));
		m_tx_retx_temp.push_back(std::pair<int,int>(0,0));
	}
	
	switch (m_M){
		case 5:{
			double MATRIX_1_TMP[14]		=	{0.6902 ,   0.0365,    0.0472,    0.0944,    0.1197,    0.0119,   0.0002,    0.7742,    0.0376,    0.0294,    0.0691,    0.0829,    0.0067,    0.000031739};
			double MATRIX_2_TMP[10] 	= 	{0.7386,    0.0683,    0.0915,    0.0959,    0.0057,    0.8186,    0.0420,    0.0566,    0.0763,    0.0064};
			double MATRIX_3_TMP[8]		=	{0.9328,    0.0046,    0.0187,    0.0439,    0.9384,    0.0037,    0.0076,    0.0504};
			double MATRIX_4_TMP[10]		=	{0.9108,    0.0401,    0.0360,    0.0127,    0.0004,    0.9396,    0.0288,    0.0241,    0.0072,    0.0003};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);

		break;
		}
		case 10:{
			double MATRIX_1_TMP[14]		=	{0.6056,    0.0300,    0.0534,    0.1280,    0.1426,    0.0402,    0.0003,    0.7251,    0.0348,    0.0352,    0.0841,    0.0999,    0.0206,    0.0003};
			double MATRIX_2_TMP[10] 	= 	{0.6520,    0.1242,    0.1483,    0.0738,    0.0017,    0.7710,    0.0733,    0.0864,    0.0647,    0.0045};
			double MATRIX_3_TMP[8]		=	{0.9213,    0.0086,    0.0156,    0.0545,    0.9289,    0.0022,    0.0061,    0.0628};
			double MATRIX_4_TMP[10]		=	{0.8572,    0.0515,    0.0543,    0.0339,    0.0032,    0.9023,    0.0357,    0.0365,    0.0219,    0.0036};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
		break;
		}
		case 20:
		{
			double MATRIX_1_TMP[14]		=	{0.5041,    0.0217,    0.1079,    0.1707,    0.1351,    0.0601,    0.0004,    0.6751,    0.0304,    0.0406,    0.1035,    0.1080,    0.0418,    0.0006};
			double MATRIX_2_TMP[10] 	= 	{0.5586,    0.2217,    0.1742,    0.0449,    0.0005,    0.7187,    0.1142,    0.1129,    0.0508,    0.0035};
			double MATRIX_3_TMP[8]		=	{0.9242,    0.0028,    0.0167,    0.0563,    0.9354,    0.0004,    0.0032,    0.0609};
			double MATRIX_4_TMP[10]		=	{0.8130,    0.0497,    0.0531,    0.0590,    0.0252,    0.8714,    0.0350,    0.0401,    0.0378,    0.0157};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
		break;
		}
		case 50:{
			double MATRIX_1_TMP[14]		=	{0.4846,    0.0203,    0.1101,    0.1647,    0.1349,    0.0842,    0.0010,    0.6272,    0.0270,    0.0531,    0.1151,    0.1141,    0.0628,    0.0006};
			double MATRIX_2_TMP[10] 	= 	{0.5445,    0.2466,    0.1764,    0.0326,    0.0000,    0.6716,    0.1636,    0.1257,    0.0365,    0.0027};
			double MATRIX_3_TMP[8]		=	{0.9247,    0.0025,    0.0146,    0.0583,    0.9315,    0.0007,    0.0012,    0.0667};
			double MATRIX_4_TMP[10]		=	{0.7890,    0.0400,    0.0367,    0.0444,    0.0900,    0.8383,    0.0337,    0.0337,    0.0389,    0.0555};

			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
		break;
		}
		case 100:
		{
			double MATRIX_1_TMP[14]		=	{0.4479,    0.0172    0.1388    0.1874    0.1233    0.0839    0.0014    0.6273    0.0271    0.0536    0.1173    0.1066    0.0671    0.0010};
			double MATRIX_2_TMP[10] 	= 	{0.5118    0.2929    0.1701    0.0252    0.0000    0.6725    0.1697    0.1160    0.0365    0.0053};
			double MATRIX_3_TMP[8]		=	{0.9254    0.0038    0.0155    0.0553    0.9365    0.0001    0.0008    0.0626};
			double MATRIX_4_TMP[10]		=	{0.7877    0.0414    0.0346    0.0393    0.0970    0.8476    0.0330    0.0311    0.0354    0.0529};
			
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
		break;
		}
		default:{
			

			double MATRIX_1_TMP[14]		=	{0.4846,    0.0203,    0.1101,    0.1647,    0.1349,    0.0842,    0.0010,    0.6272,    0.0270,    0.0531,    0.1151,    0.1141,    0.0628,    0.0006};
			double MATRIX_2_TMP[10] 	= 	{0.5445,    0.2466,    0.1764,    0.0326,    0.0000,    0.6716,    0.1636,    0.1257,    0.0365,    0.0027};
			double MATRIX_3_TMP[8]		=	{0.9247,    0.0025,    0.0146,    0.0583,    0.9315,    0.0007,    0.0012,    0.0667};
			double MATRIX_4_TMP[10]		=	{0.7890,    0.0400,    0.0367,    0.0444,    0.0900,    0.8383,    0.0337,    0.0337,    0.0389,    0.0555};

			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

			MATRIX_4 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_4, MATRIX_4_TMP, sizeof(double) * 10);
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
			x_values[2]	=  4 - counter;
		
			x_values[3]	= mac_queue.at(i)->getSize();

			//FUNZIONE DI CALCOLO DELLE PROBABILITA'
			double * mobilityProbability = ComputationPosteriori(x_values);
		
			if (Simulator::Now().GetSeconds()>m_start)
			{
			// check whether we need to increase the rate of topology discovery messages		        		        

				        if ((mobilityProbability[0] * 100) >= percentageMove )          
						{
							NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << " " <<(mobilityProbability[0] * 100) << " " << percentageMove);			
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
			}//if	

		//MATTEO
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
		      fprintf(log_file, "%f\t %d \n", Simulator::Now().GetSeconds(), bayesCheck);
		      fflush(log_file);
		    }
		    else
		    {
		      FILE * log_file = FILE_DIC_BayesCtrl.at(i);
		      fprintf(log_file, "%f\t %d \n", Simulator::Now().GetSeconds(), bayesCheck );
		      fflush(log_file);
		    }

		
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
		// 	else if (m_corruptedIPtables)
		// 	{
		// 		if (!m_corr)
		// 		{
		// 			//NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << " increase OLSR msg rate because there are corrupted IP tables"); 
		// 			Bayes::ForceTopologyDiscovery();  
		// 			m_corr = true;
	 //                m_stability = false;
		// 		}
		// 	}	
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
	double level_queue[4] 		=	{1.5, 3.5, 7.5, 15.5};
	double level_table[3] 		=	{0.5, 1.5, 2.5};

	posterior[0]=0.5513;
	posterior[1]=1-posterior[0];
	for (ii	=	0; ii < 3 ; ii++)
	{
		switch (ii){
			case 0:
				quantize=0;
				while(x_values[ii] > level_mactx[quantize] && quantize < 6 ){
					quantize=quantize+1;
				}
				indexRow =  (quantize*2)-2;
				indexColumn = indexRow+1;
				posterior[0] = posterior[0] * MATRIX_1[indexRow];
				posterior[1] = posterior[1] * MATRIX_1[indexColumn];

			break;
			case 1:
				quantize=0;
				while(x_values[ii] > level_retx[quantize] && quantize < 4 ){
					quantize=quantize+1;
				}
				indexRow =  (quantize*2)-2;
				indexColumn = indexRow+1;
				posterior[0] = posterior[0] * MATRIX_2[indexRow];
				posterior[1] = posterior[1] * MATRIX_2[indexColumn];
			break;

			case 2:
				quantize=0;
				while(x_values[ii] > level_table[quantize] && quantize < 3){
					quantize=quantize+1;
				}
				indexRow =  (quantize*2)-2;
				indexColumn = indexRow+1;
				posterior[0] = posterior[0] * MATRIX_3[indexRow];
				posterior[1] = posterior[1] * MATRIX_3[indexColumn];
			break;

			case 3:
				quantize=0;
				while(x_values[ii] > level_queue[quantize] && quantize < 3){
					quantize=quantize+1;
				}
				indexRow =  (quantize*2)-2;
				indexColumn = indexRow+1;
				posterior[0] = posterior[0] * MATRIX_4[indexRow];
				posterior[1] = posterior[1] * MATRIX_4[indexColumn];
			break;
		
		}//switch
		
	}//for

	normPosterior 	= posterior[0] + posterior[1];
	posterior[0] 	= posterior[0] / normPosterior;
	posterior[1] 	= posterior[1] / normPosterior;
	return posterior;
}

} // namespace ns3







