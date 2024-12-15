#include <random>

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
