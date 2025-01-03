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

  bool CheckConvergence(const vector<vector<double>> &resultNew, const vector<vector<double>> &resultOld, const double tolerance)
  {
    /* bool r=0; */
    vector<double> rowNew;
    vector<double> rowOld;
    double delta;
    cout<<"CHECK"<<'\n';
    double largest=0;

    for(unsigned i=0; i<resultNew.size(); i++)
      {
	/* cout<<resultNew[i].size()<<" "<<resultOld[i].size()<<'\n'; */
	rowNew=resultNew[i];
	rowOld=resultOld[i];

	for(unsigned j=0; j<rowNew.size(); j++)
	  {
	    delta=abs(rowNew[j]-rowOld[j]);
	    if(delta>largest) largest=delta;
	    /* cout<<rowNew[j]<<" "<<rowOld[j]<<" "<<delta<<" "<<i<<" "<<j<<'\n'; */
	    /* if(rowOld[j]==0 or rowNew[j]==0) continue; */
	    /* cout<<delta<<'\n'; */

	    /* if(delta>tolerance and rowNew[j]!=0 and rowOld[j]!=0 and (rowNew[j]==0 or rowOld[j]==0)) return 0; */
	    /* if(delta>tolerance and rowNew[j]!=0 and rowOld[j]!=0) return 0; */
	  }
      }
    cout<<largest<<'\n';
    return largest<tolerance;
  }

  void GS(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    const unsigned size=m.N-1;
    const double deltaMu=(double) 2/size;
    /* const double deltaE=(E.back()-E[0])/size; */
    vector<vector<double>> result;
    vector<vector<double>> resultOld;
    vector<double> row;

    result.reserve(size);
    row.reserve(size);

    print2(gridE, E);
    print3(size);

    for(unsigned i=0; i<size; i++)
      {
	for(unsigned j=0; j<size; j++)
	  {
	    row.emplace_back(0);
	    /* row.emplace_back(j+1); */
	  }
	result.emplace_back(row);
	row.clear();
      }

    cout<<"AOE "<<m.N<<'\n';

    /* bool convergence=0; */
    /* /\* const unsigned stop=200; *\/ */
    /* unsigned k=0; */

    /* /\* while(k!=stop) *\/ */
    /* while(!convergence) */
    /*   { */
    /* 	resultOld=result; */
    /* 	/\* cout<<"ao"<<'\n'; *\/ */

    /* 	for(unsigned i=1; i<size-1; i++) */
    /* 	  { */
    /* 	    for(unsigned j=1; j<size-1; j++) */
    /* 	      { */
    /* 		result[i][j]=result[i+1][j+1]-result[i+1][j-1]-result[i-1][j+1] */
    /* 		  /\* -result[i-1][j-1]-4*deltaE*deltaMu; *\/ */
    /* 		  -result[i-1][j-1]-4*gridE[i]*deltaMu; */
    /* 	      } */
    /* 	  } */
    /* 	convergence=CheckConvergence(result, resultOld, 0.5); */
    /* 	cout<<"convergence "<<convergence<<'\n'; */
    /* 	k++; */
    /*   } */


    for(unsigned i=1; i<size-1; i++)
      {
	for(unsigned j=1; j<size-1; j++)
	  {
	    result[i][j]=result[i+1][j+1]-result[i+1][j-1]-result[i-1][j+1]
	      /* -result[i-1][j-1]-4*deltaE*deltaMu; */
	    -result[i-1][j-1]-4*gridE[i]*deltaMu;
	  }
      }
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
