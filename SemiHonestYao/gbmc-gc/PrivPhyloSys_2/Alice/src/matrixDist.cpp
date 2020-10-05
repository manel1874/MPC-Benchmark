#include "../include/matrixDist.hpp"

matrixDist::matrixDist(shared_ptr<HamParty> meParty)
{

    int id = meParty->id;
    int numOfParties = meParty->numOfParties;
    vector<int> numOfInputsOtherParties = meParty->numOfInputsOtherParties;

    int N = accumulate(numOfInputsOtherParties.begin(),numOfInputsOtherParties.end(),0);

    // create empty matrix
    std::vector< std::vector<float> > matrixDistance;
    matrixDistance = std::vector< std::vector<float>>(N, std::vector<float>(N, -1.0));

    // Create node names
    vector<string> nodeNames(N, "None");
    
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
    {
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

                            int yaoResult_int;
                            if(yaoResult_string.length() < 32)
                            {
                                cout << "here decimal" <<endl;
                                cout << i << j << partyNum << k << endl;
                                cout << yaoResult_string << endl;
                                yaoResult_int = std::stoi(yaoResult_string);
                            } else {
                                cout << "here binary" <<endl;
                                // convert from binary string to int
                                yaoResult_int = std::stoi(yaoResult_string, nullptr, 2);
                            }

                            
                            string row_str = to_string(i) + to_string(j);
                            int row = getIndex(nodeNames, row_str);

                            string column_str = to_string(partyNum) + to_string(k);
                            int column = getIndex(nodeNames, column_str);
                            
                            if(row < column)
                            {
                                matrixDistance[row][column] = yaoResult_int;
                                cout << "Saved " + to_string(yaoResult_int) + " in place " + to_string(row) + ", " + to_string(column) << endl;
                            } else {
                                matrixDistance[column][row] = yaoResult_int;
                                cout << "Saved " + to_string(yaoResult_int) + " in place " + to_string(column) + ", " + to_string(row) << endl;
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
