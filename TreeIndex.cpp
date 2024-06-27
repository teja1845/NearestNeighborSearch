#include"TreeIndex.h"

using namespace std;

#define minSize 10
Node*cur;

 bool ComparePairs(const std::pair<double, DataVector>& a, const std::pair<double, DataVector>& b) {
    return a.first < b.first; // Compare doubles
}

// #include "VectorDataset.h" // Include necessary headers

double Node::calMedian(int xi) {
    
    int size = dataset.getrows();
    
    vector<double> values(size);
   
    
    for (int i = 0; i < size-1; ++i) {
        
        values[i] = dataset.getRowi(i).getVal(xi); 
        
        
    }
     
   
    double median;
    
    

    sort(values.begin(), values.end());

    
   
    if (size % 2 == 0) {
        median = (values[size/ 2 - 1] + values[size / 2]) / 2.0;
    } else {
        median = values[size / 2];
    }
    
    return median;
}
double Node::check() {
    if (this->left) { // Check if the left child exists
        return this->left->dataset.getRowi(3).getVal(4);
    } else {
        // Handle the case when the left child is NULL
        return 0.0; // Or any appropriate default value
    }
}

DataVector getFarthestPoint(const DataVector& x,  VectorDataset& dataset) {
    int farthestIndex = 0;
    double maxDistance = 0.0;
    int dataSize = dataset.getrows();

    for (int i = 0; i < dataSize; ++i) {
        double distance = dist(x, dataset.getRowi(i));
        if (distance > maxDistance) {
            maxDistance = distance;
            farthestIndex = i;
        }
    }

    return dataset.getRowi(farthestIndex);
}


TreeIndex* TreeIndex::instance = NULL;
KDTreeIndex* KDTreeIndex::inst1 = NULL;
RPTreeIndex* RPTreeIndex::inst2 = NULL;

TreeIndex& TreeIndex::GetInstance() {
    if (!instance) {
        instance = new TreeIndex();
    }
    return *instance;
}

KDTreeIndex& KDTreeIndex::GetInstance() {
    if (!inst1) {
        inst1 = new KDTreeIndex();
    }
    return *inst1;
}

RPTreeIndex& RPTreeIndex::GetInstance() {
    if (!inst2) {
        inst2 = new RPTreeIndex();
    }
    return *inst2;
}

TreeIndex::TreeIndex() {
    root.prt = NULL;
    cur = &root;
    
    root.dataset.ReadDataset("train.csv", 500);
    cout << "Read in TreeIndex" << endl;
}


void TreeIndex::deleteTree(Node* currentNode) {
    if (!currentNode) {
        return; 

    
    deleteTree(currentNode->left);
    deleteTree(currentNode->rt);

   
    delete currentNode;
}
}


void TreeIndex::kNearestNeighbours(int k, DataVector d) {
    // Find the lowermost node containing at least k points
    Node* lowermostNode = findPos(d, &root, k);
   
    // If lowermostNode is NULL, there are not enough points in the dataset
    if (lowermostNode == NULL) {
        std::cout << "Not enough points in the dataset." << std::endl;
        return;
    }

    // Create a vector of pairs to store the k nearest neighbors
    std::vector<std::pair<double, DataVector>> pq;

    // Initialize the vector with the first k points
    for (int i = 0; i < k; ++i) {
        pq.push_back({ dist(d, lowermostNode->dataset.getRowi(i)), lowermostNode->dataset.getRowi(i) });
    }

    // Sort the vector based on the first element (distance)
    std::sort(pq.begin(), pq.end(), ComparePairs);
     
    // Perform a recursive search starting from the lowermost node
    kNearestNeighboursRecursive(k, d, lowermostNode, pq);
      cout << "Done" << endl;
    // Print the k nearest neighbors
    for (const auto& pair : pq) {
        cout << "Distance: " << pair.first << ", Point: ";
        pair.second.print();
        cout<<endl;
    }
}

