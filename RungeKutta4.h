#ifndef RUNGEKUTTA4_H
#define RUNGEKUTTA4_H

#include "PhotonAngularDistribution.h"
#include "K.h"

struct RungeKutta4
{
private:
  unsigned size;
  double delta;
  PhotonAngularDistribution *distribution=nullptr;

  void RK4()
  {
    /* unsigned i=0; */
    /* double mu=-1; */
    /* double E; */
    /* /\* Records &function=distribution->tape->MF27->incoherentFunction->recordsAll[0].r; *\/ */
    /* Records &crossSection=distribution->tape->MF23->incoherentScattering->recordsAll[0].r; */
    /* size=crossSection.map1.size(); */
    /* delta=2.0/size; */
    /* double k1=0; */
    /* double k2=0; */
    /* double k3=0; */
    /* double k4=0; */
    /* double w1=0; */
    /* vector<double> result; */
    /* result.reserve(size); */
    /* E=crossSection.energy[i]; */
    /* E=1000; */

    /* while(mu<=1) */
    /*   { */
    /* 	k1=delta*distribution->GetV(E, mu); */
    /* 	k2=delta*distribution->GetV(E, mu+0.5*delta); */
    /* 	k3=delta*distribution->GetV(E, mu+0.5*delta); */
    /* 	k4=delta*distribution->GetV(E, mu+delta); */

    /* 	w1+=(k1+2*k2+2*k3+k4)/6; */
    /* 	cout<<mu<<" "<<w1<<'\n'; */
    /* 	mu+=delta; */
    /*   } */
  }
public:
  RungeKutta4(){}
  RungeKutta4(PhotonAngularDistribution *d)
  {
    this->distribution=d;
    RK4();
  }
  ~RungeKutta4(){}
};

#endif
