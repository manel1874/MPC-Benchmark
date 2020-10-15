#include "../include/HamParties.hpp"


HamParty::HamParty(int id, int numOfParties, int numOfInputs)
{
    this-> id = id;
    this-> numOfParties = numOfParties;
    this-> numOfInputs = numOfInputs;

    
    // Initialization
    std::vector<int> numOfInputsOtherParties(numOfParties, 0);
    numOfInputsOtherParties[id] = numOfInputs;
    this->numOfInputsOtherParties = numOfInputsOtherParties;
    
    
    // Extract ips and port vlaues
    ConfigFile cf("partiesFiles/Parties");

    string portString, ipString;
    vector<int> ports(numOfParties);
    vector<string> ips(numOfParties);
    //int counter = 0;
    for(int i=0; i < numOfParties; i++)
    {
        portString = "party_" + to_string(i) + "_port";
        ipString = "party_" + to_string(i) + "_ip";
        // Get parties IPs and ports data
        ports[i] = stoi(cf.Value("", portString));
        ips[i] = cf.Value("", ipString);
    }

    this->ports = ports;
    this->ips = ips;

    // Create YaoConfig files for each input
    for(int i=0; i < numOfInputs; i++)
    {
        ofstream yaoConfigFile;
        std::string yaoConfigFileName = "yaoConfigFiles/YaoConfig_seq_";
        yaoConfigFileName += to_string(i); 
        yaoConfigFileName += ".txt";

        yaoConfigFile.open(yaoConfigFileName);

        yaoConfigFile << "print_output = true\n";
        yaoConfigFile << "input_section = AES\n";
        yaoConfigFile << "circuit_type = NoFixedKey\n";
        yaoConfigFile << "\n";
        yaoConfigFile << "# OS name is added automatically\n";
        yaoConfigFile << "[AES-Linux]\n";
        yaoConfigFile << "circuit_file = boolCircuit/mainScapi.txt\n";
        yaoConfigFile << "input_file_party_1 = inputFiles/Party_" + to_string(id) + "_seq_" + to_string(i) + ".txt\n";
        yaoConfigFile << "input_file_party_2 = inputFiles/Party_" + to_string(id) + "_seq_" + to_string(i) + ".txt\n";

        yaoConfigFile.close();
    }

}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

//                                              PART 1.1 - SEND NUMBER OF INPUTS


void HamParty::runNumberOfInputs()
{
    int partyNum = this->id;
    int numOfParties = this->numOfParties;
    int numOfInputs = this->numOfInputs;

    vector<int> ports = this->ports;
    vector<string> ips = this->ips;

    SocketPartyData me, other;
    boost::asio::io_service io_service;

    // for each party
    for(int i=0; i < numOfParties; i++)
    {
        if(i < partyNum) 
        {// Evaluator - Send result

            int myPort = ports[partyNum] + i*10;
            int otherPort = ports[i] + partyNum*10 - 10;

            
            cout <<"SEND NUMBER OF INPUTS TO PARTY " + to_string(i) <<endl;
            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), myPort);
            cout<<"my port = "<< myPort <<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), otherPort);
            cout<<"other port = "<< otherPort <<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;
            
            cout <<"RECEIVE NUMBER OF INPUTS FROM PARTY " + to_string(i) <<endl;
            // send number of inputs
            channel->writeWithSize(to_string(numOfInputs)); // TODO: Encrypt the communication

            // Receive number of inputs from i
            string numberOfInputsOtherParty;
            vector<byte> raw_numberOfInputsOtherParty;
            channel->readWithSizeIntoVector(raw_numberOfInputsOtherParty); // TODO: Encrypt the communication
            const byte * uc = &(raw_numberOfInputsOtherParty[0]);
            numberOfInputsOtherParty = string(reinterpret_cast<char const *>(uc), raw_numberOfInputsOtherParty.size());

            this-> numOfInputsOtherParties[i] = stoi(numberOfInputsOtherParty);
            


        }else if(i > partyNum)
        {// Garbler - Receive result
            
            int myPort = ports[partyNum] + i*10 - 10;
            int otherPort = ports[i] + partyNum*10;

            
            cout <<"RECEIVE NUMBER OF INPUTS FROM PARTY " + to_string(i) <<endl;
            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), myPort);
            cout<<"my port = "<<myPort<<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), otherPort);
            cout<<"other port = "<<otherPort<<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;

            // Receive
            string numberOfInputsOtherParty;
            vector<byte> raw_numberOfInputsOtherParty;
            channel->readWithSizeIntoVector(raw_numberOfInputsOtherParty); // TODO: Encrypt the communication
            const byte * uc = &(raw_numberOfInputsOtherParty[0]);
            numberOfInputsOtherParty = string(reinterpret_cast<char const *>(uc), raw_numberOfInputsOtherParty.size());

            this-> numOfInputsOtherParties[i] = stoi(numberOfInputsOtherParty);

            cout <<"SEND NUMBER OF INPUTS TO PARTY " + to_string(i) <<endl;
            // send number of inputs
            channel->writeWithSize(to_string(numOfInputs)); // TODO: Encrypt the communication

            


        }
    }

}


