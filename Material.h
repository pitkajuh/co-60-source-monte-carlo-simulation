#ifndef MATERIAL_H
#define MATERIAL_H

#include "MassAttenuation.h"

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
  double density=0; // in g/cm3
  map<double, pair<double, double>> muMap;
  virtual const double GetMu(const double photonEnergy)=0;
  virtual ~Material(){}
};

class Sodium: public Material
{
public:
  Sodium(const double d)
  {
    density=d;
    muMap=muMapSodium;
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
  }

  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}
};

class Steel: public Material
{
public:
  Steel(const double d)
  {
    density=d;
    muMap=muMapSteel;
  }
  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}
};

#endif
