#ifndef PHOTONANGULARDISTRIBUTION_H
#define PHOTONANGULARDISTRIBUTION_H

#include "Tape.h"
#include "RungeKutta4.h"

class PhotonAngularDistribution
{
 public:
  Tape *tape=nullptr;

  PhotonAngularDistribution(){}
  virtual ~PhotonAngularDistribution(){delete tape;}
};

class IncoherentAngularDistribution: public PhotonAngularDistribution
{
 public:
  IncoherentAngularDistribution(){}
  IncoherentAngularDistribution(Tape *tape)
  {

  }
  ~IncoherentAngularDistribution(){}
};

#endif
