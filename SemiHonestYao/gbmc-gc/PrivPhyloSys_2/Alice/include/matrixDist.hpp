#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <cmath>

#include "HamParties.hpp"

class matrixDist
{
    private:
        

    public:
        matrixDist(shared_ptr<HamParty> meParty);

        std::vector< std::vector<float> > mD;


};














