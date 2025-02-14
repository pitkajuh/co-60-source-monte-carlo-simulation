#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

// #include "CentralDifference.h"
#include "Matrix.h"
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
    // cout<<largest<<" tolerance "<<tolerance<<'\n';
    return largest<tolerance;
  }

  void GS(Matrix &m, const double deltaX, const double deltaY, const unsigned N, const string &name, const double accuracy)
  {
    bool convergence=0;
    result=m.matrix;
    vector<vector<double>> resultOld;
    vector<double> row;
    vector<double> row2;
    const unsigned size=N-1;
    // result.reserve(size);
    row.reserve(size);

    while(!convergence)
      {
     	resultOld=result;

     	for(unsigned i=1; i<size-1; i++)
     	  {
     	    row2=m.GetRow(i);

     	    for(unsigned j=1; j<size-1; j++)
     	      {
		result[i][j]=0.25*(result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1]
				   +result[i-1][j-1]-4*deltaY*deltaX*row2[j]);
     	      }
     	  }
     	convergence=CheckConvergence(result, resultOld, accuracy);
      }
    print(result, name);
  }
public:
  vector<vector<double>> result;

  GaussSeidel(){}
  GaussSeidel(Matrix &m, const double deltaX, const double deltaY, const unsigned N, const string &name, const double accuracy, const vector<double> &X, const vector<double> &Y)
  {
    GS(m, deltaX, deltaY, N, name, accuracy);
  }
  ~GaussSeidel(){}
};

#endif
