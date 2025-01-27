#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "PhotonAngularDistribution.h"
#include "Matrix.h"
#include "GaussSeidel.h"
#include "LU.h"

struct CentralDifference
{
private:
  void subs(Matrix m)
  {
    std::ofstream file;
    file.open("centraldifference.txt");
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
  void saveFile(double from, const double to, const double delta, const string &file, vector<double> v)
  {
    std::ofstream file1;
    file1.open(file);

    while(from<=to)
      {
	v.emplace_back(from);
	file1<<from;
	file1<<'\n';
	from+=delta;
      }
  }
  void write()
  {
    for(const auto &i: discretized.matrix)
      {
	for(const auto &j: i)
	  {
	    cout<<j<<'\n';
	  }
      }
  }
  PhotonAngularDistribution *distribution=nullptr;

  void cd(const double xFrom, const double xTo, const double yFrom, const double yTo, const double N)
  {
    double x=xFrom;
    double y=yFrom;
    const double deltaX=(double) (xTo-xFrom)/N;
    double Eprime=y/(1+(y/m_e)*(1-x));
    vector<double> result;
    result.reserve(N);
    double deltaY=(double) (yTo-xFrom)/N;
    const double width=deltaY/2;
    double d2sigmadmudE;
    discretized.init(N);
    vector<double> row;
    row.reserve(N);
    discretized.matrix.reserve(N);
    saveFile(xFrom, xTo-deltaX, deltaX, "mu.txt", X);
    saveFile(yFrom, yTo-deltaY, deltaY, "E.txt", Y);

    while(x<xTo-deltaX)
      {
	while(y<yTo-deltaY)
	  {
	    Eprime=y/(1+(y/m_e)*(1-x));
	    d2sigmadmudE=(distribution->Getd2sigma(y, Eprime+deltaY, x+deltaX, width)
			  -distribution->Getd2sigma(y, Eprime+deltaY, x-deltaX, width)
			  -distribution->Getd2sigma(y, Eprime-deltaY, x+deltaX, width)
			  +distribution->Getd2sigma(y, Eprime-deltaY, x-deltaX, width)
			  )/(4*deltaY*deltaX);
	    row.emplace_back(d2sigmadmudE);
	    y+=deltaY;
	  }

	discretized.emplace_back(row);
	row.clear();
	y=yFrom;
	x+=deltaX;
      }
  }
public:
  vector<double> X;
  vector<double> Y;
  Matrix discretized;

  CentralDifference(){}
  CentralDifference(PhotonAngularDistribution *d, const double xFrom, const double xTo, const double yFrom, const double yTo, const double N)
  {
    this->distribution=d;
    cd(xFrom, xTo, yFrom, yTo, N);
    GaussSeidel gs(discretized, X, Y);
    // write();
    // LU lu(discretized, X, Y);
  }
  ~CentralDifference(){}
};

#endif
