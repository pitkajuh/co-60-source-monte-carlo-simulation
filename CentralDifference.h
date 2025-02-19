#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "PhotonAngularDistribution.h"
#include "Matrix.h"

struct CentralDifference
{
private:
  void CreateVector(const double from, const double to, const double delta, vector<double> &v)
  {
    double f=from;

    while(f<to)
      {
	v.emplace_back(f);
	f+=delta;
      }
  }
  void subs(Matrix &m, const string &name)
  {
    std::ofstream file;
    file.open(name+"centraldifference.txt");
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
  void saveFile(const string &file, vector<double> v)
  {
    std::ofstream file1;
    file1.open(file);

    for(const auto &w:v)
      {
	file1<<w;
	file1<<'\n';
      }
  }

  PhotonAngularDistribution *dist=nullptr;

  void cd1(const unsigned N, const string &name, const double deltaX, const double deltaY)
  {
    double dsigmadmu;
    vector<double> row;
    row.reserve(N);
    discretized.matrix.reserve(N);

    for(const auto &y:Y)
      {
	for(const auto &x:X)
	  {
	    dsigmadmu=(dist->Getdsigma(y, x+deltaX)
		       -dist->Getdsigma(y, x-deltaX))
	               /(2*deltaX);
	    row.emplace_back(dsigmadmu);
	  }
	discretized.emplace_back(row);
	row.clear();
      }
  }

  void cd(const unsigned N, const string &name, const double deltaX, const double deltaY)
  {
    double Eprime;
    const double width=deltaY/2;
    double d2sigmadmudE;
    discretized.init(N);
    vector<double> row;
    row.reserve(N);
    discretized.matrix.reserve(N);

    for(const auto &x:X)
      {
	for(const auto &y:Y)
	  {
	    Eprime=y/(1+(y/m_e)*(1-x));
	    d2sigmadmudE=(dist->Getd2sigma(y, Eprime+deltaY, x+deltaX, width)
			 -dist->Getd2sigma(y, Eprime+deltaY, x-deltaX, width)
			 -dist->Getd2sigma(y, Eprime-deltaY, x+deltaX, width)
			 +dist->Getd2sigma(y, Eprime-deltaY, x-deltaX, width)
			  )/(4*deltaY*deltaX);
	    row.emplace_back(d2sigmadmudE);
	  }
	discretized.emplace_back(row);
	row.clear();
      }
  }
public:
  vector<double> X;
  vector<double> Y;
  Matrix discretized;

  CentralDifference(){}
  CentralDifference(PhotonAngularDistribution *d, const double xFrom, const double xTo, const double yFrom, const double yTo, const unsigned N, const string &name)
  {
    this->dist=d;
    const double deltaX=(double)(xTo-xFrom)/N;
    const double deltaY=(double)(yTo-yFrom)/N;
    CreateVector(xFrom, xTo, deltaX, X);
    CreateVector(yFrom, yTo-deltaY, deltaY, Y);
    saveFile(name+"mu.txt", X);
    saveFile(name+"E.txt", Y);
    // cd(N, name, deltaX, deltaY);
    cd1(N, name, deltaX, deltaY);
    subs(discretized, name);
  }
  ~CentralDifference(){}
};

#endif
