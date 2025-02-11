#ifndef MATERIAL_H
#define MATERIAL_H

#include "MassAttenuation.h"
#include "Tape.h"
#include "PhotonAngularDistribution.h"
#include "CentralDifference.h"

class Material
{
protected:
  const double GetMuValue(const double photonEnergy, map<double, pair<double, double>> &muMap, const double density)
  {
    double energyPrevious=0;
    double energyCurrent=0;

    for(const auto& [energy, mu]: muMap)
      {
	if(energy>=photonEnergy)
	  {
	    energyCurrent=energy;
	    break;
	  }
	energyPrevious=energy;
      }
    // Use linear interpolation to get the mass attenuation value corresponding to photonEnergy and turns it to linear attenuation coefficient. Returns the value in 1/m.
    return 100*density*(muMap[energyPrevious].first+(photonEnergy-energyPrevious)*(muMap[energyCurrent].first-muMap[energyPrevious].first)/(energyCurrent-energyPrevious));
  }
  void CreateAngularDistribution(const double limIncoherent, const double limCoherent)
  {
    // incoherent=new IncoherentAngularDistribution(endf);
    // CentralDifference cd(incoherent, -1, 1, 1, limIncoherent, 100);
    coherent=new CoherentAngularDistribution(endf);
    CentralDifference cd1(coherent, -1, 1, 1, limCoherent, 100);
  }
public:
  Tape *endf=nullptr;
  PhotonAngularDistribution *incoherent=nullptr;
  PhotonAngularDistribution *coherent=nullptr;
  double density=0; // in g/cm3
  map<double, pair<double, double>> muMap;
  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}
  virtual ~Material()
  {
    delete incoherent;
    delete coherent;
  }
};

class Steel: public Material
{
public:
  Steel(const string &endfTape, const double limIncoherent, const double limCoherent)
  {
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=7.874;
    muMap=muMapSteel;
  }
};

class Sodium: public Material
{
public:
  Sodium(const string &endfTape, const double limIncoherent, const double limCoherent)
  {
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=1;
    muMap=muMapSodium;
  }
};

class Titanium: public Material
{
public:
  Titanium(const string &endfTape, const double limIncoherent, const double limCoherent)
  {
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=1;
    muMap=muMapTitanium;
  }
};

class Nitrogen: public Material
{
public:
  Nitrogen(const string &endfTape, const double limIncoherent, const double limCoherent)
  {
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=1;
    muMap=muMapNitrogen;
  }
};

#endif
