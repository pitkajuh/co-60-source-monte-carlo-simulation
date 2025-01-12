#ifndef LU_H
#define LU_H

#include "CentralDifference.h"
#include "K.h"
#include <algorithm>

struct LU
{
private:
  // void subs(Matrix m, vector<double> &r)
  void subs(Matrix m)
  {
    std::ofstream file;
    file.open("LU.txt");
    unsigned k=0;
    for(const auto &i: m.matrix)
      {
	for(const auto &j: i)
	  {
	    // cout<<j<<'\n';
	    // file<<j*r[k];
	    file<<j;

	    if(k<i.size()-1) file<<';';
	    k++;
	  }
	k=0;
	file<<'\n';
      }
  }
  void print2(const vector<double> &matrix, const string &name)
  {
    std::ofstream file;
    file.open(name);
    // unsigned i=0;
    for(const auto &row: matrix)
      {
	// for(const auto &columnValue: row)
	//   {
	    file<<row;

	    // if(i<row.size()-1) file<<';';
	    // i++;
	  // }
	// i=0;
	file<<'\n';
      }
    file.close();
  }
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

  void print2(const vector<double> &matrix)
  {
    std::ofstream file;
    file.open("E.txt");
    for(unsigned i=0; i<matrix.size(); i++)
      {
	// file<<std::to_string(matrix[i]+matrix2[i])+'\n';
	file<<std::to_string(matrix[i])+'\n';
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

  vector<double> BackL(const vector<vector<double>> &L, const vector<double> &rhs)
  {
    vector<double> temp;
    double sm;

    for(unsigned i=0; i<L.size(); i++)
      {
	temp.emplace_back(0);
      }

    for(unsigned i=0; i<L.size(); i++)
      {
	sm=0;

	for(unsigned j=0; j<i; j++)
	  {
	    sm+=L[i][j]*temp[j];
	  }
	temp[i]=rhs[i]-sm;
      }
    return temp;
  }

  vector<double> BackU(const vector<vector<double>> &U, const vector<double> &rhs)
  {
    vector<double> temp;
    double sm;

    for(unsigned i=0; i<U.size(); i++)
      {
	temp.emplace_back(0);
      }

    for(int i=rhs.size()-1; i>-1; i--)
      {
	sm=0;

	for(unsigned j=i+1; j<rhs.size(); j++)
	  {
	    sm+=U[i][j]*temp[j];
	  }
	temp[i]=(rhs[i]-sm)/U[i][i];
      }
    return temp;
  }

  vector<double> lu(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    // vector<vector<double>> aaa={{2, 1, 4, 1}, {3, 4, -1, -1}, {1, -4, 1, 5}, {2, -2, 1, 3}};
    vector<vector<double>> aaa=m.matrix;
    // m.matrix=aaa;
    // m.N=aaa.size();
    const unsigned size=aaa.size();

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
    // vector<vector<double>> U=L;

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

    print(L, "L.txt");
    print(U, "U.txt");

    vector<double> result;
    result.reserve(size);
    // vector<double> row1;
    // row1.reserve(size);
    // cout<<L.size()<<" "<<L[0].size()<<'\n';
    for(unsigned i=0; i<size; i++)
      {
	// for(unsigned j=0; j<size; j++)
	//   {
	  //   row1.emplace_back(1);
	  // // }
	result.emplace_back(1);
	// row1.clear();
      }

    // result={-4,3,9,7};
    vector<double> r1=BackL(L, result);
    r1=BackU(U, r1);

    const double deltaMu=(double) 2/E.size();

    print2(r1, "res.txt");
    print2(gridE);
    saveFile(-1+deltaMu, 1, deltaMu, "mu.txt");
    return r1;
  }
public:
  LU(){}
  LU(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    // vector<double> res=lu(m, gridE, E);
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
