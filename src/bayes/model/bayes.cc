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
Bayes::Collect(void)
{
	m_mob = false;
	for (uint64_t i = 0; i < m_nodes; i++)
	{				
		m_tx_retx.at(i).first=m_tx_vector.at(i)->GetTxPackets()-m_tx_retx_temp.at(i).first ;
		m_tx_retx.at(i).second=m_retx_vector.at(i)->GetNumberRetx()-m_tx_retx_temp.at(i).second;
		                                           
		m_tx_retx_temp.at(i).first = m_tx_vector.at(i)->GetTxPackets();
		m_tx_retx_temp.at(i).second = m_retx_vector.at(i)->GetNumberRetx();
	
		if (Simulator::Now().GetSeconds()>m_start)
		{
			// check whether we need to increase the rate of topology discovery messages
                        switch(m_M)
                        {
                                case(5):
			        if ((m_tx_retx.at(i).first>19 && m_tx_retx.at(i).first<41 && m_tx_retx.at(i).second>23) || 
                                   (m_tx_retx.at(i).first<21 && m_tx_retx.at(i).second>11 && m_tx_retx.at(i).second<18))          
			        {
				        if (!m_discovery)
				        {
					        Bayes::ForceTopologyDiscovery();   
				        }
				        m_mob = true;                                   
				        m_discovery = true;
				        m_stability = false;
				        m_corr = false;				
			        }
                                break;
                                case(10):
			        if ((m_tx_retx.at(i).first<21 && m_tx_retx.at(i).second>7) || 
                                   (m_tx_retx.at(i).first<41 && m_tx_retx.at(i).first>20 && m_tx_retx.at(i).second>21) ||
                                   (m_tx_retx.at(i).first<62 && m_tx_retx.at(i).first>40 && m_tx_retx.at(i).second>28) ||
                                   (m_tx_retx.at(i).first<82 && m_tx_retx.at(i).first>61 && m_tx_retx.at(i).second>28))
			        {
				        if (!m_discovery)
				        {
					        Bayes::ForceTopologyDiscovery();   
				        }
				        m_mob = true;                                   
				        m_discovery = true;
				        m_stability = false;
				        m_corr = false;				
			        }
                                break;
                                case(20):
			        if ((m_tx_retx.at(i).first<31 && m_tx_retx.at(i).second>12) || 
                                   (m_tx_retx.at(i).first<62 && m_tx_retx.at(i).first>30 && m_tx_retx.at(i).second>25) ||
                                   (m_tx_retx.at(i).first<92 && m_tx_retx.at(i).first>61 && m_tx_retx.at(i).second>25))       
			        {
				        if (!m_discovery)
				        {
					        Bayes::ForceTopologyDiscovery();   
				        }
				        m_mob = true;                                   
				        m_discovery = true;
				        m_stability = false;
				        m_corr = false;				
			        }
                                break;

                                case(50):
			        if ((m_tx_retx.at(i).first<35 && m_tx_retx.at(i).second>20) || 
                                   (m_tx_retx.at(i).first<69 && m_tx_retx.at(i).first>34 && m_tx_retx.at(i).second>25) ||
                                   (m_tx_retx.at(i).first<104 && m_tx_retx.at(i).first>68 && m_tx_retx.at(i).second>25))     
			        {
				        if (!m_discovery)
				        {
					        Bayes::ForceTopologyDiscovery();   
				        }
				        m_mob = true;                                   
				        m_discovery = true;
				        m_stability = false;
				        m_corr = false;				
			        }
                                break;
                                case(100):
			        if ((m_tx_retx.at(i).first<34 && m_tx_retx.at(i).second>13) || 
                                   (m_tx_retx.at(i).first>33 && m_tx_retx.at(i).second>26))        
			        {
				        if (!m_discovery)
				        {
					        Bayes::ForceTopologyDiscovery();   
				        }
				        m_mob = true;                                   
				        m_discovery = true;
				        m_stability = false;
				        m_corr = false;				
			        }
                                break;
                        }			
		}
	}	
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

} // namespace ns3