void TreeIndex::kNearestNeighboursRecursive(int k, DataVector d, Node* currentNode, std::vector<std::pair<double, DataVector>>& pq) {
    //cout << d.getVal(currentNode->prt->b_ind) << endl;
   
    
    // Check if the distance from the top of the vector (the farthest point found so far) is greater than distToBoundary
    if (currentNode->prt != NULL ) {
        cout<<"Hi";
         double distToBoundary = abs(d.getVal(currentNode->prt->b_ind) - currentNode->prt->b_val);
        if( pq.back().first > distToBoundary)
        // Backtrack to the parent node until a suitable node is found
            kNearestNeighboursRecursive(k, d, currentNode->prt, pq);
        
    }
    else {
        
        // Search all the points in the region corresponding to the current node
        for (int i = 0; i < currentNode->dataset.getrows(); ++i) {
            double distance = dist(currentNode->dataset.getRowi(i), d);
            pq.push_back({ distance, currentNode->dataset.getRowi(i) });

            // Keep only the k nearest neighbors in the vector
            if (pq.size() > k) {
                // Sort the vector based on distance and keep only the top k elements
                std::sort(pq.begin(), pq.end(), ComparePairs);
                pq.pop_back();
            }
        }
    }
}
Node* TreeIndex::findPos(DataVector d, Node* currentNode,int k) {
    // Base case: If the current node is NULL, return NULL
    if (currentNode == NULL) {
        return NULL;
    }

    
    if (isInPartition(d, currentNode)) {
        if(!currentNode->left || currentNode->left->dataset.getrows() < k )
         {
           

            return currentNode;
         }
          else {   
                    
                return findPos(d, currentNode->left,k);
          }
        }
    else {
        if(!currentNode->rt || currentNode->rt->dataset.getrows() < k )
         return currentNode;
          else        
                 return findPos(d, currentNode->rt,k);
    }
}

bool TreeIndex::isInPartition(DataVector d, Node* parentNode) {
    // Check if the data vector's value along the split dimension falls within the parent node's boundary
    double val = d.getVal(parentNode->b_ind);
    return (val < parentNode->b_val);
}




//-------------KD TreeINdex---------------
bool KDTreeIndex::chooseRule(int i,   DataVector d,double med) {
   
    return d.getVal(i) >= med;
}

void KDTreeIndex::MakeTree(Node*& currentNode, int dimensionIndex) {
    if (currentNode->dataset.getrows() < minSize) {
        // Base case: If the size of the dataset associated with the current node is less than minSize, set left and right pointers to NULL
        currentNode->left = NULL;
        currentNode->rt = NULL;
        return;
    }

    VectorDataset leftSubset, rightSubset;
    // leftSubset = new VectorDataset;
    //  rightSubset = new VectorDataset;
    leftSubset.setCols(currentNode->dataset.getCols()) ;
    rightSubset.setCols(currentNode->dataset.getCols()) ;

    double med = currentNode->calMedian(dimensionIndex);
    currentNode->b_ind = dimensionIndex;
    currentNode->b_val = med;
    for (int i = 0; i < currentNode->dataset.getrows() ; ++i) {
        bool rule = chooseRule(dimensionIndex, currentNode->dataset.getRowi(i), med);
        
        if (!rule) {
            leftSubset.addRowData(currentNode->dataset.getRowi(i));
        } else {
            rightSubset.addRowData(currentNode->dataset.getRowi(i));
        }
    }

    cout << "Left subset size: " << leftSubset.getrows() << ", Right subset size: " << rightSubset.getrows() << endl;

    // Create left subtree recursively
    currentNode->left = new Node();
    currentNode->left->prt = currentNode; // Set parent pointer
    currentNode->left->dataset = leftSubset;
    
    MakeTree(currentNode->left, (dimensionIndex + 1) % currentNode->dataset.getCols()); // Recursively build left subtree with next dimension

   // cout << "Dimension index: " << dimensionIndex << endl;

    // Create right subtree recursively
    currentNode->rt = new Node();
    currentNode->rt->prt = currentNode; // Set parent pointer
    currentNode->rt->dataset =  rightSubset;
    MakeTree(currentNode->rt, (dimensionIndex + 1) % currentNode->dataset.getCols()); // Recursively build right subtree with next dimension

   // cout << "Made tree" << endl;
}


KDTreeIndex& KDTreeIndex::AddData(DataVector d) {
    // Add the given DataVector to the root dataset
    cur->dataset.addRowData(d);

    // Destroy the existing tree
    deleteTree(cur->left);
    deleteTree(cur->rt);

    // Rebuild the KD tree
    MakeTree(cur, 0);

    // Return a reference to the current KDTreeIndex object
    return *this;
}



