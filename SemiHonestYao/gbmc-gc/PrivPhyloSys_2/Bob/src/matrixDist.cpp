#include "../include/matrixDist.hpp"

matrixDist::matrixDist(shared_ptr<HamParty> meParty)
{

    int id = meParty->id;
    int numOfParties = meParty->numOfParties;
    vector<int> numOfInputsOtherParties = meParty->numOfInputsOtherParties;

    this->n = accumulate(numOfInputsOtherParties.begin(),numOfInputsOtherParties.end(),0);

    int N = 2*n - 1;
    // create empty matrix
    std::vector< std::vector<float> > matrixDistance;
    matrixDistance = std::vector< std::vector<float>>(N, std::vector<float>(N, -1.0));

    // Introduce 0 in the diagonal
    for(int i = 0; i < n; i++)
    {
        matrixDistance[i][i] = 0.0;
    }

    // Create node names
    vector<string> nN(n, "None");
    this->nodeNames = nN;
    
    int index_n = 0;
    for(int i = 0; i< numOfParties; i++)
    {
        int numOfInputs = numOfInputsOtherParties[i];
        for(int j = 0; j < numOfInputs; j++)
        {
            nodeNames[index_n] = to_string(i) + to_string(j);
            index_n += 1;
        }
    }
   

    // for each party
    for(int i = 0; i < numOfParties; i++)
    {   // myseq txt style
        if(id == i)
        {
            int numOfInputs = numOfInputsOtherParties[i];
            // for each sequence
            for(int j = 0; j < numOfInputs; j++)
            {
                // for each other parties
                for(int partyNum = 0; partyNum < numOfParties; partyNum++)
                {

                    int partyNumSeq = numOfInputsOtherParties[partyNum];
                    // for each seq of other party
                    for(int k = 0; k < partyNumSeq; k++)
                    {
                        if( partyNum != i || j < k ) 
                        {

                            // read from file
                            std::string newYaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                            newYaoOutputFileName += "otherparty_";
                            newYaoOutputFileName += to_string(partyNum); 
                            newYaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                            ifstream yaoOutput(newYaoOutputFileName);
                            std::string yaoResult_string;
                            std::getline(yaoOutput, yaoResult_string);

                            float yaoResult_float;
                            if(yaoResult_string.length() < 32)
                            {
                                yaoResult_float = std::stof(yaoResult_string);
                            } else {
                                // convert from binary string to int
                                int yaoResult_int;
                                yaoResult_int = std::stoi(yaoResult_string, nullptr, 2);
                                yaoResult_float = (float) yaoResult_int;
                            }

                            
                            string row_str = to_string(i) + to_string(j);
                            int row = getIndex(nodeNames, row_str);

                            string column_str = to_string(partyNum) + to_string(k);
                            int column = getIndex(nodeNames, column_str);
                            
                            matrixDistance[row][column] = yaoResult_float;
                            matrixDistance[column][row] = yaoResult_float;
                        }
                    }
                }
            }
        } else { //others!


            int numOfInputs = numOfInputsOtherParties[i];
            // for each sequence
            for(int j = 0; j < numOfInputs; j++)
            {
                // for each other parties
                for(int partyNum = 0; partyNum < numOfParties; partyNum++)
                {

                    int partyNumSeq = numOfInputsOtherParties[partyNum];
                    // for each seq of other party
                    for(int k = 0; k < partyNumSeq; k++)
                    {
                        if( (partyNum == i && j < k) && (partyNum != id)) // CASE : internal computation
                        {

                            // read from file
                            std::string YaoOutputFileName = "results/out_party_" + to_string(i) + "_";
                            YaoOutputFileName += "seq_" + to_string(j);
                            YaoOutputFileName += "_otherparty_";
                            YaoOutputFileName += to_string(partyNum); 
                            YaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                            ifstream yaoOutput(YaoOutputFileName);
                            std::string yaoResult_string;
                            std::getline(yaoOutput, yaoResult_string);

                            float yaoResult_float;
                            if(yaoResult_string.length() < 32)
                            {
                                yaoResult_float = std::stof(yaoResult_string);
                            } else {
                                // convert from binary string to int
                                int yaoResult_int;
                                yaoResult_int = std::stoi(yaoResult_string, nullptr, 2);
                                yaoResult_float = (float) yaoResult_int;
                            }

                            
                            string row_str = to_string(i) + to_string(j);
                            int row = getIndex(nodeNames, row_str);

                            string column_str = to_string(partyNum) + to_string(k);
                            int column = getIndex(nodeNames, column_str);
                            
                            matrixDistance[row][column] = yaoResult_float;
                            matrixDistance[column][row] = yaoResult_float;

                        } else if (partyNum != id && partyNum > i)
                        {
                            // read from file
                            std::string YaoOutputFileName = "results/out_party_" + to_string(i) + "_";
                            YaoOutputFileName += "seq_" + to_string(j);
                            YaoOutputFileName += "_otherparty_";
                            YaoOutputFileName += to_string(partyNum); 
                            YaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                            ifstream yaoOutput(YaoOutputFileName);
                            std::string yaoResult_string;
                            std::getline(yaoOutput, yaoResult_string);

                            // read from file
                            std::string YaoOutputFileName_compare = "results/out_party_" + to_string(partyNum) + "_";
                            YaoOutputFileName_compare += "seq_" + to_string(k);
                            YaoOutputFileName_compare += "_otherparty_";
                            YaoOutputFileName_compare += to_string(i); 
                            YaoOutputFileName_compare += "_otherseq_" + to_string(j) + ".txt";

                            ifstream yaoOutput_compare(YaoOutputFileName_compare);
                            std::string yaoResult_string_compare;
                            std::getline(yaoOutput_compare, yaoResult_string_compare);

                            if (yaoResult_string == yaoResult_string_compare)
                            {
                                float yaoResult_float;
                                if(yaoResult_string.length() < 32)
                                {
                                    yaoResult_float = std::stof(yaoResult_string);
                                } else {
                                    // convert from binary string to int
                                    int yaoResult_int;
                                    yaoResult_int = std::stoi(yaoResult_string, nullptr, 2);
                                    yaoResult_float = (float) yaoResult_int;
                                }

                                
                                string row_str = to_string(i) + to_string(j);
                                int row = getIndex(nodeNames, row_str);

                                string column_str = to_string(partyNum) + to_string(k);
                                int column = getIndex(nodeNames, column_str);
                                
                                matrixDistance[row][column] = yaoResult_float;
                                matrixDistance[column][row] = yaoResult_float;
                            }
                            

                        }     
                    }
                }
            }


        }
        // Compare if other elements are the same
    }
    
    
    this->mD = matrixDistance;



}



int getIndex(vector<string> v, string K) 
{ 
    auto it = find(v.begin(), 
                   v.end(), K); 
  
    // If element was found 
    if (it != v.end()) { 
        // calculating the index 
        // of K 
        int index = distance(v.begin(), 
                             it); 
        return index; 
    } 
    else { 
        // If the element is not 
        // present in the vector 
        return -1; 
    } 
} 
