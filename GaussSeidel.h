#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "CentralDifference.h"
#include <algorithm>

struct GaussSeidel
{
private:
  void print(const vector<vector<double>> &matrix, const string &name)
  {
    std::ofstream file;
    file.open(name+"gaussseidel.txt");
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

  void saveFile(double from, const double to, const double delta, const string &file)
  {
    std::ofstream file1;
    file1.open(file);

    while(from<=to)
      {
	file1<<from;
	file1<<'\n';
	from+=delta;
      }
  }

  bool CheckConvergence(const vector<vector<double>> &resultNew, const vector<vector<double>> &resultOld, const double tolerance)
  {
    vector<double> rowNew;
    vector<double> rowOld;
    double delta=0;
    double largest=0;

    for(unsigned i=0; i<resultNew.size(); i++)
      {
	rowNew=resultNew[i];
	rowOld=resultOld[i];

	for(unsigned j=0; j<rowNew.size(); j++)
	  {
	    delta=fabs(rowNew[j]-rowOld[j]);

	    if(delta>largest) largest=delta;
	  }
      }
    cout<<largest<<" tolerance "<<tolerance<<'\n';
    return largest<tolerance;
  }

  void GS(Matrix &m, const double xFrom, const double xTo, const double yFrom, const double yTo, const double N, const string &name)
  {
    const unsigned size=N-1;
    const double deltaMu=(double) (xTo-xFrom)/N;
    const double deltaE=(double) (yTo-xFrom)/N;
    vector<vector<double>> result=m.matrix;
    vector<vector<double>> resultOld;
    vector<double> row;
    vector<double> row2;
    result.reserve(size);
    row.reserve(size);

    bool convergence=0;

    while(!convergence)
      {
     	resultOld=result;

     	for(unsigned i=1; i<size-1; i++)
     	  {
     	    row2=m.GetRow(i);

     	    for(unsigned j=1; j<size-1; j++)
     	      {
		result[i][j]=0.25*(result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1]
				   +result[i-1][j-1]-4*deltaE*deltaMu*row2[j]);
     	      }
     	  }
     	convergence=CheckConvergence(result, resultOld, 1e-32);
      }
    print(result, name);
  }
public:
  GaussSeidel(){}
  GaussSeidel(Matrix &m, const double xFrom, const double xTo, const double yFrom, const double yTo, const double N, const string &name)
  {
    GS(m, xFrom, xTo, yFrom, yTo, N, name);
  }
  ~GaussSeidel(){}
};

#endif
