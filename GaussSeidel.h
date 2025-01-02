#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "CentralDifference.h"

struct GaussSeidel
{
private:

  void GS(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    unsigned i=1;
    unsigned j=1;
    double d;
    double d1=0;
    double d2=0;
    double d3=0;
    double d4=0;
    double d5=0;
    Matrix result;
    result.init2(m.N);
    cout<<"AOE "<<m.N<<'\n';
    while(i<m.N-1)
      {
	d1=result.Get(i+1, j+1);
	d2=result.Get(i+1, j-1);
	d3=result.Get(i-1, j+1);
	d4=result.Get(i-1, j-1);
	d5=4*gridE[i-1]*2/m.N;
	d=d1-d2-d3-d4-d5;
	cout<<E[i]+gridE[i-1]<<";"<<d<<'\n';
	result.Set(i,j, d);
	i++;
	j++;
      }
  }
public:
  GaussSeidel(){}
  GaussSeidel(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    GS(m, gridE, E);
  }
  ~GaussSeidel(){}
};

#endif
