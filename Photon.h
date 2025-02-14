#ifndef PHOTON_H
#define PHOTON_H

#include "Coordinate.h"
#include "RadioNuclide.h"

class Photon
{
public:
  double energy;
  Coordinate direction;
  Coordinate origin;
  Coordinate to;
  Coordinate mfp;

  Photon(){}
  Photon(const double energy, const double mu, Coordinate origin)
  {
    this->origin=origin;
    this->energy=energy;
    direction=RandomEmissionDirection();
    to=direction*(-1/mu)*log(RNG(0, 1));
    mfp=origin+to;
  }
  ~Photon(){}
};


#endif