//

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //




// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

//                                      PART 1.2 - RUN HAMMING SMC BETWEEN TWO PARTOES


void HamParty::runHamSMC()
{

    int partyNum = this->id;
    int numOfParties = this->numOfParties;

    vector<int> ports = this->ports;
    vector<string> ips = this->ips;

    // for each party
    for(int i=0; i < numOfParties; i++)
    {
        int numOfInputsOtherParty = this->numOfInputsOtherParties[i];

        if(i < partyNum)
        {// Evaluator

            cout << "EVALUATOR STARTING" << endl;

            int myPort = ports[partyNum] + i*10 - 10;
            int otherPort = ports[i] + partyNum*10;

            // Create Parties file ===========================
            ofstream PartiesFile;
            string partiesFile_name = "partiesFiles/Parties_" + to_string(partyNum) + "_" + to_string(i);
            PartiesFile.open(partiesFile_name);

            PartiesFile << "party_0_ip = " + ips[i] + "\n";
            PartiesFile << "party_1_ip = " + ips[partyNum]  + "\n";
            PartiesFile << "party_0_port = " + to_string(otherPort) + "\n";
            PartiesFile << "party_1_port = " + to_string(myPort) + "\n";

            PartiesFile.close();
            // ==============================================

            // SMC for all inputs 
            for(int j=0; j < numOfInputs; j++)
            {// for each myInput

                for(int k=0; k < numOfInputsOtherParty; k++)
                {

                    cout << "Computing hamming distance between myseq " + to_string(j) + " and otherseq " + to_string(k) << endl;
                    // Run SMC between evaluator and garbler=======
                    std::string run_script = "./runSMCParty.sh ";
                    run_script += to_string(1);
                    run_script += " yaoConfigFiles/YaoConfig_seq_" + to_string(j) + ".txt ";
                    run_script += partiesFile_name;

                    std::cout << "Running Yao protocol between my port "<< myPort << " and other port "<< otherPort <<endl;
                    // run ./sunSMCParty.sh
                    std::system(run_script.c_str()); 
                    std::cout <<"Finnished Yao protocol"<<endl;
                    std::cout <<"\n"<<endl;
                    // ============================================

                    // Rename output file =====
                    std::string newYaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                    newYaoOutputFileName += "otherparty_";
                    newYaoOutputFileName += to_string(i); 
                    newYaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";
                    rename("output_file.txt", newYaoOutputFileName.c_str());
                    // ========================
                }

            }



        }else if(i > partyNum)
        {// Garbler 

            cout << "GARBLER STARTING" << endl;

            int myPort = ports[partyNum] + i*10;
            int otherPort = ports[i] + partyNum*10 - 10;
            // Create Parties file ===
            ofstream PartiesFile;
		    
            string partiesFile_name = "partiesFiles/Parties_" + to_string(partyNum)  + "_" + to_string(i) ;
            PartiesFile.open(partiesFile_name);

            PartiesFile << "party_0_ip = " + ips[partyNum] + "\n";
            PartiesFile << "party_1_ip = " + ips[i] + "\n";
            PartiesFile << "party_0_port = " + to_string(myPort) + "\n";
            PartiesFile << "party_1_port = " + to_string(otherPort) + "\n";

            PartiesFile.close();
            // ========================

            // SMC for all inputs 
            for(int k=0; k < numOfInputsOtherParty; k++)
            {// for each myInput
                for(int j=0; j < numOfInputs; j++)
                {   
                    cout << "Computing hamming distance between myseq " + to_string(j) + " and otherseq " + to_string(k) << endl;
                    // Run SMC between evaluator and garbler=======
                    std::string run_script = "./runSMCParty.sh ";
                    run_script += to_string(0);
                    run_script += " yaoConfigFiles/YaoConfig_seq_" + to_string(j) + ".txt ";
                    run_script += partiesFile_name;
                    cout << "Running Yao protocol between my port "<< myPort << " and other port "<< otherPort <<endl;
                    // run ./sunSMCParty.sh
                    system(run_script.c_str());
                    cout <<"Finnished Yao protocol"<<endl;
                    cout <<"\n"<<endl;
                    // ============================================
                }
            }

        }



    }

};

