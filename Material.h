#ifndef MATERIAL_H
#define MATERIAL_H

#include "MassAttenuation.h"
#include "Tape.h"
#include "Reactions.h"
#include "PhotonAngularDistribution.h"
#include "CentralDifference.h"
#include "GaussSeidel.h"
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
    // Use linear interpolation to get the mass attenuation value corresponding to photonEnergy and turns it to linear attenuation coefficient. Returns the value in 1/m.
    return 100*density*(muMap[energyPrevious].first+(photonEnergy-energyPrevious)*(muMap[energyCurrent].first-muMap[energyPrevious].first)/(energyCurrent-energyPrevious));
  }

  // void AngularDistribution(PhotonAngularDistribution *d, const string name, const double lim, const double accuracy, const double deltaX, const double deltaY, const unsigned N)
  // {
  //   incoherent=new IncoherentAngularDistribution(endf);

  //   CentralDifference cd(incoherent, -1, 1, 1, lim, N, name, accuracy);
  //   GaussSeidel gs(cd.discretized, deltaX*deltaY, N, name, accuracy);
  //   // incoherent->d2sigmadmudE=gs.result;
  // }

  void CreateAngularDistribution(const double limIncoherent, const double limCoherent, const double accuracyIncoherent, const double accuracyCoherent)
  {
    const unsigned N=100;
    const double deltaX=(double)2/N;
    double deltaY=(double)(limIncoherent-1)/N;
    // AngularDistribution(new IncoherentAngularDistribution(endf), name+"incoherent", limIncoherent, accuracyIncoherent, deltaX, deltaY, N);
    incoherent=new IncoherentAngularDistribution(endf);

    CentralDifference cd(incoherent, -1+deltaX, 1-deltaX, 1, limIncoherent, N, name+"incoherent");
    GaussSeidel gs(cd.discretized, deltaX*deltaY, N, name+"incoherent", accuracyIncoherent);
    incoherent->d2sigmadmudE=gs.result;

    coherent=new CoherentAngularDistribution(endf);
    deltaY=(double)(limCoherent-1)/N;
    CentralDifference cd1(coherent, -1, 1, 1, limCoherent, N, name+"coherent");
    GaussSeidel gs1(cd1.discretized, deltaX*deltaY, N, name+"coherent", accuracyCoherent);
    coherent->d2sigmadmudE=gs1.result;
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
    CreateAngularDistribution(limIncoherent, limCoherent, 1e-24, 1e-26);
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
    CreateAngularDistribution(limIncoherent, limCoherent, 1e-32, 1e-32);
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
    CreateAngularDistribution(limIncoherent, limCoherent, 1e-32, 1e-32);
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
    CreateAngularDistribution(limIncoherent, limCoherent, 1e-32, 1e-32);
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
    CreateAngularDistribution(limIncoherent, limCoherent, 1e-32, 1e-35);
    density=1;
    muMap=muMapIodine;
  }
};

#endif
