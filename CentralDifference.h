#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "PhotonAngularDistribution.h"
#include "K.h"

struct Matrix
{
 public:
  unsigned N;
  vector<double> matrix;

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
  unsigned size;
  double delta;
  PhotonAngularDistribution *distribution=nullptr;

  void cd()
  {
    unsigned i=1;
    double mu=-1;
    double E;
    double S=1;
    double ep1;
    double em1;
    double mup1;
    double mum1;
    Records &function=distribution->tape->MF27->incoherentFunction->recordsAll[0].r;
    /* Section *function=distribution->tape->MF27->incoherentFunction; */
    Records &crossSection=distribution->tape->MF23->incoherentScattering->recordsAll[0].r;
    size=crossSection.map1.size();
    delta=2.0/size;
    /* double k1=0; */
    /* double k2=0; */
    /* double k3=0; */
    /* double k4=0; */
    /* double w1=0; */
    vector<double> result;
    result.reserve(size);
    E=crossSection.energy[i];
    E=1000;
    double deltaEnergy;
    double d2sigmadmudE;
    /* while(mu<=1) */
    while(i<size)
      {
	E=crossSection.energy[i-1];
	deltaEnergy=crossSection.energy[i]-E;
	S=function.GetValue(E);

	ep1=E+deltaEnergy;
	em1=E-deltaEnergy;
	mup1=mu+delta;
	mum1=mu-delta;

	d2sigmadmudE=(distribution->GetV(ep1, mup1, S)-distribution->GetV(ep1, mum1, S)-distribution->GetV(em1, mup1, S)+distribution->GetV(em1, mum1, S))/(4*deltaEnergy*delta);
	cout<<i-1<<" "<<mu<<" "<<E<<" "<<d2sigmadmudE<<'\n';

	mu+=delta;
	i++;
      }
  }
public:
  CentralDifference(){}
  CentralDifference(PhotonAngularDistribution *d)
  {
    this->distribution=d;
    cd();
  }
  ~CentralDifference(){}
};

#endif
