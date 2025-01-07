#ifndef LU_H
#define LU_H

#include "CentralDifference.h"
#include <algorithm>

struct LU
{
private:
  void print(const vector<vector<double>> &matrix, const string &name)
  {
    std::ofstream file;
    file.open(name);
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

  void print4(const unsigned size, const double delta)
  {
    string m="";
    std::ofstream file;
    file.open("E.txt");
    double mu=0;

    while(mu<=1)
      {
	file<<std::to_string(mu)+'\n';
	mu+=delta;
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

  vector<vector<double>> BackL(const vector<vector<double>> &L, vector<vector<double>> &rhs)
  {
    vector<vector<double>> temp;
    vector<vector<double>> x=rhs;
    double sm;

    for(unsigned i=0; i<L.size(); i++)
      {
	// for(unsigned j=0; j<)
	temp[i].emplace_back(0);
      }

    for(unsigned i=0; i<L.size(); i++)
      {
	sm=0;

	for(unsigned j=0; j<i; j++)
	  {
	    sm+=L[i][j]*temp[j][0];
	  }
	for(unsigned k=0; k<rhs.size(); k++)
	  {
	    temp[i][0]=rhs[k][i]-sm;
	  }
      }


    // for(unsigned i=0; i<L.size(); i++)
    //   {
    // 	temp=rhs[i];

    // 	// for(unsigned j=0; j<i-1; j++)
    // 	for(unsigned j=0; j<i; j++)
    // 	  {
    // 	    // cout<<"j "<<j<<" "<<i-1<<'\n';
    // 	    for(unsigned k=0; k<temp.size(); k++){temp[k]-=L[i][j]*x[j][k];}
    // 	  }
    // 	for(unsigned k=0; k<temp.size(); k++){x[i][k]=temp[k]/L[i][i];}
    //   }
    return temp;
  }

  void BackU(const vector<vector<double>> &U, vector<vector<double>> &rhs)
  {
    vector<double> v;
    for(int i=U.size()-1; i>-1; i--)
      {
	v=rhs[i];

	for(unsigned j=i+1; j<U.size(); j++)
	  {
	    for(unsigned k=0; k<v.size(); k++){v[k]-=U[i][j]*rhs[j][k];}
	  }
	for(unsigned k=0; k<v.size(); k++){rhs[i][k]=v[k]/U[i][i];}
      }
  }

  void lu(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    // vector<vector<double>> aaa={{2, 1, 4, 1}, {3, 4, -1, -1}, {1, -4, 1, 5}, {2, -2, 1, 3}};
    // m.matrix=aaa;
    // m.N=aaa.size();
    const unsigned size=m.N-1;
    cout<<size<<'\n';
    vector<double> row;
    row.reserve(size);

    // print2(gridE, E);
    // // // print4(size, deltaE);
    // // print3(size);

    // // saveFile(deltaE, E.back(), deltaE, "E.txt");
    // saveFile(-1+deltaMu, 1, deltaMu, "mu.txt");

    vector<vector<double>> L;
    L.reserve(size);
    for(unsigned i=0; i<size; i++)
      {
	for(unsigned j=0; j<size; j++)
	  {
	    row.emplace_back(0);
	  }
	L.emplace_back(row);
	row.clear();
      }

    vector<vector<double>> U=L;
    double sum1;
    double sum2;

    for(unsigned j=0; j<size; j++)
      {
	L[j][j]=1;

	for(unsigned i=0; i<j+1; i++)
	  {
	    sum1=0;

	    for(unsigned k=0; k<j; k++)
	      {
		sum1+=L[i][k]*U[k][j];
	      }

	    U[i][j]=m.matrix[i][j]-sum1;

	  }
	for(unsigned i=j; i<size; i++)
	  {
	    sum2=0;

	    for(unsigned k=0; k<i; k++)
	      {
		sum2+=L[i][k]*U[k][j];
	      }
	    L[i][j]=m.matrix[i][j]-sum2;
	    L[i][j]/=U[j][j];
	  }
      }

    // print(L, "L.txt");
    // print(U, "U.txt");


    vector<vector<double>> result;
    result.reserve(size);
    vector<double> row1;
    row1.reserve(size);
    cout<<L.size()<<" "<<L[0].size()<<'\n';
    for(unsigned i=0; i<size; i++)
      {
	for(unsigned j=0; j<size; j++)
	  {
	    row1.emplace_back(1);
	  }
	result.emplace_back(row1);
	row1.clear();
      }
    cout<<L.size()<<" "<<L[0].size()<<'\n';
    result=BackL(L, result);
    BackU(U, result);
    print(result, "res.txt");
  }
public:
  LU(){}
  LU(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    lu(m, gridE, E);
  }
  ~LU(){}
};

#endif
