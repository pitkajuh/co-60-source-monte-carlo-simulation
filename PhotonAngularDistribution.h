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
  vector<vector<double>> d2sigmadmudE;
  vector<double> mu;
  vector<double> Eprime;

  virtual double Getd2sigma(const double E, const double Eprime, const double mu, const double width)=0;
  virtual double Getdsigma(const double E, const double mu)=0;
  PhotonAngularDistribution(){}
  virtual ~PhotonAngularDistribution(){}
};

class IncoherentAngularDistribution: public PhotonAngularDistribution
{
 private:
  Section *incoherentFunction=nullptr;

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
    return incoherentFunction->GetLibraryValue(x(E, mu), 504)*KleinNishinaCrossSection(E, mu);
  }

  double d2sigmadEdmu(const double E, const double Eprime, const double mu, const double width)
  {
    return dsigmadmu(E, mu)*DiracDelta(Eprime-Eprimev(E, mu), width);
  }

 public:
  double Getd2sigma(const double E, const double Eprime, const double mu, const double width) override
  {
    return d2sigmadEdmu(E, Eprime, mu, width);
  }

  double Getdsigma(const double E, const double mu) override
  {
    return dsigmadmu(E, mu);
  }

  IncoherentAngularDistribution(){}
  IncoherentAngularDistribution(Tape *tape)
  {
    this->tape=tape;
    incoherentFunction=tape->MF27->incoherentFunction;
  }
  ~IncoherentAngularDistribution(){}
};

class CoherentAngularDistribution: public PhotonAngularDistribution
{
 private:
  Section *coherentFactor=nullptr;
  Section *imaginaryFactor=nullptr;
  Section *realFactor=nullptr;

  double ThomsonCrossSection(const double mu)
  {
    return M_PI*r_e*r_e*(1+mu*mu);
  }

  double dsigmadmu(const double E, const double mu)
  {
    const double F=coherentFactor->GetLibraryValue(x(E, mu), 502);
    const double Fprime=realFactor->GetLibraryValue(E, 506);
    const double Fprimeprime=imaginaryFactor->GetLibraryValue(E, 505);
    const double FF=F+Fprime;
    return ThomsonCrossSection(mu)*(FF*FF+Fprimeprime*Fprimeprime);
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

  double Getdsigma(const double E, const double mu) override
  {
    return dsigmadmu(E, mu);
  }

  CoherentAngularDistribution(){}
  CoherentAngularDistribution(Tape *tape)
  {
    this->tape=tape;
    coherentFactor=tape->MF27->coherentFactor;
    imaginaryFactor=tape->MF27->imaginaryFactor;
    realFactor=tape->MF27->realFactor;
  }
  ~CoherentAngularDistribution(){}
};

#endif
