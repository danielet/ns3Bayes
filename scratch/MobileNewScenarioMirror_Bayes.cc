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
 */

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/bayes-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/tools-module.h"
#include "ns3/wifi-module.h"
#include "ns3/olsr-helper.h"
#include <ns3/flow-monitor-helper.h>
#include <ns3/flow-monitor.h>
#include "ns3/animation-interface.h"
#include "ns3/olsr-routing-protocol.h"
#include "ns3/object.h"

#include "ns3/flow-monitor-module.h"

#include "ns3/ipv4.h"


#include <cmath> 
#include <iostream>
#include <fstream>

using namespace ns3;
using namespace olsr;
std::ofstream fileMove;
Gnuplot2dDataset m_output;
NS_LOG_COMPONENT_DEFINE ("cognitive_net_mobility");
/*********************************************************************************************************************************
MOBILITY FUNCtiONS
********************************************************************************************************************************/
void 
Start_X(Ptr<ConstantVelocityMobilityModel> pos1, Ptr<ConstantVelocityMobilityModel> pos2 , int id1 , int id2 )
{
	//MODEL 1
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition());
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	pos1->SetVelocity(Vector (2.0, 0.0, 0.0));
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity());
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	//MODEL 2
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition());		
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";
	pos2->SetVelocity(Vector (-2.0, 0.0, 0.0));
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity());
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";
}

void 
Back_X(Ptr<ConstantVelocityMobilityModel> pos1, Ptr<ConstantVelocityMobilityModel> pos2 , int id1 , int id2 )
{
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition());
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	pos1->SetVelocity(Vector (-2.0, 0.0, 0.0));
	fileMove  << id1 << " " <<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity());
	
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition());	
	fileMove  << id2 << " "<<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";
	pos2->SetVelocity(Vector (2.0, 0.0, 0.0));
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity());
}

void 
Stop(Ptr<ConstantVelocityMobilityModel> pos1, Ptr<ConstantVelocityMobilityModel> pos2, int id1 , int id2)
{
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition());
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	pos1->SetVelocity(Vector (0.0, 0.0, 0.0));
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity());
	
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition());	
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";	
	pos2->SetVelocity(Vector (0.0, 0.0, 0.0));
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity());
}


void 
Start_Y(Ptr<ConstantVelocityMobilityModel> pos1, Ptr<ConstantVelocityMobilityModel> pos2, int id1 , int id2)
{
	//MODEL 1
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition());
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	pos1->SetVelocity(Vector (0.0, 2.0, 0.0));
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity());
	
	//MODEL 2
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition());		
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";	
	pos2->SetVelocity(Vector (0.0, -2.0, 0.0));
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity());
}

void 
Back_Y(Ptr<ConstantVelocityMobilityModel> pos1, Ptr<ConstantVelocityMobilityModel> pos2, int id1 , int id2)
{
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition());
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	pos1->SetVelocity(Vector (0.0, -2.0, 0.0));
	fileMove  << id1<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity() << " position "<< pos1->GetPosition() <<"\n";
	// NS_LOG_UNCOND("POS1 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos1->GetVelocity());
	
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition());		
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " old vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";	
	pos2->SetVelocity(Vector (0.0, 2.0, 0.0));
	fileMove  << id2<< " " <<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity() << " position "<< pos2->GetPosition() <<"\n";
	// NS_LOG_UNCOND("POS2 Time: "<<Simulator::Now().GetSeconds() << " new vel "<< pos2->GetVelocity());
}



