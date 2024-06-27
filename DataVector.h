//Header file
/*
   1)Here the class Datavector is defined with two private variables vector v,dim
   2)In DataVector constructor we are defining the size of vector v
   3)the DataVector copy constructor copies other value to this declared object
   4)The "=" operator is used as copy assignment operator which copies the other values to this object
   5)The "+" operator adds the other dataVectors to this object(according to dimension) and returns final val
   6)The "-" operator subtracts the other dataVector from this object(according to dimension) and returns final val
   7)The "*" operator returns the dot product of other and this object
   8)The setDimension function changes the dimensions of the DataVector and allocates the size accordingly clearing all the old memory
   9)The print function prints the value in each dimension of the dataVector
   10)the getVal function takes an index as input and returns the value at that index
   11)the setVal function sets the value at a given index to given value
   12)the norm function calculates the distance of the given vector from the origin,that is it calculates the length of that vector
->The dist function calculates the distance between two vectors
*/
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

class DataVector {
 vector<double> v;
 int dim;
 public:
 DataVector(int dimension=0);
 ~DataVector();
 DataVector(const DataVector& other);
 DataVector & operator=(const DataVector &other);
 void setDimension(int dimension=0);
 DataVector operator+(const DataVector &other);
 DataVector operator-(const DataVector &other);
 double operator*(const DataVector &other);
 void print();
 double norm( );
 
 double getVal(int j);

void setVal(int x,int ind);
};


double dist(DataVector,DataVector);