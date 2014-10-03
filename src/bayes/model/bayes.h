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
#ifndef BAYES_H
#define BAYES_H

#include "ns3/net-device.h"
#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include <vector>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/olsr-routing-protocol.h"

namespace ns3 {

/**
 * \ingroup bayes
 * 
 * \brief Bayesian Learning
 *
*/

class Bayes : public Object
{

public:
	static TypeId GetTypeId (void);
	Bayes ();
	virtual ~Bayes();
	
	void Setup (uint64_t M, double sampleTime, uint64_t n_nodes, std::vector<Ptr<YansWifiPhy> > tx_vector, std::vector<Ptr<DcaTxop> > retx_vector, std::vector<Ptr<olsr::RoutingProtocol> > olsr_vector);
	void BayesIntervention (double start, double stop);        
	
	double GetStartTime(void);
	void Collect (void);
	void CheckTables (void);
	void ForceTopologyDiscovery (void);
	bool CorruptedTopology (void);
	
	bool m_corruptedIPtables;
	bool m_discovery;
	bool m_stability;
	bool m_corr;
	bool m_mob;

	
private:
    uint64_t m_M;
	double m_sample;
	double m_start;
	double m_stop;
	uint64_t m_nodes;
	std::vector<Ptr<YansWifiPhy> > m_tx_vector;
	std::vector<Ptr<DcaTxop> > m_retx_vector;
	std::vector<Ptr<olsr::RoutingProtocol> > m_olsr_vector;
	std::vector<std::pair<int,int> > m_tx_retx;
	std::vector<std::pair<int,int> > m_tx_retx_temp;

	static double bayes::MATRIX_1[14];
	static double bayes::MATRIX_2[10];
	static double bayes::MATRIX_3[8];

};

} // namespace ns3

#endif /* BAYES_H */
