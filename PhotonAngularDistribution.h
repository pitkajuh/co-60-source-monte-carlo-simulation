#ifndef PHOTONANGULARDISTRIBUTION_H
#define PHOTONANGULARDISTRIBUTION_H

#include "Tape.h"
#include "RungeKutta4.h"

using std::numeric_limits;

const double m_e=0.51099895069E+06;
const double r_e=2.8179403227E-15;

class PhotonAngularDistribution
{
 public:
  PhotonAngularDistribution(){}
  virtual ~PhotonAngularDistribution(){}
};

class IncoherentAngularDistribution: public PhotonAngularDistribution
{
 private:
  double KleinNishinaCrossSection(const double E, const double Edot, const double mu)
  {
    const double k=E/m_e;
    const double kdot=Edot/m_e;
    const double kk=kdot/k;
    const double mudot=1-mu;
    return M_PI*r_e*r_e*kk*kk*(1+mu*mu+k*kdot*mudot*mudot);
  }

  double DiracDeltaFunction(const double x)
  {
    if(x!=0) return 0;
    else return numeric_limits<double>::infinity();
  }

  double Edotv(const double E, const double mu)
  {
    return E/(1+(E/m_e)*(1-mu));
  }

  static double IncoherentScatteringCrossSection(const double E, const double S, const double mu)
  {
    // S is the incoherent scattering function. Get from ENDF 27504
    // mu is the cosine unit (cos(theta))
    // E is the incident photon energy
    const double Edot=Edotv(E, mu);
    return S*KleinNishinaCrossSection(E, Edot, mu);//*DiracDeltaFunction(E-Edot);
  }

  void Solve(Records &function, Records &crossSection)
  {
    vector<double> resultEnergy;
    resultEnergy.reserve(crossSection.map1.size());
    vector<double> resultCos;
    resultCos.reserve(crossSection.map1.size());
    vector<double> cosine;
    cosine.reserve(crossSection.map1.size());

    double i=-1;
    double h=2.0/crossSection.map1.size();

    while(i<=1)
      {
	cosine.emplace_back(i);
	i+=h;
      }
    RungeKutta4(1, 3, 4, &IncoherentScatteringCrossSection);
    int j=0;
    for(const auto &[energy, cs]: crossSection.map1)
      {
	cout<<energy<<";"<<IncoherentScatteringCrossSection(energy, function.GetValue(energy), cosine[j])<<'\n';
	j+=1;
      }
  }
 public:
  IncoherentAngularDistribution(){}
  IncoherentAngularDistribution(Tape *tape)
  {
    Records &function=tape->MF27->incoherentFunction->recordsAll[0].r;
    Records &crossSection=tape->MF23->incoherentScattering->recordsAll[0].r;
    cout<<function.size()<<" "<<crossSection.size()<<'\n';
    Solve(function, crossSection);
  }
  ~IncoherentAngularDistribution(){}
};

class CoherentAngularDistribution: public PhotonAngularDistribution
{
 private:
  double ThomsonCrossSection(const double E, const double mu)
  {
    return M_PI*r_e*r_e*(1+mu*mu);
  }

  double CoherentScatteringCrossSection(const double E, const double mu)
  {
    // Coherent scattering form factor
    const double F=0;
    // Real anomalous scattering factor
    const double Fdot=0;
    // Imaginary anomalous scattering factor
    const double Fdotdot=0;
    // mu is the cosine unit (cos(theta))
    return ThomsonCrossSection(E, mu)*((F+Fdot)*(F+Fdot)+Fdotdot*Fdotdot);
  }
 public:
  CoherentAngularDistribution(){}
  ~CoherentAngularDistribution(){}
};

#endif
