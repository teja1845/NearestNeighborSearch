

#include <chrono>
#include "VectorDataset.h"
// #include"DataVector.cpp"
// #include"DataVector.h"


using namespace std;

int VectorDataset :: getrows()
{
    return rows;
}
int VectorDataset :: getCols()
{
    return cols;
}
 void VectorDataset :: setrows(int r)
 {
    rows = r;
 }
 void VectorDataset :: setCols(int c)
 {
    cols = c;
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
    bool VectorDataset :: isEmpty()  {
        return this->getrows() == 0;
    }


VectorDataset :: ~VectorDataset()
{
   
}

void VectorDataset :: addDataVector(DataVector vec)
{
    data.push_back(vec);
    //setrows(rows+1);
   

}
void VectorDataset :: addRowData(DataVector vec)
{
    data.push_back(vec);
    rows++;
   

}

void VectorDataset :: deleteDataVector(DataVector vec)
{
    for(int i = 0;i < getrows();i++)
    {
         if(getRowi(i).areEqual(vec))
         {   data.erase(data.begin()+i);
            return;
         }
         
    }
     //setrows(rows-1);
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
            //cout<<rows<<"added,";
            
        }
    }

    rows = rowsToRead-2;

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
const DataVector VectorDataset::getFarthestPoint(const DataVector& d)  {
    // Initialize variables to store the farthest point and its distance
    DataVector farthestPoint;
    double maxDistance = 0.0;

    // Iterate over all points in the dataset
    for (int i = 0; i < data.size(); ++i) {
        // Compute the distance between the current point and the given point d
        double distance = dist(data[i], d);

        // Update farthestPoint and maxDistance if necessary
        if (distance > maxDistance) {
            maxDistance = distance;
            farthestPoint = data[i];
        }
    }

    // Return the farthest point found
    return farthestPoint;
}


// int main()
// {
//     VectorDataset v;
//     v.ReadDataset("train.csv",1000);
//     cout<<"Read";
//     cout<<v.getRowi(998).getVal(4);
// }

// void freeVectorDataset(VectorDataset& dataset) {
//     // Get the number of rows in the dataset
//     int numRows = dataset.getrows();

//     // Iterate over each row and deallocate the memory for the data vectors
//     for (int i = 0; i < numRows; ++i) {
//         DataVector* row = & dataset.getRowi(i);
//         delete row;
//     }

//     // Clear the dataset, deallocating the memory for the data vectors container
    
// }