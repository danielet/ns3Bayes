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
	m_olsr_vector(0)
{
}

Bayes::~Bayes()
{
	m_tx_vector.clear();
	m_retx_vector.clear();	
	m_olsr_vector.clear();	
	m_tx_retx.clear();
	m_tx_retx_temp.clear();
}

void 
Bayes::Setup (uint64_t M, double sampleTime, uint64_t n_nodes, std::vector<Ptr<YansWifiPhy> > tx_vector, std::vector<Ptr<DcaTxop> > retx_vector, std::vector<Ptr<olsr::RoutingProtocol> > olsr_vector)
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
	for (uint64_t i = 0; i < n_nodes; i++)
	{
		m_tx_retx.push_back(std::pair<int,int>(0,0));
		m_tx_retx_temp.push_back(std::pair<int,int>(0,0));
	}
	
	switch (m_M){
		case 5:{
			double MATRIX_1_TMP[14]		=	{0.6713,    0.7932,	0.0341,    0.0384, 0.0080,    0.0199, 0.0923,    0.0613, 0.0891,    0.0651, 0.1029,    0.0218, 0.0023,    0.0003};
			double MATRIX_2_TMP[10] 	= 	{0.7092,	0.8352,	0.0442,    0.0238, 0.1004,    0.0470, 0.1412, 0.0744, 0.0049,    0.0196};
			double MATRIX_3_TMP[8]		=	{0.3937,    0.8875, 0.2800,    0.0711, 0.1302,    0.0087, 0.1962,    0.0326};

			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);

		break;
		}
		case 10:{
			double MATRIX_1_TMP[14]	=	{0.6566 ,   0.7793, 0.0332, 0.0378, 0.0067, 0.0211,	0.0879, 0.0511,	0.0749,	0.0646,	0.1358,	0.0441,	0.0049,	0.0019};
			double MATRIX_2_TMP[10] = 	{0.6939	,   0.8215, 0.0736,	0.0287,	0.1467,	0.0634,	0.0858,	0.0719, 0.0001,	0.0145};
			double MATRIX_3_TMP[8]	=	{0.4254	,	0.8949,	0.2373, 0.0608, 0.1506, 0.0099, 0.1868,	0.0344};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);
		break;
		}
		case 20:
		{
			double MATRIX_1_TMP[14]	=	{0.7138,    0.7918, 0.0366, 0.0370 , 0.0052,    0.0175 ,0.0729,    0.0340 , 0.0549,    0.0653, 0.1095,    0.0487, 0.0071,    0.0056};
			double MATRIX_2_TMP[10] = 	{0.7534,    0.8330, 0.0791,    0.0316,0.1217,    0.0624, 0.0458,    0.0611, 0.000027229 ,    0.0119};
			double MATRIX_3_TMP[8]	=	{ 0.4336,    0.9510, 0.2037,    0.0334, 0.1554,    0.0010, 0.2073,    0.0145};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);
		break;
		}
		case 50:{
			double MATRIX_1_TMP[14]	=	{0.7335,0.0387,0.0038,0.0690,0.0471,0.0998,0.0080,0.8257,0.0389, 0.0106,    0.0207,    0.0553,    0.0405,    0.0082};
			double MATRIX_2_TMP[10] = 	{0.7747,    0.0766,    0.1130,    0.0356,    0.000036304,    0.8677,    0.0314,    0.0511,    0.0388,    0.0109};
			double MATRIX_3_TMP[8]	=	{0.4043,    0.1703,    0.1919,    0.2335,    0.9534,    0.0273,    0.000034631,    0.0194};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);
		break;
		}
		case 100:
		{
			double MATRIX_1_TMP[14]	=	{0.7109,    0.0353,    0.0031,    0.0783,    0.0512,    0.1092,    0.0120,    0.7737,    0.0350,0.0129, 0.0280,    0.0817,    0.0563,  0.0123};
			double MATRIX_2_TMP[10] = 	{0.7485,    0.0866,    0.1246,    0.0402,    0.000027229,    0.8118,    0.0504,    0.0660,    0.0521,    0.0196};
			double MATRIX_3_TMP[8]	=	{0.4072,    0.1690,    0.1971,    0.2267,    0.9602,    0.0237,    0.0015,    0.0147};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);
		break;
		}
		default:{
			double MATRIX_1_TMP[14]	=	{0.7335,0.0387,0.0038,0.0690,0.0471,0.0998,0.0080,0.8257,0.0389, 0.0106,    0.0207,    0.0553,    0.0405,    0.0082};
			double MATRIX_2_TMP[10] = 	{0.7747,    0.0766,    0.1130,    0.0356,    0.000036304,    0.8677,    0.0314,    0.0511,    0.0388,    0.0109};
			double MATRIX_3_TMP[8]	=	{0.4043,    0.1703,    0.1919,    0.2335,    0.9534,    0.0273,    0.000034631,    0.0194};
			MATRIX_1 = (double *) malloc(sizeof(double) * 14);
			memcpy(MATRIX_1, MATRIX_1_TMP, sizeof(double) * 14);
			MATRIX_2 = (double *) malloc(sizeof(double) * 10);
			memcpy(MATRIX_2, MATRIX_2_TMP, sizeof(double) * 10);
			MATRIX_3 = (double *) malloc(sizeof(double) * 8);
			memcpy(MATRIX_3, MATRIX_3_TMP, sizeof(double) * 8);
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
		double x_values[3];
		for ( i = 0; i < m_nodes; i++){
			m_tx_retx.at(i).first	=	m_tx_vector.at(i)->GetTxPackets()		- 	m_tx_retx_temp.at(i).first ;
			m_tx_retx.at(i).second =	m_retx_vector.at(i)->GetNumberRetx()	- 	m_tx_retx_temp.at(i).second;
			x_values[0]=m_tx_retx.at(i).first;
			x_values[1]=m_tx_retx.at(i).second;
			
			m_tx_retx_temp.at(i).first = m_tx_vector.at(i)->GetTxPackets();
			m_tx_retx_temp.at(i).second = m_retx_vector.at(i)->GetNumberRetx();

			std::map<Ipv4Address, olsr::RoutingTableEntry> check;
			check=m_olsr_vector.at(i)->GetTable(); 
			uint32_t counter = 0;
			for (std::map<Ipv4Address, olsr::RoutingTableEntry>::const_iterator iter = check.begin (); iter != check.end (); iter++)
			{
		        counter++;
			}
			x_values[2]= counter;
			//CHECK ROUTING TABLE VALUE
			double * mobilityProbability = ComputationPosteriori(x_values);
			//FUNZIONE DI CALCOLO DELLE PROBABILITA'
			if (Simulator::Now().GetSeconds()>m_start)
			{
			// check whether we need to increase the rate of topology discovery messages		        		        
				        if (mobilityProbability[0] > 0.75)          
						{
					        if (!m_discovery)
					        {
						        Bayes::ForceTopologyDiscovery();   
					        }
					        m_mob 		= true;                                   
					        m_discovery = true;
					        m_stability = false;
					        m_corr 		= false;				
			        	}
		        
			}//if	
		
	//NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << " m_discovery " << m_discovery << " m_stability " << m_stability << " m_corruptedIPtables " << m_corruptedIPtables << " m_corr " << m_corr); 	
		if (!m_mob)
		{
			if (!m_corruptedIPtables)
			{
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
			}
			else if (m_corruptedIPtables)
			{
				if (!m_corr)
				{
					//NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << " increase OLSR msg rate because there are corrupted IP tables"); 
					Bayes::ForceTopologyDiscovery();  
					m_corr = true;
	                m_stability = false;
				}
			}	
		}
	}
}



