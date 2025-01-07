#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "PhotonAngularDistribution.h"
#include "Matrix.h"
#include "GaussSeidel.h"
#include "LU.h"

struct CentralDifference
{
private:
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

  void cd()
  {
    unsigned i=1;
    unsigned j=1;
    double mu=-1;
    double E;
    double width;
    double Eprime;
    Records &crossSection=distribution->tape->MF23->incoherentScattering->recordsAll[0].r;
    const unsigned size=crossSection.map1.size()-1;
    const double deltaMu=(double) 2.0/size;
    vector<double> result;
    result.reserve(size);
    // const double deltaEnergy=(double)(crossSection.energy.back()-crossSection.energy[0])/size;
    double deltaEnergy;
    double d2sigmadmudE;
    // double dsigmadmu;
    gridmu.init(size);
    discretized.init(size);
    vector<double> row;
    row.reserve(size);
    // vector<double> row2;
    // row2.reserve(size);
    // std::ofstream file11;
    // file11.open("E123.txt");

    while(j<size)
      {
	while(i<size)
	  {
	    E=crossSection.energy[i-1];
	    Eprime=E/(1+(E/m_e)*(1-mu));
	    deltaEnergy=crossSection.energy[i]-E;
	    deltaEnergy=deltaEnergy/(1+(deltaEnergy/m_e)*(1-mu));
	    width=5*deltaEnergy;

	    d2sigmadmudE=(distribution->Getd2sigma(E, Eprime+deltaEnergy, mu+deltaMu, width)
			  -distribution->Getd2sigma(E, Eprime+deltaEnergy, mu-deltaMu, width)
			  -distribution->Getd2sigma(E, Eprime-deltaEnergy, mu+deltaMu, width)
			  +distribution->Getd2sigma(E, Eprime-deltaEnergy, mu-deltaMu, width)
			  )/(4*deltaEnergy*deltaMu);

	    row.emplace_back(d2sigmadmudE);
	    // gridE.emplace_back(Eprime+deltaEnergy);
	    // E1.emplace_back(E);
	    i++;
	  }
	E=crossSection.energy[j-1];
	Eprime=E/(1+(E/m_e)*(1-mu));
	deltaEnergy=crossSection.energy[j]-E;
	deltaEnergy=deltaEnergy/(1+(deltaEnergy/m_e)*(1-mu));
	// cout<<E<<" "<<Eprime<<" "<<deltaEnergy<<" tot "<<Eprime+deltaEnergy<<'\n';
	gridE.emplace_back(Eprime);
	discretized.emplace_back(row);
	row.clear();
	i=1;
	mu+=deltaMu;
	j++;
      }

    // while(i<size)
    //   {
    // 	E=crossSection.energy[i-1];
    // 	E1.emplace_back(E);
    // 	deltaEnergy=crossSection.energy[i]-E;
    // 	width=5*deltaEnergy;

    // 	while(j<size)
    // 	  {
    // 	    // cout<<"E="<<E<<" mu="<<mu<<" x="<<GetX(E, mu)<<" S="<<S<<'\n';
    // 	    Eprime=E/(1+(E/m_e)*(1-mu));
    // 	    d2sigmadmudE=(distribution->Getd2sigma(E, Eprime+deltaEnergy, mu+deltaMu, width)
    // 			  -distribution->Getd2sigma(E, Eprime+deltaEnergy, mu-deltaMu, width)
    // 			  -distribution->Getd2sigma(E, Eprime-deltaEnergy, mu+deltaMu, width)
    // 			  +distribution->Getd2sigma(E, Eprime-deltaEnergy, mu-deltaMu, width)
    // 			  )/(4*deltaEnergy*deltaMu);
    // 	    row.emplace_back(d2sigmadmudE);
    // 	    // cout<<E<<d2sigmadmudE<<'\n';
    // 	    file11<<mu;
    // 	    file11<<";";
    // 	    file11<<d2sigmadmudE;
    // 	    file11<<'\n';
    // 	    // dsigmadmu=(distribution->Getdsigma(E, Eprime, mu+deltaMu)
    // 	    // 	       -distribution->Getdsigma(E, Eprime, mu-deltaMu))/(2*deltaMu);
    // 	    // row2.emplace_back(dsigmadmu);
    // 	    // gridE.emplace_back(deltaEnergy/(1+(deltaEnergy/m_e)*(1-mu)));
    // 	    mu+=deltaMu;
    // 	    j++;
    // 	  }

    // 	gridE.emplace_back(Eprime+deltaEnergy);
    // 	discretized.emplace_back(row);
    // 	row.clear();
    // 	// row2.clear();

    // 	mu=-1;
    // 	j=1;
    // 	i++;
    //   }
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
    // GaussSeidel gs(discretized, gridE, E1);
    // write();
    LU lu(discretized, gridE, E1);
  }
  ~CentralDifference(){}
};

#endif
