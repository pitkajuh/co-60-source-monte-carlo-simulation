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
  Photon *next=nullptr;

  Photon(){}
  Photon(Cell *source, RadioNuclide *nuclide)
  {
    origin=source->GetInitialPosition();
    direction=RandomEmissionDirection();
    energy=nuclide->PDF();
    to=direction*(-1/source->material->GetMu(energy))*log(RNG(0, 1));
    mfp=origin+to;
  }
  ~Photon(){}
};


#endif
