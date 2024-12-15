#ifndef RADIONUCLIDE_H
#define RADIONUCLIDE_H

#include "Coordinate.h"
#include <iostream>
using std::cout;

class RadioNuclide
{
protected:
  double halfLife=0;
  unsigned activity=0;
public:
  unsigned GetActivity(){return activity;}
  virtual double PDF()=0;
  virtual ~RadioNuclide(){}
  RadioNuclide(){}
};

class Co60: public RadioNuclide
{
public:
  Co60(const unsigned activity)
  {
  this->halfLife=5.27;
  this->activity=activity;
  }
  double PDF()
  {
    // Probability distribution function of Co-60 gamma energies (eV).
    if(RNG(0, 1)<=0.85) return 1173e3;
    return 1332e3;
  }
};

#endif
