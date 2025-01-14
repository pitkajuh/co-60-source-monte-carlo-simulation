#ifndef PHOTONANGULARDISTRIBUTION_H
#define PHOTONANGULARDISTRIBUTION_H

#include "Tape.h"

const double m_e=0.51099895069E+06;
const double r_e=2.8179403227E-15;

class PhotonAngularDistribution
{
protected:
  double Gaussian(const double x, const double width)
  {
    return exp(-x*x/(2*width*width))/(width*sqrt(2*M_PI));
  }

  double DiracDelta(const double x, const double width)
  {
    // Dirac delta is approximated as Gaussian function
    return Gaussian(x, width);
  }

  double x(const double E, const double mu)
  {
    const double h=4.135667696e-15;
    const double c=299792458;
    return (E/(h*c))*sqrt((1-mu)/2);
  }
public:
  Tape *tape=nullptr;
  virtual double GetV(const double E, const double Eprime, const double mu, const double width)=0;
  virtual double Getd2sigma(const double E, const double Eprime, const double mu, const double width)=0;
  PhotonAngularDistribution(){}
  virtual ~PhotonAngularDistribution(){delete tape;}
};

class IncoherentAngularDistribution: public PhotonAngularDistribution
{
 private:
  Records *function=nullptr;

  double Eprimev(const double E, const double mu)
  {
    return E/(1+(E/m_e)*(1-mu));
  }

  double KleinNishinaCrossSection(const double E, const double mu)
  {
    const double Eprime=Eprimev(E, mu);
    const double k=E/m_e;
    const double kprime=Eprime/m_e;
    const double kk=kprime/k;
    const double muprime=1-mu;
    return M_PI*r_e*r_e*kk*kk*(1+mu*mu+k*kprime*muprime*muprime);
  }

  double dsigmadmu(const double E, const double mu)
  {
    return function->GetValue(x(E, mu))*KleinNishinaCrossSection(E, mu);
  }

  double d2sigmadEdmu(const double E, const double Eprime, const double mu, const double width)
  {
    return dsigmadmu(E, mu)*DiracDelta(Eprime-Eprimev(E, mu), width);
  }

 public:
  double GetV(const double E, const double Eprime, const double mu, const double width) override
  {
    return d2sigmadEdmu(E, Eprime, mu, width);
  }

  double Getd2sigma(const double E, const double Eprime, const double mu, const double width) override
  {
    return d2sigmadEdmu(E, Eprime, mu, width);
  }

  IncoherentAngularDistribution(){}
  IncoherentAngularDistribution(Tape *tape)
  {
    this->tape=tape;
    function=&tape->MF27->incoherentFunction->recordsAll[0].r;
  }
  ~IncoherentAngularDistribution(){}
};

class CoherentAngularDistribution: public PhotonAngularDistribution
{
 private:
  double ThomsonCrossSection(const double mu)
  {
    return M_PI*r_e*r_e*(1+mu*mu);
  }

  double dsigmadmu(const double E, const double mu)
  {
    // Coherent scattering form factor
    const double F=0;
    // Real anomalous scattering factor
    const double Fprime=0;
    // Imaginary anomalous scattering factor
    const double Fprimeprime=0;
    // mu is the cosine unit (cos(theta))
    return ThomsonCrossSection(mu)*((F+Fprime)*(F+Fprime)+Fprimeprime*Fprimeprime);
  }

  double d2sigmadEdmu(const double E, const double Eprime, const double mu, const double width)
  {
    return dsigmadmu(E, mu)*DiracDelta(Eprime-E, width);
  }
 public:
  double Getd2sigma(const double E, const double Eprime, const double mu, const double width) override
  {
    return d2sigmadEdmu(E, Eprime, mu, width);
  }

  CoherentAngularDistribution(){}
  ~CoherentAngularDistribution(){}
};

#endif
