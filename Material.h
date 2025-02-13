#ifndef MATERIAL_H
#define MATERIAL_H

#include "MassAttenuation.h"
#include "Tape.h"
#include "PhotonAngularDistribution.h"
#include "CentralDifference.h"
#include "GaussSeidel.h"

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
  void CreateAngularDistribution(const double limIncoherent, const double limCoherent, const double accuracyIncoherent, const double accuracyCoherent)
  {
    const unsigned N=100;
    const double deltaX=(double)(2)/N;
    const double deltaY=(double)(limIncoherent-1)/N;
    incoherent=new IncoherentAngularDistribution(endf);

    CentralDifference cd(incoherent, -1, 1, 1, limIncoherent, N, name+"incoherent", accuracyIncoherent);
    incoherent->mu=cd.X;
    incoherent->Eprime=cd.Y;
    GaussSeidel gs(cd.discretized, deltaX, deltaY, N, name, accuracyIncoherent, cd.X, cd.Y);
    incoherent->d2sigmadmudE=gs.result;

    coherent=new CoherentAngularDistribution(endf);
    CentralDifference cd1(coherent, -1, 1, 1, limCoherent, N, name+"coherent", accuracyCoherent);
    GaussSeidel gs1(cd1.discretized, deltaX, deltaY, N, name, accuracyCoherent, cd1.X, cd1.Y);
    coherent->mu=cd1.X;
    coherent->Eprime=cd1.Y;
    coherent->d2sigmadmudE=gs1.result;
  }
  void GetIncoherentScattering(const double crossSection)
  {
    bool found1=0;
    unsigned column;
    unsigned row;

    for(unsigned i=0; i<incoherent->d2sigmadmudE.size(); i++)
      {
	for(unsigned j=0; i<incoherent->d2sigmadmudE[i].size(); j++)
	  {
	    if(incoherent->d2sigmadmudE[i][j]<crossSection)
	      {
		cout<<"found "<<i<<" "<<j<<'\n';
		found1=1;
		column=j;
		row=i;
		break;
	      }
	  }
	if(found1)
	  {
	    break;
	  }
      }
    cout<<row<<" "<<column<<" "<<incoherent->mu[row]<<" "<<incoherent->Eprime[column]<<'\n';
  }
public:
  string name;
  Tape *endf=nullptr;
  PhotonAngularDistribution *incoherent=nullptr;
  PhotonAngularDistribution *coherent=nullptr;
  double density=0; // in g/cm3
  map<double, pair<double, double>> muMap;
  const double GetMu(const double photonEnergy){return GetMuValue(photonEnergy, muMap, density);}

  virtual ~Material()
  {
    delete endf;
    delete incoherent;
    delete coherent;
  }
};

class Steel: public Material
{
public:
  Steel(const string &endfTape, const double limIncoherent, const double limCoherent, const string &name)
  {
    this->name=name;
    this->endf=new Tape(endfTape);
    CreateAngularDistribution(limIncoherent, limCoherent, 1e-32, 1e-34);
    density=7.874;
    muMap=muMapSteel;
    GetIncoherentScattering(3e-30);
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