//                                     

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

//                                      PART 1.3 - EVALUATOR SEND RESULTS TP GARBLER


void HamParty::evaluatorSendResultToGarbler()
{
    int partyNum = this->id;
    int numOfParties = this->numOfParties;

    vector<int> ports = this->ports;
    vector<string> ips = this->ips;

    SocketPartyData me, other;
    boost::asio::io_service io_service;

    for(int i=0; i < numOfParties; i++)
    {
        int numOfInputsOtherParty = this->numOfInputsOtherParties[i];

        if(i < partyNum)
        {// Evaluator - Send result

            int myPort = ports[partyNum] + i*10;
            int otherPort = ports[i] + partyNum*10 - 10;

            
            cout <<"SEND RESULTS"<<endl;
            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), myPort);
            cout<<"my port = "<< myPort <<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), otherPort);
            cout<<"other port = "<< otherPort <<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;

            // SMC for all inputs 
            for(int j=0; j < numOfInputs; j++)
            {// for each myInput

                for(int k=0; k < numOfInputsOtherParty; k++)
                {

                    std::string newYaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                    newYaoOutputFileName += "otherparty_";
                    newYaoOutputFileName += to_string(i); 
                    newYaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                    ifstream yaoOutput(newYaoOutputFileName);
                    std::string yaoResult;
                    std::getline(yaoOutput, yaoResult);

                    channel->writeWithSize(yaoResult); // TODO: Encrypt the communication
                }

            }
            


        }else if(i > partyNum)
        {// Garbler - Receive result
            
            int myPort = ports[partyNum] + i*10 - 10;
            int otherPort = ports[i] + partyNum*10;

            
            cout <<"RECEIVE RESULTS"<<endl;
            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), myPort);
            cout<<"my port = "<<myPort<<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), otherPort);
            cout<<"other port = "<<otherPort<<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;

            for(int k=0; k < numOfInputsOtherParty; k++)
            {// for each myInput
                for(int j=0; j < numOfInputs; j++)
                {  
                    // process
                    string yaoResult;
                    vector<byte> raw_yaoResult;
                    channel->readWithSizeIntoVector(raw_yaoResult); // TODO: Encrypt the communication
                    const byte * uc = &(raw_yaoResult[0]);
                    yaoResult = string(reinterpret_cast<char const *>(uc), raw_yaoResult.size());

                    // save to file
                    ofstream yaoOutputFile;

                    std::string YaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                    YaoOutputFileName += "otherparty_";
                    YaoOutputFileName += to_string(i); 
                    YaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";
                    
                    yaoOutputFile.open(YaoOutputFileName);
                    yaoOutputFile << yaoResult;
                    yaoOutputFile.close();
            
                }
            }   
        }
    }
}

