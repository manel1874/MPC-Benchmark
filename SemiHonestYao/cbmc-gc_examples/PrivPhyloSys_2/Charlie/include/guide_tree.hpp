#ifndef GUIDE_TREE_HPP
#define GUIDE_TREE_HPP

#include <vector>
#include <string>

//#include "genome.hpp"

class GuideTree
{
public:
  GuideTree(int N, std::vector<std::vector<float>> mD, std::vector<std::string> mNodeNames);
  ~GuideTree();

  void CreateTree(std::string method_name);
  void Output(std::ostream& my_output);
  std::vector< std::vector<float> > Output();
  
private:
  // Member data
  int n, N;
  std::vector< std::vector<float> > mD;         // Distance matrix   
  std::vector< std::string > mNodeNames;        // Names of the nodes   
  std::vector< std::vector<float> > mTree;      // Tree matrix
  
  // Tree creation algorithms
  void Upgma();
  void NeighborJoining();
};

#endif // GUIDE_TREE_HPP
