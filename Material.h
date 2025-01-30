#ifndef MATERIAL_H
#define MATERIAL_H

#include "MassAttenuation.h"
#include "Tape.h"
#include "PhotonAngularDistribution.h"
#include "CentralDifference.h"

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

class Material
{
public:
  Tape *endf=nullptr;
  PhotonAngularDistribution *incoherent=nullptr;
  PhotonAngularDistribution *coherent=nullptr;
  double density=0; // in g/cm3
  map<double, pair<double, double>> muMap;
  // CrossSections crossSections;
  virtual const double GetMu(const double photonEnergy)=0;
  virtual ~Material()
  {
    delete incoherent;
    delete coherent;
  }
};

class Steel: public Material
{
public:
  Steel(const double d, const string &endfTape)
  {
    this->endf=new Tape(endfTape);
    // incoherent=new IncoherentAngularDistribution(endf);
    // CentralDifference cd(incoherent, -1, 1, 1, 2e6, 100);
    coherent=new CoherentAngularDistribution(endf);
    CentralDifference cd1(coherent, -1, 1, 1, 1e4, 100);
    density=d;
    muMap=muMapSteel;
    // crossSections=steelCrossSections;

  }
  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}

};

class Sodium: public Material
{
public:
  Sodium(const double d)
  {
    density=d;
    muMap=muMapSodium;
    // crossSections=sodiumCrossSections;
  }

  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}
};

class Titanium: public Material
{
public:
  Titanium(const double d)
  {
    density=d;
    muMap=muMapTitanium;
    // crossSections=titaniumCrossSections;
  }

  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}
};

class Nitrogen: public Material
{
public:
  Nitrogen(const double d)
  {
    density=d;
    muMap=muMapNitrogen;
    // crossSections=nitrogenCrossSections;
  }

  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}
};

#endif
