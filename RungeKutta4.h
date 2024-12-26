#ifndef RUNGEKUTTA4_H
#define RUNGEKUTTA4_H

#include "PhotonAngularDistribution.h"
#include "K.h"

struct RungeKutta4
{
private:
  unsigned size;
  double delta;

  K k0(const double mu, const double E)
  {
    K k0v(size);
    return k0v;
  }
  K k(K k, const double d, const double mu, const double E)
  {
    K kx(size);
    return kx;
  }

  void RK4(PhotonAngularDistribution *d)
  {
    unsigned i=0;
    double mu=-1;
    double E;
    /* Records &function=d->tape->MF27->incoherentFunction->recordsAll[0].r; */
    Records &crossSection=d->tape->MF23->incoherentScattering->recordsAll[0].r;
    size=crossSection.map1.size();
    delta=2.0/size;
    K k1(size);
    K k2(size);
    K k3(size);
    K k4(size);
    vector<double> result;
    result.reserve(size);

    while(mu<=1+delta)
      {
	E=crossSection.energy[i];
	k1=k0(mu, E);
	k2=k(k1, 0.5, mu, E);
	k3=k(k2, 0.5, mu, E);
	k4=k(k3, 0, mu, E);
	cout<<mu<<'\n';
	mu+=delta;
      }
  }
public:
  RungeKutta4(){}
  RungeKutta4(PhotonAngularDistribution *d)
  {
    RK4(d);
  }
  ~RungeKutta4(){}
};

#endif
