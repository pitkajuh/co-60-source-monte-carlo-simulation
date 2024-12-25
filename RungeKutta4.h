#ifndef RUNGEKUTTA4_H
#define RUNGEKUTTA4_H

struct RungeKutta4
{
public:
  RungeKutta4(){}
  RungeKutta4(const double E, const double S, const double mu, double (*f)(double, double, double))
  {

  }
  ~RungeKutta4(){}
};

#endif