//

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

//                                      PART 1.4 - COMPUTE INTERNAL HAMMING DISTANCE


void HamParty::computeInternalHammingDistance()
{
    int numOfInputs = this->numOfInputs;
    int id = this->id;

    for(int i = 0; i < numOfInputs; i++)
    {
        for(int j = i + 1; j < numOfInputs; j++)
        {

            // Converto format
            string filename_i = "inputFiles/Party_" + to_string(id) + "_seq_" + to_string(i) + ".txt";
            string inputSeq_i;
            inputSeq_i = preprocessInput(filename_i);

            string filename_j = "inputFiles/Party_" + to_string(id) + "_seq_" + to_string(j) + ".txt";
            string inputSeq_j;
            inputSeq_j = preprocessInput(filename_j);

            // Compute Hamming distance
            int hamDist;
            hamDist = hammingDistance(inputSeq_i, inputSeq_j);

            // save to file
            ofstream outputFile;

            std::string outputFileName = "results/out_myseq_" + to_string(i) + "_";
            outputFileName += "otherparty_";
            outputFileName += to_string(id); 
            outputFileName += "_otherseq_" + to_string(j) + ".txt";
            
            outputFile.open(outputFileName);
            outputFile << hamDist;
            outputFile.close();

        }
    }


}

//

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

//                                      PART 1.5 - SEND/RECEIVE OTHER DISTANCES


