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

    vector<string> hamming_distances(numOfParties * 1);
    hamming_distances[partyNum] = "00000000000000000000000000000000";

    for(int i=0; i < numOfParties; i++)
    {
        if (i < partyNum)
        {// Garbler (has to send the result to the garbler)

            int myPort = ports[partyNum] + i*10;
            int otherPort = ports[i] + partyNum*10 - 10;
            // Create Parties file ===
            ofstream PartiesFile;
		    
            string partiesFile_name = "Parties_" + to_string(partyNum)  + "_" + to_string(i) ;
            PartiesFile.open(partiesFile_name);

            PartiesFile << "party_0_ip = " + ips[partyNum] + "\n";
            PartiesFile << "party_1_ip = " + ips[i] + "\n";
            PartiesFile << "party_0_port = " + to_string(myPort) + "\n";
            PartiesFile << "party_1_port = " + to_string(otherPort) + "\n";

            PartiesFile.close();
            // ========================

            // Run Yao protocol =======
            std::string run_script = "./runParty1.sh ";
            run_script += partiesFile_name;
            cout << "Running Yao protocol between my port "<< myPort << " and other port "<< otherPort <<endl;
            system(run_script.c_str());
            cout <<"Finnished Yao protocol"<<endl;
            cout <<"\n"<<endl;
            // ========================
            
            // Receive result =========
            cout <<"RECEIVE RESULTS"<<endl;
            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), myPort);
            cout<<"my port = "<<myPort<<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), otherPort);
            cout<<"other port = "<<otherPort<<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;

            // process
            string yaoResult;
            vector<byte> raw_yaoResult;
            channel->readWithSizeIntoVector(raw_yaoResult); // TODO: Encrypt the communication
            const byte * uc = &(raw_yaoResult[0]);
            yaoResult = string(reinterpret_cast<char const *>(uc), raw_yaoResult.size());

            // save to file
            ofstream yaoOutputFile;

            std::string YaoOutputFileName = "out_myseq_1_";
            YaoOutputFileName += "otherparty_";
            YaoOutputFileName += to_string(i); 
            YaoOutputFileName += "_otherseq_1.txt";
            
            yaoOutputFile.open(YaoOutputFileName);
            yaoOutputFile << yaoResult;
            yaoOutputFile.close();
            // ==========================

            hamming_distances[i] = yaoResult;



        }else if (i>partyNum) 
        {// Evaluator (has to receive the result from evaluator)

            int myPort = ports[partyNum] + i*10 - 10;
            int otherPort = ports[i] + partyNum*10;

            // Create Parties file ===
            ofstream PartiesFile;
            string partiesFile_name = "Parties_" + to_string(partyNum) + "_" + to_string(i);
            PartiesFile.open(partiesFile_name);

            PartiesFile << "party_0_ip = " + ips[i] + "\n";
            PartiesFile << "party_1_ip = " + ips[partyNum]  + "\n";
            PartiesFile << "party_0_port = " + to_string(otherPort) + "\n";
            PartiesFile << "party_1_port = " + to_string(myPort) + "\n";

            PartiesFile.close();
            // ========================

            std::string run_script = "./runParty2.sh ";
            run_script += partiesFile_name;
            cout << "Running Yao protocol between my port "<< myPort << " and other port "<< otherPort<<endl;
            system(run_script.c_str());
            cout <<"Finnished Yao protocol"<<endl;
            cout <<"\n"<<endl;

            // Rename output file =====
            std::string newYaoOutputFileName = "out_myseq_1_";
            newYaoOutputFileName += "otherparty_";
            newYaoOutputFileName += to_string(i); 
            newYaoOutputFileName += "_otherseq_1.txt";
            rename("output_file.txt", newYaoOutputFileName.c_str());
            // ========================

            // Send result ============
            cout <<"SEND RESULTS"<<endl;
            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), myPort);
            cout<<"my port = "<< myPort <<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), otherPort);
            cout<<"other port = "<< otherPort <<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;
            // read from newYaoOutputFileName file
            ifstream yaoOutput(newYaoOutputFileName);
            std::string yaoResult;
            std::getline(yaoOutput, yaoResult);

            channel->writeWithSize(yaoResult); // TODO: Encrypt the communication
            // =========================

            hamming_distances[i] = yaoResult;


        }
    }

