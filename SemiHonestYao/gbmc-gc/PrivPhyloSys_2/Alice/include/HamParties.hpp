#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <boost/filesystem.hpp>

#include "/home/manel/libscapi/include/comm/Comm.hpp"
#include "/home/manel/libscapi/include/infra/ConfigFile.hpp"

string preprocessInput(string path);
int hammingDistance(string input_i, string input_j);

class HamParty
{
private:

public:
    HamParty(int id, int numOfParties, int numOfInputs);

    int id;
    int numOfParties;
    int numOfInputs;

    vector<int> numOfInputsOtherParties;

    //ConfigFile configFile;
    
    vector<int> ports;
    vector<string> ips;

    vector<vector<int>> results;

    void runNumberOfInputs();

    void runHamSMC();

    void evaluatorSendResultToGarbler();

    void computeInternalHammingDistance();

    void sendAndReceiveOtherDistances();

    //vector<vector<int>> mat(int numOfcols, vector<int>(numOfrows)); // Definir a matrix!



};