void HamParty::sendAndReceiveOtherDistances()
{

    int partyNum = this->id;
    int numOfParties = this->numOfParties;

    vector<int> ports = this->ports;
    vector<string> ips = this->ips;

    SocketPartyData me, other;
    boost::asio::io_service io_service;

    for(int i=0; i < numOfParties; i++)
    {
        int numOfInputsOtherParty = this->numOfInputsOtherParties[i];

        if(i < partyNum)
        {// Evaluator - Send result

            int myPort = ports[partyNum] + i*10;
            int otherPort = ports[i] + partyNum*10 - 10;

            
            cout <<"SEND RESULTS"<<endl;
            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), myPort);
            cout<<"my port = "<< myPort <<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), otherPort);
            cout<<"other port = "<< otherPort <<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;

            cout << "EVALUATOR INTERNAL SENDING..." << endl;
            // Send internal results
            for(int j = 0; j < numOfInputs; j++)
            {
                for(int k = j + 1; k < numOfInputs; k++)
                {   
                    cout << "I AM SENDING..." << endl;
                    std::string newYaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                    newYaoOutputFileName += "otherparty_";
                    newYaoOutputFileName += to_string(partyNum); 
                    newYaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                    ifstream yaoOutput(newYaoOutputFileName);
                    std::string yaoResult;
                    std::getline(yaoOutput, yaoResult);

                    channel->writeWithSize(yaoResult); // TODO: Encrypt the communication
                    cout << "I SENT" << endl;
                }
            }

            cout << "EVALUATOR RECEIVING..." << endl;
            // Receive internal results
            for(int j = 0; j < numOfInputsOtherParty; j++)
            {
                for(int k = j + 1; k < numOfInputsOtherParty; k++)
                {
                    // process
                    string yaoResult;
                    vector<byte> raw_yaoResult;
                    channel->readWithSizeIntoVector(raw_yaoResult); // TODO: Encrypt the communication
                    const byte * uc = &(raw_yaoResult[0]);
                    yaoResult = string(reinterpret_cast<char const *>(uc), raw_yaoResult.size());

                    // save to file
                    ofstream yaoOutputFile;

                    std::string YaoOutputFileName = "results/out_party_" + to_string(i) + "_";
                    YaoOutputFileName += "seq_" + to_string(j);
                    YaoOutputFileName += "_otherparty_";
                    YaoOutputFileName += to_string(i); 
                    YaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";
                    
                    yaoOutputFile.open(YaoOutputFileName);
                    yaoOutputFile << yaoResult;
                    yaoOutputFile.close();
                }
            }

            cout << "EVALUATOR OTHER SENDING..." << endl;
            // send other elements
            for(int restOfParties = 0; restOfParties < numOfParties; restOfParties++)
            {
                int numOfInputsRestOfParties = this->numOfInputsOtherParties[restOfParties];

                if(restOfParties != i && restOfParties != partyNum)
                {
                    for(int j = 0; j < numOfInputs; j++)
                    {
                        for(int k = 0; k < numOfInputsRestOfParties; k++)
                        {
                            cout << "Inside! j= " + to_string(j) + " and k= " + to_string(k) << endl;  
                            std::string newYaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                            newYaoOutputFileName += "otherparty_";
                            newYaoOutputFileName += to_string(restOfParties); 
                            newYaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                            cout << newYaoOutputFileName << endl;

                            ifstream yaoOutput(newYaoOutputFileName);
                            std::string yaoResult;
                            std::getline(yaoOutput, yaoResult);

                            channel->writeWithSize(yaoResult); // TODO: Encrypt the communication
                        }
                    }
                }
            }

            // Receive rest of parties
            for(int restOfParties = 0; restOfParties < numOfParties; restOfParties++)
            {
                int numOfInputsRestOfParties = this->numOfInputsOtherParties[restOfParties];
                if(restOfParties != partyNum && restOfParties != i)
                {
                    for(int j = 0; j < numOfInputsOtherParty; j++)
                    {
                        for(int k = 0; k < numOfInputsRestOfParties; k++)
                        {
                            // process
                            string yaoResult;
                            vector<byte> raw_yaoResult;
                            channel->readWithSizeIntoVector(raw_yaoResult); // TODO: Encrypt the communication
                            const byte * uc = &(raw_yaoResult[0]);
                            yaoResult = string(reinterpret_cast<char const *>(uc), raw_yaoResult.size());

                            // save to file
                            ofstream yaoOutputFile;

                            std::string YaoOutputFileName = "results/out_party_" + to_string(i) + "_";
                            YaoOutputFileName += "seq_" + to_string(j);
                            YaoOutputFileName += "_otherparty_";
                            YaoOutputFileName += to_string(restOfParties); 
                            YaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";
                            
                            yaoOutputFile.open(YaoOutputFileName);
                            yaoOutputFile << yaoResult;
                            yaoOutputFile.close();
                        }    
                    }
                }
            }




        } else if(i > partyNum) {

            int myPort = ports[partyNum] + i*10 - 10;
            int otherPort = ports[i] + partyNum*10;

            
            cout <<"RECEIVE RESULTS"<<endl;
            me = SocketPartyData(boost_ip::address::from_string(ips[partyNum]), myPort);
            cout<<"my port = "<<myPort<<endl;
            other = SocketPartyData(boost_ip::address::from_string(ips[i]), otherPort);
            cout<<"other port = "<<otherPort<<endl;

            shared_ptr<CommParty> channel = make_shared<CommPartyTCPSynced>(io_service, me, other);
            // connect to party i
            channel->join(500, 5000);
            cout<<"channel established"<<endl;


            cout << "GARBLER FIRST RECEIVING..." << endl;
            // Receive internal results
            for(int j = 0; j < numOfInputsOtherParty; j++)
            {
                for(int k = j + 1; k < numOfInputsOtherParty; k++)
                {
                    cout << "I AM RECEIVING..." << endl;
                    // process
                    string yaoResult;
                    vector<byte> raw_yaoResult;
                    channel->readWithSizeIntoVector(raw_yaoResult); // TODO: Encrypt the communication
                    const byte * uc = &(raw_yaoResult[0]);
                    yaoResult = string(reinterpret_cast<char const *>(uc), raw_yaoResult.size());

                    // save to file
                    ofstream yaoOutputFile;

                    std::string YaoOutputFileName = "results/out_party_" + to_string(i) + "_";
                    YaoOutputFileName += "seq_" + to_string(j);
                    YaoOutputFileName += "_otherparty_";
                    YaoOutputFileName += to_string(i); 
                    YaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";
                    
                    yaoOutputFile.open(YaoOutputFileName);
                    yaoOutputFile << yaoResult;
                    yaoOutputFile.close();
                }
            }

            cout << "GARBLER SENDING INTERNAL RESULTS..." << endl;
            // Send internal results
            for(int j = 0; j < numOfInputs; j++)
            {
                for(int k = j + 1; k < numOfInputs; k++)
                {

                    std::string newYaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                    newYaoOutputFileName += "otherparty_";
                    newYaoOutputFileName += to_string(partyNum); 
                    newYaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                    ifstream yaoOutput(newYaoOutputFileName);
                    std::string yaoResult;
                    std::getline(yaoOutput, yaoResult);

                    channel->writeWithSize(yaoResult); // TODO: Encrypt the communication
                }
            }

            cout << "GARBLER SECOND RECEIVING..." << endl;
            // Receive rest of parties
            for(int restOfParties = 0; restOfParties < numOfParties; restOfParties++)
            {
                int numOfInputsRestOfParties = this->numOfInputsOtherParties[restOfParties];
                
                if(restOfParties != partyNum && restOfParties != i)
                {
                    for(int j = 0; j < numOfInputsOtherParty; j++)
                    {
                        for(int k = 0; k < numOfInputsRestOfParties; k++)
                        {
                            // process
                            string yaoResult;
                            vector<byte> raw_yaoResult;
                            channel->readWithSizeIntoVector(raw_yaoResult); // TODO: Encrypt the communication
                            const byte * uc = &(raw_yaoResult[0]);
                            yaoResult = string(reinterpret_cast<char const *>(uc), raw_yaoResult.size());

                            // save to file
                            ofstream yaoOutputFile;

                            std::string YaoOutputFileName = "results/out_party_" + to_string(i) + "_";
                            YaoOutputFileName += "seq_" + to_string(j);
                            YaoOutputFileName += "_otherparty_";
                            YaoOutputFileName += to_string(restOfParties); 
                            YaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                            cout << YaoOutputFileName << endl;
                            
                            yaoOutputFile.open(YaoOutputFileName);
                            yaoOutputFile << yaoResult;
                            yaoOutputFile.close();
                        }    
                    }
                }
            }

            // send other elements
            for(int restOfParties = 0; restOfParties < numOfParties; restOfParties++)
            {
                int numOfInputsRestOfParties = this->numOfInputsOtherParties[restOfParties];

                if(restOfParties != partyNum && restOfParties != i)
                {
                    for(int j = 0; j < numOfInputs; j++)
                    {
                        for(int k = 0; k < numOfInputsRestOfParties; k++)
                        {
                        
                            std::string newYaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                            newYaoOutputFileName += "otherparty_";
                            newYaoOutputFileName += to_string(restOfParties); 
                            newYaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                            ifstream yaoOutput(newYaoOutputFileName);
                            std::string yaoResult;
                            std::getline(yaoOutput, yaoResult);

                            channel->writeWithSize(yaoResult); // TODO: Encrypt the communication
                        }
                    }
                }
            }




        }

    }

}

//

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

//                                                      Auxiliary functions


string preprocessInput(string path)
{
    
    int numOfDigits = 0;
    string sequence;
    string element;

    std::string line;    
    ifstream inputFile(path);

    string A = "00000000";
    string C = "10000000";
    string G = "01000000";
    string T = "11000000";

    while(std::getline(inputFile, line))
    {
        ++numOfDigits;
        element += line;

        if(numOfDigits == 8)
        {
            if(element == A)
                sequence += "A";
            if(element == C)
                sequence += "C";
            if(element == G)
                sequence += "G";
            if(element == T)
                sequence += "T";
            
            numOfDigits = 0;
            element = "";

        }

    }

    return sequence;

}


int hammingDistance(string input_i, string input_j)
{

    int hamDist = 0;

    int length;
    length = input_i.length();
    
    
    for(int i=0; i < length; i++)
    {
        if(input_i[i] != input_j[i])
            hamDist++;
    }

    return hamDist;
}

//

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //