#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "CentralDifference.h"

struct GaussSeidel
{
private:

  void GS(Matrix &m, vector<double> &gridE)
  {
    unsigned i=0;
    Matrix result;
    result.init2(m.N);
    /* while(i!=gridE.size()*gridE.size()) */
    /*   { */
	for(const auto &e: gridE)
	  {

	    for(const auto &m1: m)
	      {
		cout<<i<<" "<<e<<" "<<gridE.size()<<'\n';
	      }
	  }
	/* i++; */
      /* } */
  }
public:
  GaussSeidel(){}
  GaussSeidel(Matrix &m, vector<double> &gridE)
  {
    GS(m, gridE);
  }
  ~GaussSeidel(){}
};

#endif
