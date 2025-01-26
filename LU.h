#ifndef LU_H
#define LU_H

#include "CentralDifference.h"
#include "K.h"
#include <algorithm>

struct LU
{
private:
  void subs(Matrix m)
  {
    std::ofstream file;
    file.open("LU.txt");
    unsigned k=0;

    for(const auto &i: m.matrix)
      {
	for(const auto &j: i)
	  {
	    file<<j;

	    if(k<i.size()-1) file<<';';
	    k++;
	  }
	k=0;
	file<<'\n';
      }
  }
  void print2(const vector<vector<double>> &matrix, const string &name)
  {
    std::ofstream file;
    file.open(name);
    for(unsigned i=0; i<matrix.size(); i++)
      {
	for(unsigned j=0; j<matrix[i].size(); j++)
	  {
	    file<<matrix[i][j];
	    if(j<matrix[i].size()-1) file<<';';
	  }
	file<<'\n';
      }
    file.close();
  }

  vector<vector<double>> BackL(const vector<vector<double>> &L, const vector<vector<double>> &rhs)
  {
    vector<vector<double>> temp;
    vector<double> row;
    double sm;

    for(unsigned i=0; i<L.size(); i++)
      {
	for(unsigned j=0; j<L.size(); j++)
	  {
	    row.emplace_back(0);
	  }
	temp.emplace_back(row);
	row.clear();
      }

    for(unsigned i=0; i<L.size(); i++)
      {
	sm=0;

	for(unsigned j=0; j<i; j++)
	  {
	    for(unsigned k=0; k<rhs[0].size(); k++)
	      {
		sm+=L[i][j]*temp[j][k];
	      }
	  }
	for(unsigned k1=0; k1<rhs[0].size(); k1++)
	  {
	    temp[i][k1]=rhs[i][k1]-sm;
	  }
      }
    return temp;
  }

vector<vector<double>> BackU(const vector<vector<double>> &U, const vector<vector<double>> &rhs, const unsigned size)
  {
    vector<vector<double>> temp;
    vector<double> row;
    double sm;

    for(unsigned i=0; i<U.size(); i++)
      {
	for(unsigned j=0; j<U.size(); j++)
	  {
	    row.emplace_back(0);
	  }
	temp.emplace_back(row);
	row.clear();
      }

    for(int i=rhs.size()-1; i>-1; i--)
      {
	sm=0;

	for(unsigned j=i+1; j<rhs.size(); j++)
	  {
	    for(unsigned k=0; k<size; k++)
	      {
		sm+=U[i][j]*temp[j][k];
	      }
	  }
	for(unsigned k1=0; k1<size; k1++)
	  {
	    temp[i][k1]=(rhs[i][k1]-sm)/U[i][i];
	    cout<<k1<<" "<<temp[i][k1]<<'\n';
	  }
      }
    return temp;
  }

  vector<vector<double>> lu(Matrix &m, vector<double> &X, vector<double> &Y)
  {
    vector<vector<double>> aaa={{2, 1, 4, 1}, {3, 4, -1, -1}, {1, -4, 1, 5}, {2, -2, 1, 3}};
    // vector<vector<double>> aaa=m.matrix;
    const unsigned size=aaa.size();

    cout<<size<<'\n';
    vector<double> row;
    row.reserve(size);

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

	    U[i][j]=aaa[i][j]-sum1;

	  }
	for(unsigned i=j; i<size; i++)
	  {
	    sum2=0;

	    for(unsigned k=0; k<i; k++)
	      {
		sum2+=L[i][k]*U[k][j];
	      }
	    L[i][j]=aaa[i][j]-sum2;
	    L[i][j]/=U[j][j];
	  }
      }

    vector<vector<double>> rhs;
    rhs.reserve(size);
    vector<double> row1;
    row1.reserve(size);

    for(unsigned i=0; i<size; i++)
      {
	for(unsigned j=0; j<size; j++)
	  {
	    row1.emplace_back(1);
	  }
	rhs.emplace_back(1);
	row1.clear();
      }

    rhs={{-4},{3},{9},{7}};
    vector<vector<double>> r1=BackL(L, rhs);
    r1=BackU(U, r1, rhs[0].size());

    print2(r1, "res.txt");
    return r1;
  }
public:
  LU(){}
  LU(Matrix &m, vector<double> &X, vector<double> &Y)
  {
    vector<vector<double>> res=lu(m, X, Y);
    // print2(gridE);
    // const double deltaMu=(double) 2/100;
    // const double deltaE=(double) (1e5-1)/100;
    // cout<<deltaMu<<" "<<deltaE<<" "<<gridE.size()<<'\n';
    // saveFile(-1+deltaMu, 1, deltaMu, "mu.txt");
    // saveFile(1+deltaE, 1e5, deltaE, "E.txt");
    subs(m);
  }
  ~LU(){}
};

#endif