/*      for (int i=0; i<numOfParties; i++)
    {
        if (i < partyNum)
        {

            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), ports[partyNum] + i + 1000);
            cout<<"my port = "<<ports[partyNum] + i + 1000<<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), ports[i] + partyNum - 1 + 1000);
            cout<<"other port = "<<ports[i] + partyNum - 1 + 1000<<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;


            // ENVIAR OS QUE FALTAM AOS OUTROS

            for(int j=0; j<numOfParties; j++)
            {
                if(j != partyNum && j != i)
                {
                    channel->writeWithSize(to_string(j));
                    channel->writeWithSize(hamming_distances[j]); // TODO: Encrypt the communication
                }
            }

            // Receber os que faltam dos outros
            for(int j=0; j<numOfParties - 2; j++)
            {

                // Receive the id of the other party
                string otherPartyNumb;
                vector<byte> raw_otherPartyNumb;
                channel->readWithSizeIntoVector(raw_otherPartyNumb); // TODO: Encrypt the communication
                const byte * uc1 = &(raw_otherPartyNumb[0]);
                otherPartyNumb = string(reinterpret_cast<char const *>(uc1), raw_otherPartyNumb.size());

                string yaoResult;
                vector<byte> raw_yaoResult;
                channel->readWithSizeIntoVector(raw_yaoResult); // TODO: Encrypt the communication
                const byte * uc2 = &(raw_yaoResult[0]);
                yaoResult = string(reinterpret_cast<char const *>(uc2), raw_yaoResult.size());

                // save to file
                ofstream yaoOutputFile;

                std::string YaoOutputFileName = "out_";
                YaoOutputFileName += to_string(i); 
                YaoOutputFileName += "_1_";
                YaoOutputFileName += "otherparty_";
                YaoOutputFileName += otherPartyNumb; 
                YaoOutputFileName += "_otherseq_1.txt";
                
                yaoOutputFile.open(YaoOutputFileName);
                yaoOutputFile << yaoResult;
                yaoOutputFile.close();
                // ==========================
            }



        }else if (i>partyNum) 
        {

            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), ports[partyNum] + i - 1 + 1000);
            cout<<"my port = "<<ports[partyNum] + i - 1 + 1000<<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), ports[i] + partyNum + 1000);
            cout<<"other port = "<< ports[i] + partyNum + 1000<<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;

            // RECEBER OS QUE FALTAM DOS OUTROS
            for(int j=0; j<numOfParties - 2; j++)
            {

                // Receive the id of the other party
                string otherPartyNumb;
                vector<byte> raw_otherPartyNumb;
                channel->readWithSizeIntoVector(raw_otherPartyNumb); // TODO: Encrypt the communication
                const byte * uc3 = &(raw_otherPartyNumb[0]);
                otherPartyNumb = string(reinterpret_cast<char const *>(uc3), raw_otherPartyNumb.size());

                string yaoResult;
                vector<byte> raw_yaoResult;
                channel->readWithSizeIntoVector(raw_yaoResult); // TODO: Encrypt the communication
                const byte * uc4 = &(raw_yaoResult[0]);
                yaoResult = string(reinterpret_cast<char const *>(uc4), raw_yaoResult.size());

                // save to file
                ofstream yaoOutputFile;

                std::string YaoOutputFileName = "out_";
                YaoOutputFileName += to_string(i); 
                YaoOutputFileName += "_1_";
                YaoOutputFileName += "otherparty_";
                YaoOutputFileName += otherPartyNumb; 
                YaoOutputFileName += "_otherseq_1.txt";
                
                yaoOutputFile.open(YaoOutputFileName);
                yaoOutputFile << yaoResult;
                yaoOutputFile.close();
                // ==========================
            }

            // Enviar os que faltam aos outros

            for(int j=0; j<numOfParties; j++)
            {
                if(j != partyNum && j != i)
                {
                    channel->writeWithSize(to_string(j));
                    channel->writeWithSize(hamming_distances[j]); // TODO: Encrypt the communication
                }
            }

        }

    } */


}