Vector GetPosition (Ptr<Node> node)
	{
	// std::ostream prova;
	std::ostream os(void);
	Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> (); 
	Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);  
	Ipv4Address ipAddr = iaddr.GetLocal (); 

	// ipAddr.Print(&os );
	uint32_t ipAdd = ipAddr.Get();
	
	Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
	printf("NODE %2d -- %u.%u.%u.%-2u  (%4.2f;%3.2f;%3.2f;%3.2f)\n" , node->GetId() ,((ipAdd >> 24)  & 0xff) , ((ipAdd >> 16)  & 0xff) ,((ipAdd >> 8)  & 0xff) ,((ipAdd >> 0)  & 0xff)  ,mobility->GetPosition().x , mobility->GetPosition().y , mobility->GetVelocity().x ,mobility->GetVelocity().y);
	
	// fprintf(fileMove ,"NODE %2d -- %u.%u.%u.%-2u  (%4.2f;%3.2f;%3.2f;%3.2f)\n" , node->GetId(),
	// 	((ipAdd >> 24)  & 0xff) , ((ipAdd >> 16)  & 0xff) ,((ipAdd >> 8)  & 0xff) ,((ipAdd >> 0)  & 0xff)  ,mobility->GetPosition().x , mobility->GetPosition().y , mobility->GetVelocity().x ,mobility->GetVelocity().y);
	// m_output.Add(mobility->GetPosition().x , mobility->GetPosition().y);
	return mobility->GetPosition ();
}






void 
GetBayes(Ptr<TcpL4Protocol> tcpl4, Ptr<Bayes> bayes)
{
  tcpl4->AddBayestoTcpSocketBase(bayes);
}
	

/******************************************************************************
MAIN
*******************************************************************************/


