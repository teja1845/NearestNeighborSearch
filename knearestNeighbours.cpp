

#include <chrono>
#include "VectorDataset.h"


using namespace std;

int VectorDataset :: getrows()
{
    return rows;
}
int VectorDataset :: getCols()
{
    return cols;
}

DataVector VectorDataset :: getRowi(int i)
{
    return data[i];
}

VectorDataset :: VectorDataset(int x,int y)
{
   
         rows = x;
         cols = y;
       
       

}

VectorDataset :: ~VectorDataset()
{
   
}

void VectorDataset :: addDataVector(DataVector vec)
{
    data.push_back(vec);

}


void VectorDataset::ReadDataset(string filename, int rowsToRead ) {
    // Open the CSV file
    ifstream file(filename);

    // Check if the file is open
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;  // Or handle the error appropriately
    }

    string line;

    while (getline(file, line) && (rowsToRead == -1 || rows < rowsToRead)) {
        ++rows;

        // Create a stringstream to parse the line
        stringstream ss(line);

        if (rows == 1) {
            string field;
            while (getline(ss, field, ',')) {
                ++cols;
            }
        } else {
            DataVector vec(cols);
            for (size_t j = 0; j < cols; ++j) {
                string field;
                getline(ss, field, ',');

                try {
                    // Use stod for double values
                    vec.setVal(stod(field), j);
                } catch (const invalid_argument& e) {
                    cerr << "Error converting string to double: " << e.what() << endl;
                    // Handle this error appropriately, e.g., set the value to a default
                }
            }

            // Add the DataVector to the VectorDataset
            addDataVector(vec);
        }
    }

    // Close the file
    file.close();
    
}







void  VectorDataset :: print()
{
    for(int i=0;i<rows;i++)
    {
     for(int j=0;j< cols;j++)
     {
        cout << data[0].getVal(j)<<",";
     }
     cout<<endl;
    }
    
}



VectorDataset kNearestNeighbours(int k, VectorDataset set, DataVector vec)
{
    VectorDataset result(k,set.getCols());
     

    // Create a vector to store distances and indices
    vector<pair<double, int>> distances;

    // Iterate over the dataset to calculate distances
    for (int i = 0; i < set.getrows(); ++i) {
        const DataVector& currentVector = set.getRowi(i);  // Replace with your actual method to access a DataVector from VectorDataset

        // Calculate the Euclidean distance between vec and currentVector
        double distance = dist(vec,currentVector);  // Implement this function

        // Store the distance and index in the distances vector
        distances.push_back({distance, i});
    }
   

    // Sort distances in ascending order
    std::sort(distances.begin(), distances.end());

    // Retrieve the k-nearest neighbors
    for (int i = 0; i < k && i < set.getrows(); ++i) {
        size_t neighborIndex = distances[i].second;
        const DataVector& neighbor = set.getRowi(neighborIndex);  // Replace with your actual method to access a DataVector from VectorDataset

        // Add the neighbor to the result VectorDataset
        result.addDataVector(neighbor);  // Implement this function
    }
   // result.getRowi(1).print();
    

    return result;
}



int main()
{
    

    string f1,f2;
     int i,k,x;
   
    f1 = "train.csv";
    f2 = "test.csv";
    VectorDataset train;
    VectorDataset test;
    train.ReadDataset(f1);
    cout << "Train read successfully";
    // train.print();
    test.ReadDataset(f2);
     cout << "Test read successfully";
   // int n = test.getrows();
    cout << "Enter 1 to get k nearest neighbours of one test vector.Then enter the row of test vector required and k val:\nEnter 2 for testing all values,Then enter k value:\n";
    cin>>x;
    if(x ==1 )
     {
        cin >>i >> k;
        // Measure the start time
       auto start_time = std::chrono::high_resolution_clock::now();
  
    VectorDataset res = kNearestNeighbours(k,train,test.getRowi(i));
    
    //res.print();
  
    // Measure the end time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Output the runtime in milliseconds
    std::cout << "Runtime: " << duration.count() << " milliseconds" << std::endl;
     }
     else  
      {
         cin >> k;
   // Measure the start time
    auto start_time = std::chrono::high_resolution_clock::now();
     for( i=1;i<=100;i++)
     {
         VectorDataset res = kNearestNeighbours(k,train,test.getRowi(i));
        cout<<"Res saved";
        //res.print();
     }
    // Measure the end time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Output the runtime in milliseconds
    std::cout << "Runtime: " << duration.count() << " milliseconds" << std::endl;
    }

    return 0;

}

/* The program is taking around 65 seconds for checking 5 nearast neighbours of 100 test cases in train dataset
  For 1 testCase it is taking around 0.75 milliseconds*/