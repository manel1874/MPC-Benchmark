#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <cmath>

#include "../include/guide_tree.hpp"


using namespace std;

GuideTree::GuideTree(int n, vector<vector<float>> mD, vector<string> mNodeNames) 
{// Changed by Manuel
  this->n = n;
  this->N = 2*n - 1;
  this->mD = mD;
  this->mNodeNames = mNodeNames;
  this->mTree = vector< vector<float> >(N-n, vector<float>(N, -1.0));
}

// Destructor
GuideTree::~GuideTree() {}
  
void GuideTree::CreateTree(string method_name = "upgma")
{
  if (method_name == "upgma") {
    Upgma();
  }
  else if (method_name == "neighbor_joining") {
    NeighborJoining();
  }
}

void GuideTree::Output(ostream& my_output)
{
  vector<string> tree_nwk(N, "");
  
  for (int i = 0; i < n; ++i) {
    tree_nwk[i] = mNodeNames[i];
  }
  
  bool line_empty;
  for (int i = 0; i < N - n; ++i) {
    line_empty = true;
    tree_nwk[i+n] = "(";
    for (int j = 0; j < N; ++j) {
      if (mTree[i][j] != -1.0) {
        line_empty = false;
        ostringstream dist;
        dist << mTree[i][j];
        tree_nwk[i+n] += tree_nwk[j] + ":" + dist.str() + ",";
      }
    }
    tree_nwk[i+n][tree_nwk[i+n].size()-1] = ')';
    if (line_empty) {
      my_output << tree_nwk[i+n-1] + ":0.05;";
      return;
    }
  }
  my_output << tree_nwk[N-1] + ":0.05";
}

vector< vector<float> > GuideTree::Output()
{
  return mTree;
}

void GuideTree::Upgma()
{
  // Heigh vector
  vector<float> h(N, 0.0);
  
  // Performs the algorithm
  for (int iter = 0; iter < n-1; ++iter) {
    
    // Find smaller element in matrix mD
    float smaller_element = FLT_MAX;
    int node_1 = -1, node_2 = -1;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < i; ++j) {
        if (mD[i][j] < smaller_element && mD[i][j] != -1.0 && i != j) {
          smaller_element = mD[i][j];
          node_1 = i;
          node_2 = j;
        }
      }
    }

    // Create the new node 
    int new_node = n + iter;
    
    h[new_node] = mD[node_1][node_2] / 2.0;

    float dist_1 = h[new_node] - h[node_1];
    float dist_2 = h[new_node] - h[node_2];
    
    mTree[iter][node_1] = dist_1;
    mTree[iter][node_2] = dist_2;  
    
    // Add new entry to 'mD' matrix
    for (int k = 0; k < N; ++k) {
      if (mD[node_1][k] != -1.0 && mD[k][node_2] != -1.0) {
        mD[new_node][k] = 0.5 * (mD[node_1][k] + mD[node_2][k]);
        mD[k][new_node] = mD[new_node][k];
      }
      mD[k][k] = 0;     // Diagonal elements are always 0
    }
    
    // Remove used columns and rows
    for (int k = 0; k < N; ++k) {
      mD[node_1][k] = -1.0; 
      mD[k][node_1] = -1.0;
      mD[node_2][k] = -1.0;
      mD[k][node_2] = -1.0;
    }
  }
}

void GuideTree::NeighborJoining()
{
  // Declare and initialize sum vector
  vector<float> sum(N, 0);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (mD[i][j] != -1.0) {
        sum[i] += mD[i][j];
      }
    }
  }
  
  // Declare and initialize optimization goal matrix
  vector< vector<float> > opt_goal(N, vector<float>(N, -1));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) {
      opt_goal[i][j] = (n - 2.0) * mD[i][j] - sum[i] - sum[j];
      opt_goal[j][i] = opt_goal[i][j];
    }
  }
  
  // Performs the algorithm
  for (int iter = 0; iter < n-2; ++iter) {
       
    // Find smaller element in opt_goal matrix
    float smaller_element = FLT_MAX;
    int node_1 = -1, node_2 = -1;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < i; ++j) {
        if (opt_goal[i][j] < smaller_element && mD[i][j] != -1.0) {
          smaller_element = opt_goal[i][j];
          node_1 = i;
          node_2 = j;
        }
      }
    }
    
    // Create new node
    int new_node = n + iter;
    int num_nodes = n - iter;
    
    float dist_1 = 0.5 * (mD[node_1][node_2] + (sum[node_1] - sum[node_2]) / (num_nodes - 2));
    float dist_2 = mD[node_1][node_2] - dist_1;
    
    mTree[iter][node_1] = dist_1;
    mTree[iter][node_2] = dist_2;  
    
    // New node is created, there is one element less
    num_nodes--;

    // Add new entries to 'mD' matrix
    for (int k = 0; k < N; ++k) {
      if (mD[node_1][k] != -1.0 && mD[k][node_2] != -1.0) {
        mD[new_node][k] = 0.5 * (mD[node_1][k] + mD[node_2][k] - mD[node_1][node_2]);
        mD[k][new_node] = mD[new_node][k];
      }
      mD[k][k] = 0;     // Diagonal elements are always 0
    }
      
    // Remove used entries
    for (int k = 0; k < N; ++k) {     
      mD[node_1][k] = -1.0;
      mD[node_2][k] = -1.0; 
      mD[k][node_1] = -1.0;
      mD[k][node_2] = -1.0;              
    }
    
    // Recalculate 'sum'
    sum = vector<float>(N, 0);
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if (mD[i][j] != -1.0) {
          sum[i] += mD[i][j];
        }
      }
    }

    // Recalculate 'opt_goal'
    opt_goal = vector< vector<float> >(N, vector<float>(N, -1));
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j <= i; ++j) {
        if (mD[i][j] != -1.0) {
          opt_goal[i][j] = (num_nodes - 2.0) * mD[i][j] - sum[i] - sum[j];
          opt_goal[j][i] = opt_goal[i][j];
        }
        else {
          opt_goal[i][j] = -1.0;
          opt_goal[j][i] = -1.0;
        }
      }
    }                                     
  } // End of the iteration

  // Find distance between last two nodes
  float dist = -1.0;
  int node_1 = -1, node_2 = -1;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < i; ++j) {
      if (mD[i][j] != -1.0) {
        dist = mD[i][j];
        node_1 = i;
        node_2 = j;
      }
    }
  }
  
  // Join last two nodes
  mTree[node_1 - n][node_2] = dist;
  
} // NeighborJoining
