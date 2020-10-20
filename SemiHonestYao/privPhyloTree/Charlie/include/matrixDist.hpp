#pragma once

#include <numeric>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <cmath>

#include "HamParties.hpp"


int getIndex(vector<string> v, string K);

class matrixDist
{
    private:
        

    public:
        matrixDist(shared_ptr<HamParty> meParty);

        std::vector< std::vector<float> > mD;
        
        vector<string> nodeNames;
        int n;


};














