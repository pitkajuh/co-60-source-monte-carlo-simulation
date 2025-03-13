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

  // void AngularDistribution(PhotonAngularDistribution *d, const string name, const double lim, const double accuracy, const double deltaX, const double deltaY, const unsigned N)
  // {
  //   incoherent=new IncoherentAngularDistribution(endf);
  //   CentralDifference cd(incoherent, -1, 1, 1, lim, N, name, accuracy);
  //   GaussSeidel gs(cd.discretized, deltaX*deltaY, N, name, accuracy);
  //   // incoherent->d2sigmadmudE=gs.result;
  // }

  void CreateAngularDistribution(const double limIncoherent, const double limCoherent)
  {
    const unsigned N=100;
    // incoherent=new IncoherentAngularDistribution(endf, -1, 1, 2, limIncoherent, N, name+"incoherent");
    coherent=new CoherentAngularDistribution(endf, -1, 1, 2, limCoherent, N, name+"coherent");
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
