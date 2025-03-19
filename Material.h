#ifndef MATERIAL_H
#define MATERIAL_H

#include "MassAttenuation.h"
#include "Tape.h"
#include "Reactions.h"
#include "PhotonAngularDistribution.h"
#include "CrossSection.h"

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
    return 100*density*(muMap[energyPrevious].first+(photonEnergy-energyPrevious)*(muMap[energyCurrent].first-muMap[energyPrevious].first)/(energyCurrent-energyPrevious));
  }

  void CreateAngularDistribution(const double limIncoherent, const double limCoherent)
  {
    // incoherent=new IncoherentAngularDistribution(endf, -1, 1, 2, limIncoherent, N, name+"incoherent");
    coherent=new CoherentAngularDistribution(endf, -1, 1, 2, limCoherent, 100, name+"coherent");
  }

public:
  string name;
  Tape *endf=nullptr;
  PhotonAngularDistribution *incoherent=nullptr;
  PhotonAngularDistribution *coherent=nullptr;
  MicroscopicCrossSection *microscopic=nullptr;

  double density=0; // in g/cm3
  map<double, pair<double, double>> muMap;
  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}

  virtual ~Material()
  {
    delete endf;
    delete incoherent;
    delete coherent;
    delete microscopic;
  }
};

class Steel: public Material
{
public:
  Steel(const string &endfTape, const double limIncoherent, const double limCoherent, const string &name)
  {
    this->name=name;
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=7.874;
    muMap=muMapSteel;
    microscopic=new MicroscopicCrossSection(endf);
    microscopic->GetReaction(100);
  }
};

class Sodium: public Material
{
public:
  Sodium(const string &endfTape, const double limIncoherent, const double limCoherent, const string &name)
  {
    this->name=name;
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=1;
    muMap=muMapSodium;
  }
};

class Titanium: public Material
{
public:
  Titanium(const string &endfTape, const double limIncoherent, const double limCoherent, const string &name)
  {
    this->name=name;
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=1;
    muMap=muMapTitanium;
  }
};

class Nitrogen: public Material
{
public:
  Nitrogen(const string &endfTape, const double limIncoherent, const double limCoherent, const string &name)
  {
    this->name=name;
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=1;
    muMap=muMapNitrogen;
  }
};

class Iodine: public Material
{
public:
  Iodine(const string &endfTape, const double limIncoherent, const double limCoherent, const string &name)
  {
    this->name=name;
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent);
    density=1;
    muMap=muMapIodine;
  }
};

#endif
