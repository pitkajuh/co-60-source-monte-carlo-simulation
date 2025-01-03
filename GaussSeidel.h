#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "CentralDifference.h"

struct GaussSeidel
{
private:

  void print(const vector<vector<double>> &matrix)
  {
    std::ofstream file;
    file.open("gaussseidel.txt");
    unsigned i=0;
    for(const auto &row: matrix)
      {
	for(const auto &columnValue: row)
	  {
	    file<<columnValue;

	    if(i<row.size()-1) file<<';';
	    i++;
	  }
	i=0;
	file<<'\n';
      }
    file.close();
  }

  void print2(const vector<double> &matrix, const vector<double> &matrix2)
  {
    std::ofstream file;
    file.open("E.txt");
    for(unsigned i=0; i<matrix.size(); i++)
      {
	file<<std::to_string(matrix[i]+matrix2[i])+'\n';
      }
    file.close();
  }

  void print3(const unsigned size)
  {
    string m="";
    std::ofstream file;
    file.open("mu.txt");
    const double delta=(double)2/size;
    double mu=-1+delta;

    while(mu<=1)
      {
	file<<std::to_string(mu)+'\n';
	mu+=delta;
      }
    file.close();
  }

  void GS(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    double d;
    double d1=0;
    double d2=0;
    double d3=0;
    double d4=0;
    double d5=0;
    /* Matrix result; */
    vector<vector<double>> result;
    result.reserve(m.N-1);

    vector<double> row;
    row.reserve(m.N-1);

    print2(gridE, E);
    print3(m.N-1);

    for(unsigned i=0; i<m.N; i++)
      {
	for(unsigned j=0; j<m.N; j++)
	  {
	    row.emplace_back(0);
	  }
	result.emplace_back(row);
	row.clear();
      }

    cout<<"AOE "<<m.N<<'\n';

    for(unsigned i=1; i<m.N-1; i++)
      {
	for(unsigned j=1; j<m.N-1; j++)
	  {
	    /* d1=result.Get(i+1, j+1); */
	    /* d2=result.Get(i+1, j-1); */
	    /* d3=result.Get(i-1, j+1); */
	    /* d4=result.Get(i-1, j-1); */
	    /* d5=4*gridE[i-1]*2/m.N-1; */
	    d1=result[i+1][j+1];
	    d2=result[i+1][j-1];
	    d3=result[i-1][j+1];
	    d4=result[i-1][j-1];
	    d5=4*gridE[i-1]*2/m.N-1;
	    d=d1-d2-d3-d4-d5;
	    /* cout<<E[i]+gridE[i-1]<<";"<<d<<'\n'; */
	    /* result.Set(i,j, d); */
	    result[i][j]=d;
	  }
      }

    /* unsigned i=1; */
    /* unsigned j=1; */

    /* while(i<m.N-1) */
    /*   { */
    /* 	d1=result.Get(i+1, j+1); */
    /* 	d2=result.Get(i+1, j-1); */
    /* 	d3=result.Get(i-1, j+1); */
    /* 	d4=result.Get(i-1, j-1); */
    /* 	d5=4*gridE[i-1]*2/m.N-1; */
    /* 	d=d1-d2-d3-d4-d5; */
    /* 	/\* cout<<E[i]+gridE[i-1]<<";"<<d<<'\n'; *\/ */
    /* 	cout<<d<<'\n'; */
    /* 	result.Set(i,j, d); */
    /* 	i++; */
    /* 	j++; */
    /*   } */

    print(result);
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
