/*
  Class VectorDataset
  1) It has 3 private members
    vector data,rows,cols
  2)Member Functions
   ->Constructor with no.of rows and cols defaultly set to zero here and Destructor
   ->ReadDataset function that opens the csv files and and reads the data and stores in the data.Here if no.of rows to read is not mentioned then it reads all rows
   ->getrows and getCols functions are used to access the rows and cols values 
   ->print function prints the values of the dataSet
   ->getRowi function returns the ith dataVector of data
   ->addDataVector pushes the DataVector given into data
   -> kNearestNeighbour function returns a dataset consisting of the top k nearest neighbours to the given dataVector among the given dataSet

*/
#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sstream>
#include "DataVector.cpp"
//  #include"DataVector.h"





class VectorDataset{
   
    vector<DataVector>data;
    int rows;
    int cols;
    public:
    VectorDataset(int x=0,int y=0);
    ~VectorDataset();
    void ReadDataset(string filename,int rowsToRead =-1);
    void addDataVector(DataVector vec);
    void addRowData(DataVector vec);
    void deleteDataVector(DataVector vec);
    int getrows();
    DataVector getRowi(int i);
    void print();
    int getCols();
    void setrows(int r);
    void setCols(int c);
   const DataVector getFarthestPoint(const DataVector& d) ;
   bool  isEmpty() ;


};


