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
    double width;
    double Edot;
    Records &crossSection=distribution->tape->MF23->incoherentScattering->recordsAll[0].r;
    const unsigned size=crossSection.map1.size()-1;
    const double deltaMu=(double) 2.0/size;
    vector<double> result;
    result.reserve(size);
    // const double deltaEnergy=(double)(crossSection.energy.back()-crossSection.energy[0])/size;
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
	width=5*deltaEnergy;

	while(j<size)
	  {
	    // cout<<"E="<<E<<" mu="<<mu<<" x="<<GetX(E, mu)<<" S="<<S<<'\n';
	    Edot=E/(1+(E/m_e)*(1-mu));
	    d2sigmadmudE=(distribution->GetV(E, Edot+deltaEnergy, mu+deltaMu, width)
			  -distribution->GetV(E, Edot+deltaEnergy, mu-deltaMu, width)
			  -distribution->GetV(E, Edot-deltaEnergy, mu+deltaMu, width)
			  +distribution->GetV(E, Edot-deltaEnergy, mu-deltaMu, width)
			  )/(4*deltaEnergy*deltaMu);
	    row.emplace_back(d2sigmadmudE);
	    // gridE.emplace_back(deltaEnergy/(1+(deltaEnergy/m_e)*(1-mu)));
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
