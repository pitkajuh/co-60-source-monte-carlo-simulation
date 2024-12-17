#ifndef REACTIONS_H
#define REACTIONS_H

#include "Photon.h"

const double m_e=0.51099895069E+06;

void ComptonScattering(Photon &photon)
{
  // mu=cos(theta), get from ENDF
  const double mu=0;
  photon.energy=photon.energy/(1+(photon.energy/m_e))*(1-mu);
  // update photon.direction using mu
}


#endif
