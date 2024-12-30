#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "PhotonAngularDistribution.h"
#include "K.h"

struct Matrix
{
 public:
  unsigned N;
  vector<vector<double>> matrix;

  void AddRow(const vector<double> &row){matrix.emplace_back(row);}
  void init(const unsigned n){matrix.reserve(N);}
  Matrix(){}
  Matrix(const unsigned n)
  {
    matrix.reserve(N);
    N=n;
  }
  ~Matrix(){}
};

struct CentralDifference
{
private:
  PhotonAngularDistribution *distribution=nullptr;

  void cd()
  {
    unsigned i=1;
    unsigned j=1;
    double mu=-1;
    double E;
    double S;
    double ep1;
    double em1;
    double mup1;
    double mum1;
    double sigma;
    Records &function=distribution->tape->MF27->incoherentFunction->recordsAll[0].r;
    Records &crossSection=distribution->tape->MF23->incoherentScattering->recordsAll[0].r;
    const unsigned size=crossSection.map1.size()-1;
    const double delta=2.0/size;
    vector<double> result;
    result.reserve(size);
    double deltaEnergy;
    double d2sigmadmudE;
    grid.init(size);
    vector<double> row;
    row.reserve(size);

    while(i<size)
      {
	E=crossSection.energy[i-1];
	deltaEnergy=crossSection.energy[i]-E;
	sigma=5*deltaEnergy;
	S=function.GetValue(E);
	ep1=E+deltaEnergy;
	em1=E-deltaEnergy;

	while(j<size)
	  {
	    mup1=mu+delta;
	    mum1=mu-delta;

	    d2sigmadmudE=(distribution->GetV(ep1, mup1, S, sigma)-distribution->GetV(ep1, mum1, S, sigma)-distribution->GetV(em1, mup1, S, sigma)+distribution->GetV(em1, mum1, S, sigma))/(4*deltaEnergy*delta);
	    cout<<i-1<<" "<<j-1<<" "<<mu<<" "<<E<<" "<<d2sigmadmudE<<'\n';
	    /* row.emplace_back(); */
	    mu+=delta;
	    j++;
	  }
	mu=-1;
	j=1;
	i++;
      }
  }
public:
  Matrix grid;

  CentralDifference(){}
  CentralDifference(PhotonAngularDistribution *d)
  {
    this->distribution=d;
    cd();
  }
  ~CentralDifference(){}
};

#endif
