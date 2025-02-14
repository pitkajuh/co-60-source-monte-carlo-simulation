#ifndef REACTIONS_H
#define REACTIONS_H

#include <queue>
#include "Photon.h"

class Reactions
{
public:
  virtual void GetReaction(Photon photon)=0;
  Reactions(){}
  // Reactions(Cell *source, RadioNuclide *nuclide)
  // {
  // }
  ~Reactions(){}
};

class PairFormationReaction: public Reactions
{
public:
  void GetReaction(Photon photon) override
  {
  std::queue<Photon> b;
  }
};

class PhotoElectricEffectReaction: public Reactions
{
public:
  void GetReaction(Photon photon) override
  {
  std::queue<Photon> b;
  }
};

#endif
