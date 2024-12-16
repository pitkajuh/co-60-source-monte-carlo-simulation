#include <random>
#include <cmath>

const double e=1.602176634E-19;
const double m_e=0.51099895069E+06;
const double c=299792458;
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

double KleinNishinaCrossSection(const double E, const double mu)
{
  const double k=E/m_e;
  const double kdot=k/(1+k*(1-mu));
  const double kk=kdot/k;
  return M_PI*r_e*r_e*kk*kk*(1+mu*mu+k*kdot*(1-mu)*(1-mu));
}

double IncoherentScatteringCrossSection(const double E, const double mu, const double S)
{
  // S is the incoherent scattering function
  // mu is the cosine unit (cos(theta))
  // E is the incident photon energy
  return S*KleinNishinaCrossSection(E, mu);
}

double ThomsonCrossSection(const double photonEnergy, const double mu){return M_PI*r_e*(1+mu);}