double * ComputationPosteriori(double * x_values )
{

	int ii, quantize;
	double normPosterior;
	double *posterior = (double *)malloc(sizeof(double) * 2);

	int indexRow , indexColumn =	0;
	double level_mactx[6] 		= 	{0.5000,    1.5000,   25.5000,   50.5000,   75.5000,  100.5000};
	double level_retx[4] 		= 	{0.5000,    6.5000,   12.5000,   24.5000};
	double level_table[3] 		=	{0.5, 1.5, 2.5};

	posterior[0]=0.4882;
	posterior[1]=1-posterior[0];
	for (ii	=	0; ii < 3 ; ii++)
	{

		
		switch (ii){
		case 1:
			quantize=0;
			while(x_values[ii] < level_mactx[quantize] && quantize < 6 ){
				quantize=quantize+1;
			}
			indexRow =  (quantize*2)-2;
			indexColumn = indexRow+1;
			posterior[0] = posterior[0] * MATRIX_1[indexRow];
			posterior[1] = posterior[1] * MATRIX_1[indexColumn];

		break;
		case 2:
			quantize=0;
			while(x_values[ii] < level_retx[quantize] && quantize < 4 ){
				quantize=quantize+1;
			}
			indexRow =  (quantize*2)-2;
			posterior[0] = posterior[0] * MATRIX_2[indexRow];
			posterior[1] = posterior[1] * MATRIX_2[indexColumn];
		break;

		case 3:
			quantize=0;

			while(x_values[ii] < level_table[quantize] && quantize < 3){
				quantize=quantize+1;
			}
			indexRow =  (quantize*2)-2;
			posterior[0] = posterior[0] * MATRIX_3[indexRow];
			posterior[1] = posterior[1] * MATRIX_3[indexColumn];
		break;
		
		}//switch
		
	}//for

	normPosterior 	= posterior[0] + posterior[1];
	posterior[0] 	= posterior[0] / normPosterior;
	posterior[1] 	= posterior[1] / normPosterior;
	return posterior;
}

} // namespace ns3







