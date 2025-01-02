#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "PhotonAngularDistribution.h"
#include "Matrix.h"
#include "GaussSeidel.h"

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
    const double deltaMu=2.0/size;
    vector<double> result;
    result.reserve(size);
    double deltaEnergy;
    double d2sigmadmudE;
    gridmu.init(size);
    discretized.init(size);
    vector<double> row;
    row.reserve(size);

    while(i<size)
      {
	E=crossSection.energy[i-1];
	E1.emplace_back(E);
	deltaEnergy=crossSection.energy[i]-E;
	sigma=5*deltaEnergy;
	S=function.GetValue(E);
	ep1=E+deltaEnergy;
	em1=E-deltaEnergy;

	while(j<size)
	  {
	    mup1=mu+deltaMu;
	    mum1=mu-deltaMu;

	    d2sigmadmudE=(distribution->GetV(ep1, mup1, S, sigma)-distribution->GetV(ep1, mum1, S, sigma)-distribution->GetV(em1, mup1, S, sigma)+distribution->GetV(em1, mum1, S, sigma))/(4*deltaEnergy*deltaMu);
	    /* cout<<i-1<<" "<<j-1<<" "<<mu<<" "<<E<<" "<<d2sigmadmudE<<'\n'; */
	    row.emplace_back(d2sigmadmudE);
	    mu+=deltaMu;
	    j++;
	  }

	gridE.emplace_back(deltaEnergy);
	discretized.emplace_back(row);
	row.clear();

	mu=-1;
	j=1;
	i++;
      }
  }
public:
  vector<double> gridE;
  vector<double> E1;
  Matrix gridmu;
  Matrix discretized;

  CentralDifference(){}
  CentralDifference(PhotonAngularDistribution *d)
  {
    this->distribution=d;
    cd();
    GaussSeidel gs(discretized, gridE, E1);
  }
  ~CentralDifference(){}
};

#endif
