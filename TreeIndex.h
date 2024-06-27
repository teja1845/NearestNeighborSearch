// #include"DataVector.h"
// #include"VectorDataset.h"
#include"VectorDataset.cpp"
#include <queue>
#include<stack>




class Node{
    public:
    VectorDataset dataset;
    Node*prt,*left,*rt;
    double b_val;
    int b_ind;

    double calMedian(int xi);
    double check();
};

class TreeIndex {



protected:
 TreeIndex() ;
 ~TreeIndex(){} ;
 static TreeIndex* instance ;
 Node root;
 
public:
 static TreeIndex& GetInstance();
 TreeIndex& AddData(DataVector d);
 TreeIndex& DeleteData(DataVector d);
 //DataVector getFarthestPoint(const DataVector& x, const VectorDataset& dataset) ;
 void deleteTree(Node* currentNode) ;
 void kNearestNeighbours(int k,DataVector d);
void kNearestNeighboursRecursive(int k, DataVector d, Node* currentNode, std::vector<std::pair<double, DataVector>>& pq);
Node* findPos(DataVector d, Node* currentNode,int k);
bool isInPartition(DataVector d, Node* parentNode);
 
 
};

class KDTreeIndex : public TreeIndex {
public:
 static KDTreeIndex& GetInstance();
 bool chooseRule(int i,  DataVector d,double med);
 void MakeTree(Node*& currentNode, int dimensionIndex );
 KDTreeIndex& AddData(DataVector d);
 
private:
 KDTreeIndex(){};
 ~KDTreeIndex() {};
 static KDTreeIndex* inst1 ;

};


class RPTreeIndex : public TreeIndex {
public:
 static RPTreeIndex& GetInstance();
 bool chooseRule(int i, DataVector d, double med) ;
 void MakeTree(Node*& currentNode, int dimensionIndex);
 
private:
 RPTreeIndex(){} ;
 ~RPTreeIndex(){} ;
 static RPTreeIndex* inst2 ;  
  
};

bool ComparePairs(const std::pair<double, DataVector>& a, const std::pair<double, DataVector>& b);