//------------RP TreeIndex-----------

bool RPTreeIndex::chooseRule(int i, DataVector d, double med) {
    // Generate a random unit direction v from the unit sphere S^(D-1)
    // For simplicity, let's assume D is the number of dimensions in the dataset
    // Randomly generate D components for v
    vector<double> v(d.dimen());
    for (int j = 0; j < d.dimen(); ++j) {
        // Generate random values for v[j] within the range [-1, 1]
        v[j] = (2.0 * rand() / RAND_MAX) - 1.0;
    }
    
    // Normalize v to make it a unit vector
    double norm = 0.0;
    for (int j = 0; j < d.dimen(); ++j) {
        norm += v[j] * v[j];
    }
    norm = sqrt(norm);
    for (int j = 0; j < d.dimen(); ++j) {
        v[j] /= norm;
    }
    
    // Find the farthest point y from x
    DataVector y = root.dataset.getFarthestPoint(d);

    // Calculate δ uniformly at random in [-1, 1] * (6 * ||x - y|| / sqrt(D))
    double deltaX = 0.0;
    for (int j = 0; j < d.dimen(); ++j) {
        deltaX += (d.getVal(j) - y.getVal(j)) * (d.getVal(j) - y.getVal(j));
    }
    deltaX = sqrt(deltaX);
    double delta = ((2.0 * rand() / RAND_MAX) - 1.0) * (6.0 * deltaX / sqrt(d.dimen()));

    // Calculate the rule
    double median = root.calMedian(i);
    return dotProduct(d,v) <= (median + delta);
}

void RPTreeIndex::MakeTree(Node*& currentNode, int dimensionIndex) {
    if (currentNode->dataset.getrows() < minSize) {
        // Base case: If the size of the dataset associated with the current node is less than minSize, set left and right pointers to NULL
        currentNode->left = NULL;
        currentNode->rt = NULL;
        return;
    }

    VectorDataset leftSubset, rightSubset;
    leftSubset.setCols(currentNode->dataset.getCols()) ;
    rightSubset.setCols(currentNode->dataset.getCols()) ;

    double med = currentNode->calMedian(dimensionIndex);
    currentNode->b_ind = dimensionIndex;
    currentNode->b_val = med;
    for (int i = 0; i < currentNode->dataset.getrows() ; ++i) {
        bool rule = chooseRule(dimensionIndex, currentNode->dataset.getRowi(i), med);
        
        if (!rule) {
            leftSubset.addRowData(currentNode->dataset.getRowi(i));
        } else {
            rightSubset.addRowData(currentNode->dataset.getRowi(i));
        }
    }

    cout << "Left subset size: " << leftSubset.getrows() << ", Right subset size: " << rightSubset.getrows() << endl;

    // Create left subtree recursively
    currentNode->left = new Node();
    currentNode->left->prt = currentNode; // Set parent pointer
    currentNode->left->dataset = leftSubset;
    
    MakeTree(currentNode->left, (dimensionIndex + 1) % currentNode->dataset.getCols()); // Recursively build left subtree with next dimension

   // cout << "Dimension index: " << dimensionIndex << endl;

    // Create right subtree recursively
    currentNode->rt = new Node();
    currentNode->rt->prt = currentNode; // Set parent pointer
    currentNode->rt->dataset = rightSubset;
    MakeTree(currentNode->rt, (dimensionIndex + 1) % currentNode->dataset.getCols()); // Recursively build right subtree with next dimension

   
}





int main() {
   // TreeIndex& x = TreeIndex::GetInstance();
    // RPTreeIndex& a = RPTreeIndex::GetInstance();
    KDTreeIndex& b = KDTreeIndex::GetInstance();
    // a.MakeTree(cur,0);
    b.MakeTree(cur,0);
    
    //ADD Data testing and delete Data iimplementation ,search implementation
    VectorDataset testing;
    testing.ReadDataset("test.csv",3);
    DataVector d = testing.getRowi(1);
   // d.print();
   // cout << "Done" << endl;
   int k;
   cout<<"Enter k value :";
    cin >> k;
    b.kNearestNeighbours(k,d);
    // b.AddData(d);
     cout << "Done" << endl;

    return 0;
}






