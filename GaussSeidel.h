#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "Matrix.h"
#include <algorithm>

struct GaussSeidel
{
private:
  void Save(const vector<vector<double>> &matrix, const string &name)
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
    return largest<tolerance;
  }

  void GS(Matrix &m, const double delta, const unsigned N, const double accuracy)
  {
    vector<double> row;
    vector<vector<double>> resultOld;
    bool convergence=0;

    while(!convergence)
      {
     	resultOld=result;

     	for(unsigned i=1; i<N-2; i++)
     	  {
     	    row=m.GetRow(i);

     	    for(unsigned j=1; j<N-2; j++)
     	      {
		result[i][j]=0.25*(result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1]
				   +result[i-1][j-1]-4*delta*row[j]);
     	      }
     	  }
     	convergence=CheckConvergence(result, resultOld, accuracy);
      }
  }
public:
  vector<vector<double>> result;

  GaussSeidel(){}
  GaussSeidel(Matrix &m, const double delta, const unsigned N, const string &name, const double accuracy)
  {
    result=m.matrix;
    GS(m, delta, N, accuracy);
    Save(result, name);
  }
  ~GaussSeidel(){}
};

#endif