int 
main (int argc, char *argv[])
{
	//LogComponentEnable ("OlsrRoutingProtocol", LOG_LEVEL_ALL);	
        if(argc < 9)
	{
		NS_LOG_UNCOND("Error in the command line. The program needs 2 input parameter, SEED number and M (for Nakagami channel)");
		exit(1);
	}
	

	int ii ;
	int SimuTime = 1000;


//PORT
	uint32_t maxBytes =1000000000; // xx MB
	//PARAMETRO CHE PASSO DA RIGA DI COMMANDO 
	


	fileMove.open("positionLog.txt");
	if (!fileMove.is_open()) { 
		printf("ERROR ---------- \n");
	}


	


	//PARAMETRI CHE RIGUARDANO IL CANALE
	SeedManager::SetRun(atof(argv[1])); // set the number of the current run
    uint64_t M 		= atof(argv[2]); // set the M for Nakagami Channel
	int n_nodes 	= atoi(argv[3]); // # nodes  
	int MOBILITY  	= atoi(argv[4]);
	int BAYES_FLAG	= atoi(argv[5]);
	int TCP_TRAFFIC = atoi(argv[6]);

//OLSR PARAM
	double HoldTc = atof(argv[7]);
	double HelloInterval = atof(argv[8]);
	double TCInterval = atof(argv[9]);

// NS_LOG_UNCOND("Run number: " << atof(argv[1])); //<< atof(argv[2]));              
	NodeContainer nodes;
	nodes.Create (n_nodes);

    //SELECTING  THE TCP ALGORITHM
	Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
/*********************************************
CHANNEL
***********************************************/
	WifiHelper wifi = WifiHelper::Default ();

	//SELECTING THE
	wifi.SetStandard (WIFI_PHY_STANDARD_80211a); 


	//CAPIRE I PARAMETRI D'INGRESSO
	YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
    wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel","Distance1",DoubleValue(5000),"m0",DoubleValue(M));


    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();	    
	YansWifiPhyHelper phy = wifiPhy;
	phy.SetChannel (wifiChannel.Create ());
    //DA CAPIRE
    phy.Set("TxPowerStart", DoubleValue(17));
    phy.Set("TxPowerEnd", DoubleValue(17));
	
	NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
	NqosWifiMacHelper mac = wifiMac;
		
	NetDeviceContainer devices;
	devices = wifi.Install (phy, mac, nodes);
/*********************************************
END CHANNEL
***********************************************/

/***********************************************
//TOPOLOGY
//SET TOPOLOGY 
INPUT: number of nodes
	   distance between 2 nodes
		single chain or cross chain
***********************************************/
//CREATE CHAIN
double mov_pos = 100.0;

if(MOBILITY == 0)
{		
	MobilityHelper mobility;
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
	double in_pos = 0.0;
	

	//FIRST H NODE SETTED UP
	positionAlloc->Add (Vector (in_pos, 0.0, 0.0));

	//HORIZONTAL CHAIN
	for (ii = 1; ii < (int)ceil((float)n_nodes/2); ii++)
	{
        in_pos = in_pos + mov_pos;
	    positionAlloc->Add (Vector (in_pos, 0.0, 0.0));
	}
	in_pos = 200.0;
	//FIRST V NODE SETTED UP
	
	for (;ii <= n_nodes; ii++)
	{
		if(ii != 7 )
	    	positionAlloc->Add (Vector (200.0,in_pos , 0.0));
    	in_pos = in_pos - mov_pos;
	}

	// mobility_inner.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
	//POSSO CREARE DIRETTAMENTE UN OGGETTO 


    mobility.SetPositionAllocator (positionAlloc);
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (nodes);
}
else
{
	
	MobilityHelper fix_node;
	Ptr<ListPositionAllocator> positionAlloc_fix = CreateObject<ListPositionAllocator> ();

	MobilityHelper mobile_node;
	Ptr<ListPositionAllocator> positionAlloc_mobile = CreateObject<ListPositionAllocator> ();

	double in_pos = 0.0;
	

	//FIRST H NODE SETTED UP
	positionAlloc_fix->Add (Vector (in_pos, 0.0, 0.0));

	//HORIZONTAL CHAIN
	for (ii = 1; ii < (int)ceil((float)n_nodes/2); ii++)
	{
        in_pos = in_pos + mov_pos;
        if(ii==4)
	    	positionAlloc_fix->Add (Vector (in_pos, 0.0, 0.0));
	    else
	    	positionAlloc_mobile->Add (Vector (in_pos, 0.0, 0.0));
	}
	in_pos = 200.0;
	//FIRST V NODE SETTED UP
	
	for (;ii <= n_nodes; ii++)
	{
		if(ii != 7 ){
			if(!(ii == 6 || ii == 8 )){
	    		positionAlloc_fix->Add (Vector (200.0,in_pos , 0.0));
	    	}else{
				positionAlloc_mobile->Add (Vector (200.0,in_pos , 0.0));
			}
		}
    	in_pos = in_pos - mov_pos;
	}

	// mobility_inner.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
	//POSSO CREARE DIRETTAMENTE UN OGGETTO 


    fix_node.SetPositionAllocator (positionAlloc_fix);
	fix_node.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

	mobile_node.SetPositionAllocator (positionAlloc_mobile);
	mobile_node.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");

	for (ii = 0; ii < n_nodes; ii++)
	{
		if((ii>0 && ii < 4) || (ii == 6) ||(ii == 7)){
			mobile_node.Install(nodes.Get(ii));      
		}else{
			fix_node.Install(nodes.Get(ii));      	
		}
	}		


}

/***********************************************
//MOBILITY PART
***********************************************/

//QUESTO E' IL MODELLO DI MOVIMENTO A CROCE    
	if(MOBILITY == 1){
		double times[8] = {atof(argv[10]), atof(argv[11]), atof(argv[12]), atof(argv[13]), atof(argv[14]), atof(argv[15]), atof(argv[16]), atof(argv[17])};
		Ptr<ConstantVelocityMobilityModel> model1 = nodes.Get(1)->GetObject<ConstantVelocityMobilityModel> ();             			
		Ptr<ConstantVelocityMobilityModel> model2 = nodes.Get(2)->GetObject<ConstantVelocityMobilityModel> ();             
		Ptr<ConstantVelocityMobilityModel> model3 = nodes.Get(3)->GetObject<ConstantVelocityMobilityModel> ();
		Ptr<ConstantVelocityMobilityModel> model6 = nodes.Get(6)->GetObject<ConstantVelocityMobilityModel> ();
		Ptr<ConstantVelocityMobilityModel> model7 = nodes.Get(7)->GetObject<ConstantVelocityMobilityModel> ();
		
	    for (int t = 0; t < 8; t++)
		{	
			switch (t){
				case 0:// CHANGE POSITION (1,2)
				Simulator::Schedule(Seconds(times[t]),&Start_X,model1,model2 , 1, 2);   
	            Simulator::Schedule(Seconds(times[t]+50),&Stop,model1,model2 , 1,  2);  
			break;
				case 1:// CHANGE POSITION (1,6)
				Simulator::Schedule(Seconds(times[t]),&Start_Y,model1,model6, 1 ,6);              
	            Simulator::Schedule(Seconds(times[t]+50),&Stop,model1,model6, 1, 6);
			break;
				case 2: //CHANGE POSITION (6,3)
				Simulator::Schedule(Seconds(times[t]),&Start_X,model6,model3 , 6 ,3);              
	            Simulator::Schedule(Seconds(times[t]+50),&Stop,model6,model3, 6 ,3);
			break;
//NEW
			case 3: //CHANGE POSITION (3,7)
				Simulator::Schedule(Seconds(times[t]),&Start_Y,model7,model3 , 7 ,3);              
	            Simulator::Schedule(Seconds(times[t]+50),&Stop,model7,model3, 7 ,3);
			break;
				
			case 4://CHANGE POSITION (7,3)
				Simulator::Schedule(Seconds(times[t]),&Back_Y,model7,model3 , 7 ,3);              
            	Simulator::Schedule(Seconds(times[t]+50),&Stop,model7,model3, 7 ,3);
			break;

//END NEW
			case 5://BACK CHANGE POSITION (3,6)
					Simulator::Schedule(Seconds(times[t]),&Back_X,model6,model3, 6 ,3); 
		            Simulator::Schedule(Seconds(times[t]+50),&Stop,model6,model3 , 6 ,3);            
			break;
			case 6: //BACK CHANGE POSITION (6,1)
				Simulator::Schedule(Seconds(times[t]),&Back_Y,model1,model6 , 1,6); 
	            Simulator::Schedule(Seconds(times[t]+50),&Stop,model1,model6, 1,6);            
			break;
			case 7: //BACK CHANGE POSITION (2,1)
				Simulator::Schedule(Seconds(times[t]),&Back_X,model1,model2, 1,2); 
	            Simulator::Schedule(Seconds(times[t]+50),&Stop,model1,model2,1,2);            
			break;
			}
	    }
	}


/***********************************************
//OLSR
***********************************************/

	OlsrHelper olsr; // OLSR
    Ipv4ListRoutingHelper list;
    //TO CHECK NUMBER 0
    list.Add (olsr, 0); // OLSR 

    InternetStackHelper stack;
	stack.SetRoutingHelper(olsr);
    stack.Install (nodes);

/***********************************************
//IP
***********************************************/
	Ipv4AddressHelper address;
	address.SetBase ("192.168.1.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = address.Assign (devices);



/**********************************************
CHECK NODE' POSITION AND IP ADDRESS
***********************************************/

for(ii=0; ii< n_nodes ; ii++){
	// printf("VALUE\n");
	GetPosition(nodes.Get(ii));
	nodes.Get(ii)->GetObject<Ipv4>()->GetRoutingProtocol()->SetAttribute("HoldTc", TimeValue(Seconds(HoldTc)));                  
	nodes.Get(ii)->GetObject<Ipv4>()->GetRoutingProtocol()->SetAttribute("HelloInterval", TimeValue(Seconds(HelloInterval))); 
	nodes.Get(ii)->GetObject<Ipv4>()->GetRoutingProtocol()->SetAttribute("TcInterval", TimeValue(Seconds(TCInterval)));    
	// 0.2
	// 0.5
			// nodes.Get(ii)->GetObject<Ipv4>()->GetRoutingProtocol()->SetAttribute("HoldTc", TimeValue(Seconds(100)));        			
				// nodes.Get(ii)->GetObject<Ipv4>()->GetRoutingProtocol()->SetAttribute("HelloInterval", TimeValue(Seconds(2))); 
				// nodes.Get(ii)->GetObject<Ipv4>()->GetRoutingProtocol()->SetAttribute("TcInterval", TimeValue(Seconds(5)));              
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************** 
			BAYES PART
*****************************/
// //QUESTA E' LA PARTE DI BAYES

if(BAYES_FLAG == 1){


	
	int percentageMove 			= atoi(argv[18]);	
	double HoldTc_BAYES  		= atof(argv[19]);
	double HelloInterval_BAYES 	= atof(argv[20]);
	double TCInterval_BAYES 	= atof(argv[21]);
	std::vector<Ptr<YansWifiPhy> > tx_vector;
	std::vector<Ptr<DcaTxop> > retx_vector;
    std::vector<Ptr<olsr::RoutingProtocol> > olsr_vector;		
    std::vector<Ptr<DcaTxop> > mac_queue;
	for (ii = 0; ii < n_nodes; ii++)
	{
	   tx_vector.push_back(devices.Get(ii)->GetObject<WifiNetDevice>()->GetPhy()->GetObject<WifiPhy>()->GetObject<YansWifiPhy>());			
	   retx_vector.push_back(devices.Get(ii)->GetObject<WifiNetDevice>()->GetMac()->GetObject<WifiMac>()->GetObject<RegularWifiMac>()->GetDcaTxop());		   
	   // mac_queue.push_back(devices.Get(ii)->GetObject<WifiNetDevice>()->GetMac()->GetObject<WifiMac>()->GetObject<RegularWifiMac>()->GetDcaTxop());	   
	   Ptr<Ipv4RoutingProtocol> prot = nodes.Get(ii)->GetObject<Ipv4>()->GetRoutingProtocol();
	   olsr_vector.push_back(DynamicCast<olsr::RoutingProtocol>(prot));   
	}

    double sampleTime = 0.1;
	Ptr<Bayes> bayes = CreateObject<Bayes> ();
	
	// bayes->Setup(M, sampleTime, n_nodes, tx_vector, retx_vector, olsr_vector, mac_queue ,percentageMove);	
	bayes->Setup(M, sampleTime, n_nodes, tx_vector, retx_vector, olsr_vector, percentageMove, HoldTc_BAYES, HelloInterval_BAYES, TCInterval_BAYES  );	
	bayes->BayesIntervention(51.0,SimuTime); 

 // //    //Extract the TCP SOCKET BASE to append Bayesian object
    Ptr<TcpL4Protocol> tcpl4 = nodes.Get(0)->GetObject<TcpL4Protocol>();
    Simulator::Schedule(Seconds(51.0),&GetBayes,tcpl4,bayes);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/************************************************************************************** 
				TRAFFIC GENERATOR
**************************************************************************************/
	
	
if(TCP_TRAFFIC == 1)
{


	// LogComponentEnable ("TcpTraceClient", LOG_LEVEL_INFO);
	// LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);


	uint32_t port = 20;
	//FIRST CHAIN
	Address sinkLocalAddressReceiver1(InetSocketAddress (interfaces.GetAddress ((int)floor((float)n_nodes/2)), port));
	BulkSendHelper sourceFTP ("ns3::TcpSocketFactory",sinkLocalAddressReceiver1);

	sourceFTP.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
	ApplicationContainer sourceAppsFTP = sourceFTP.Install (nodes.Get (0));       
	sourceAppsFTP.Start (Seconds (50.0));
	sourceAppsFTP.Stop (Seconds (1000.0 - 3 ));

	// CREATE a TCP receiver:
	Address sinkLocalAddress(InetSocketAddress (Ipv4Address::GetAny (), port));
	PacketSinkHelper sink ("ns3::TcpSocketFactory", sinkLocalAddress);
	ApplicationContainer sinkAppsTraffic = sink.Install (nodes.Get (floor(n_nodes/2)));
	sinkAppsTraffic.Start (Seconds (50.0));
	sinkAppsTraffic.Stop (Seconds (1000.0));


	NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << interfaces.GetAddress ((int)floor((float)n_nodes/2))); 
	printf("Node %d\n" , (int)floor((float)n_nodes/2));
	printf("Node 0\n" );
	printf("Node %d\n" , (int)ceil((float)n_nodes/2));
	printf("Node %d\n" , n_nodes-1);
	NS_LOG_UNCOND("At time " << Simulator::Now().GetSeconds() << interfaces.GetAddress (n_nodes-1)); 

	// // //SECOND CHAIN
	Address sinkLocalAddressReceiver2(InetSocketAddress (interfaces.GetAddress (n_nodes-1), port));
	BulkSendHelper source2FTP ("ns3::TcpSocketFactory",sinkLocalAddressReceiver2);
	source2FTP.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
	ApplicationContainer source2AppsFTP = source2FTP.Install (nodes.Get ((int)ceil((float)n_nodes/2)));       
	source2AppsFTP.Start (Seconds (50.0));
	source2AppsFTP.Stop (Seconds (1000.0));

	Address sinkLocalAddress2(InetSocketAddress (Ipv4Address::GetAny (), port));
	PacketSinkHelper sink2 ("ns3::TcpSocketFactory", sinkLocalAddress2);
	ApplicationContainer sink2AppsTraffic = sink2.Install (nodes.Get (n_nodes-1));
	sink2AppsTraffic.Start (Seconds (50.0));
	sink2AppsTraffic.Stop (Seconds (1000.0));
}
else
{


	// LogComponentEnable ("UdpTraceClient", LOG_LEVEL_INFO);
 //  	LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);

	NS_LOG_UNCOND("TEST");
	//UDP TRAFFIC Horizontal Chain
	uint16_t port = 4000;
	uint32_t MaxPacketSize = 1472;  // Back off 20 (IP) + 8 (UDP) bytes from MTU
	
	UdpServerHelper server_H (port);
  	ApplicationContainer apps_H = server_H.Install (nodes.Get ((int)floor((float)n_nodes/2)));
 	apps_H.Start (Seconds (50.0));
  	apps_H.Stop (Seconds (1000.0));

  	UdpTraceClientHelper client_H (interfaces.GetAddress ((int)floor((float)n_nodes/2)), port,"");
  	client_H.SetAttribute ("MaxPacketSize", UintegerValue (MaxPacketSize));
  	apps_H = client_H.Install (nodes.Get (0));
  	apps_H.Start (Seconds (50.0));
  	apps_H.Stop (Seconds (1000.0));

	//UDP TRAFFIC Horizontal Chain
	UdpServerHelper server_V (port);
  	ApplicationContainer apps_V = server_V.Install (nodes.Get (n_nodes-1));

 	apps_V.Start (Seconds (50.0));
  	apps_V.Stop (Seconds (1000.0));

  	UdpTraceClientHelper client_V (interfaces.GetAddress (n_nodes-1), port,"");
  	client_V.SetAttribute ("MaxPacketSize", UintegerValue (MaxPacketSize));
  	apps_V = client_V.Install (nodes.Get ((int)ceil((float)n_nodes/2)));
  	apps_V.Start (Seconds (50.0));
  	apps_H.Stop (Seconds (1000.0));
}

	//CAPIRE CHE INFO UTILI POTREBBE DARMI
      
      // Trace routing tables
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("wifi-simple-adhoc-grid.routes", std::ios::out);
      olsr.PrintRoutingTableAllEvery (Seconds (0.1), routingStream);


/************************************************************************************** 
						PCAP
**************************************************************************************/
	
	// phy.EnablePcap("testPCAP", 0, 1 , false);
	phy.EnablePcapAll("testPCAP", false);
// void EnablePcap (std::string prefix, uint32_t nodeid, uint32_t deviceid, bool promiscuous = false);
// Install FlowMonitor on all nodes



	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();

/************************************************************************************** 
				RUN SIMULATION
**************************************************************************************/


	Simulator::Stop (Seconds (SimuTime));
	Simulator::Run ();


/************************************************************************************** 
						FLOW MONITOR
**************************************************************************************/

	// Print per flow statistics
  	monitor->CheckForLostPackets ();
  	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

  	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end (); ++iter)
    {
	  Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
	
		if ((t.sourceAddress == Ipv4Address("192.168.1.1") && t.destinationAddress == Ipv4Address("192.168.1.5")) || (t.sourceAddress == Ipv4Address("192.168.1.6") && t.destinationAddress == Ipv4Address("192.168.1.9")))
        {

    	  NS_LOG_UNCOND("Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress);
    	  NS_LOG_UNCOND("Tx Packets = " << iter->second.txPackets);
    	  NS_LOG_UNCOND("Rx Packets = " << iter->second.rxPackets);
    	  NS_LOG_UNCOND("Throughput: "  << iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds()) / 1024  << " Kbps");
		}
    }
  	
  	monitor->SerializeToXmlFile("TEST.flowmon", true, true);


	Simulator::Destroy ();
	fileMove.close();
	return 0;
}
