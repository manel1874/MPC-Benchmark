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

    //important to place the matrix elements
    int accumulatedValue_rows = 0;
    // for each party
    for(int i = 0; i < numOfParties; i++)
    {
        if(id == i)
        {
            int numOfInputs = numOfInputsOtherParties[i];
            // for each sequence
            for(int j = 0; j < numOfInputs; j++)
            {
                
                int accumulatedValue_columns = 0;
                // for each other parties
                for(int partyNum = 0; partyNum < numOfParties; partyNum++)
                {

                    int partyNumSeq = numOfInputsOtherParties[partyNum];
                    // for each seq of other party
                    for(int k = 0; k < partyNumSeq; k++)
                    {
                        if(partyNum != i && partyNumSeq != j)
                        {
                            // read from file
                            std::string newYaoOutputFileName = "results/out_myseq_" + to_string(j) + "_";
                            newYaoOutputFileName += "otherparty_";
                            newYaoOutputFileName += to_string(partyNum); 
                            newYaoOutputFileName += "_otherseq_" + to_string(k) + ".txt";

                            ifstream yaoOutput(newYaoOutputFileName);
                            std::string yaoResult_binaryString;
                            std::getline(yaoOutput, yaoResult_binaryString);

                            // convert from binary string to int
                            int yaoResult_int = std::stoi(yaoResult_binaryString, nullptr, 2);

                            if(accumulatedValue_columns > accumulatedValue_rows)
                            {
                                matrixDistance[accumulatedValue_rows][accumulatedValue_columns] = yaoResult_int;
                                cout << "Saved " + to_string(yaoResult_int) + " in place " + to_string(accumulatedValue_rows) + ", " + to_string(accumulatedValue_columns) << endl;
                            } else {
                                matrixDistance[accumulatedValue_columns][accumulatedValue_rows] = yaoResult_int;
                                cout << "Saved " + to_string(yaoResult_int) + " in place " + to_string(accumulatedValue_columns) + ", " + to_string(accumulatedValue_rows) << endl;
                            }
                           
                        }

                        accumulatedValue_columns += 1;
                    }
                
                }

                accumulatedValue_rows += 1;
            }
        } 
        // Compare if other elements are the same
    }
    
    
    this->mD = matrixDistance;



}
