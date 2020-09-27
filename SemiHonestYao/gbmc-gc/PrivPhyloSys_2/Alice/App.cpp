#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "/home/manel/libscapi/include/comm/Comm.hpp"
#include "/home/manel/libscapi/include/infra/ConfigFile.hpp"

#include "include/HamParties.hpp"


int main(int argc, char* argv[])
{

    //Initialization

    int numOfParties = 3;
    int partyNum = atoi(argv[1]);
    int numOfInputs = atoi(argv[2]);

    shared_ptr<HamParty> meHamParty = make_shared<HamParty>(partyNum, numOfParties, numOfInputs);

    cout<<"After HamParty"<<endl;

    // Send number of inputs to other parties

    // Run Hamming SMC between every party present
    meHamParty->runHamSMC();

    // Send (Evaluator) or Receive (Garbler) the results from Hamming SMC
    meHamParty->evaluatorSendResultToGarbler();

    // Compute internal Hamming distances


    // Send / Receive the other values

    return 0;

}



