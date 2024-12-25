#include <random>
#include <cmath>

using std::numeric_limits;

const double m_e=0.51099895069E+06;
const double r_e=2.8179403227E-15;

double RNG(const double min, const double max)
{
  // Random number generator
  std::mt19937 random(std::random_device{}());
  std::uniform_real_distribution<double> rnd(min, max);
  return rnd(random);
}

unsigned PoissonRNG(const unsigned activity, const unsigned time)
{
  // Poisson random number generator
  std::mt19937 random(std::random_device{}());
  std::poisson_distribution<int> rnd(time*activity);
  return rnd(random);
}

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

double IncoherentScatteringCrossSection(const double E, const double S, const double mu)
{
  // S is the incoherent scattering function. Get from ENDF 27504
  // mu is the cosine unit (cos(theta))
  // E is the incident photon energy
  const double Edot=Edotv(E, mu);
  return S*KleinNishinaCrossSection(E, Edot, mu)*DiracDeltaFunction(E-Edot);
}

double ThomsonCrossSection(const double E)
{
  // Get mu from ENDF
  const double mu=0;
  return M_PI*r_e*r_e*(1+mu*mu);
}

double CoherentScatteringCrossSection(const double E)
{
  // Coherent scattering form factor
  const double F=0;
  // Real anomalous scattering factor
  const double Fdot=0;
  // Imaginary anomalous scattering factor
  const double Fdotdot=0;
  // mu is the cosine unit (cos(theta))
  return ThomsonCrossSection(E)*((F+Fdot)*(F+Fdot)+Fdotdot*Fdotdot);
}
