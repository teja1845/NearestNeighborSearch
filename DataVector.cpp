
#include"DataVector.h"

using namespace std;

DataVector ::  DataVector(int dimension  )
{
   dim = dimension;
    v.clear();
    v.resize(dim);
    

}

DataVector::~DataVector() {
  
}

void DataVector :: setDimension(int dimension)
{
    if(dimension == 0)
    {
        v.clear();
        dim =0;
    }
    else
     {
        v.clear();
        v.resize(dimension);
        dim = dimension;
     }

}

DataVector :: DataVector(const DataVector& other)
{
    if(this != & other)
    {
      dim = other.dim;
      v = other.v;
    }
}

DataVector & DataVector ::  operator=(const DataVector& other)
{
    if(this != & other)
    {
       
        dim = other.dim;
        v = other.v;
    }
    return *this;
}

DataVector  DataVector :: operator+(const DataVector& other)
{
    if(dim == other.dim)
    {
        for(int i=0;i<dim;i++)
        {
           
                v[i] = v[i]+other.v[i];
        }
    }
   
    return *this;

}

DataVector  DataVector :: operator-(const DataVector& other)
{
     if(dim == other.dim)
    {
        for(int i=0;i<dim;i++)
        {
            
                v[i] = v[i] - other.v[i];
        }
    }
    return *this;
}

double DataVector :: operator*(const DataVector& other)
{
    double res =0.0;
    for(int i=0;i < min(dim,other.dim);i++)
    {
        res+=(v[i] * other.v[i]);
    }
    return res;
    
}

void DataVector :: print()
{
    for(int i=0;i<dim;i++)
    {
        cout << v[i]<<" ";
    }
}

double DataVector :: norm()
{
    double ans=0.0;
    for(int i=0;i<dim;i++)
      ans += v[i] * v[i];
    ans = sqrt(ans);
    return ans;
}

double DataVector :: getVal(int j)
{
    return v[j];
}

double dist(DataVector vec1,DataVector vec2)
{
    double d1,d2;
    d1 = vec1.norm();
    d2 = vec2.norm();
    if(d1 > d2)
       return d1-d2;
    else
     return d2-d1;

}

void DataVector :: setVal(int x,int ind)
{
    if (ind >= 0 && ind < dim) {
            // Set the value only if it's a valid double
            v[ind] = x;
        } 
}






