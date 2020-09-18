#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "/home/manel/libscapi/include/comm/Comm.hpp"
#include "/home/manel/libscapi/include/infra/ConfigFile.hpp"


int main(int argc, char* argv[])
{
    //CmdParser parser;
    //auto parameters = parser.parseArguments("", argc, argv);
    //int numOfParties = stoi(parser.getValueByKey(parameters, "numOfParties"));
    //int partyNum = stoi(parser.getValueByKey(parameters, "partyID"));

    int numOfParties = 3;
    int partyNum = atoi(argv[1]);

    // Open file
    ConfigFile cf("Parties");

    string portString, ipString;
    vector<int> ports(numOfParties);
    vector<string> ips(numOfParties);
    int counter = 0;
    for(int i=0; i < numOfParties; i++)
    {
        portString = "party_" + to_string(i) + "_port";
        ipString = "party_" + to_string(i) + "_ip";
        // Get parties IPs and ports data
        ports[i] = stoi(cf.Value("", portString));
        ips[i] = cf.Value("", ipString);
    }

    SocketPartyData me, other;
    boost::asio::io_service io_service;

    for(int i=0; i < numOfParties; i++)
    {
        if (i < partyNum)
        {// This party will be the receiver in the protocol
            //me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), ports[partyNum] + i);
            //cout<<"my port = "<<ports[partyNum] + i<<endl;
            //other = SocketPartyData(boost_ip::address::from_string(ips[i]), ports[i] + partyNum - 1);
            //cout<<"other port = "<<ports[i] + partyNum - 1<<endl;

            //shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party one
            //channel->join(500, 5000);
            //cout<<"channel established"<<endl;

            // Create Parties file ===
            ofstream PartiesFile;
		    
            string partiesFile_name = "Parties_" + to_string(i) + "_" + to_string(partyNum);
            PartiesFile.open(partiesFile_name);

            PartiesFile << "party_0_ip = " + ips[partyNum] + "\n";
            PartiesFile << "party_1_ip = " + ips[i] + "\n";
            PartiesFile << "party_0_port = " + to_string(ports[partyNum] + i) + "\n";
            PartiesFile << "party_1_port = " + to_string(ports[i] + partyNum - 1) + "\n";

            PartiesFile.close();
            // ========================

            std::string run_script = "./runParty1.sh ";
            run_script += partiesFile_name;
            system(run_script.c_str());

        }else if (i>partyNum) 
        {// This party will be the sender in the protocol
            //me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), ports[partyNum] + i - 1);
            //cout<<"my port = "<<ports[partyNum] + i - 1<<endl;
            //other = SocketPartyData(boost_ip::address::from_string(ips[i]), ports[i] + partyNum);
            //cout<<"other port = "<< ports[i] + partyNum<<endl;

            //shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party one
            //channel->join(500, 5000);
            //cout<<"channel established"<<endl;

            // Create Parties file ===
            ofstream PartiesFile;
            string partiesFile_name = "Parties_" + to_string(partyNum) + "_" + to_string(i);
            PartiesFile.open(partiesFile_name);

            PartiesFile << "party_0_ip = " + ips[i] + "\n";
            PartiesFile << "party_1_ip = " + ips[partyNum]  + "\n";
            PartiesFile << "party_0_port = " + to_string(ports[i] + partyNum) + "\n";
            PartiesFile << "party_1_port = " + to_string(ports[partyNum] + i - 1) + "\n";

            PartiesFile.close();
            // ========================

            std::string run_script = "./runParty2.sh ";
            run_script += partiesFile_name;
            system(run_script.c_str());
        }
    }


}