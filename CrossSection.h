#ifndef CROSSESCTION_H
#define CROSSESCTION_H

#include "Tape.h"

class CrossSection
{
public:
  CrossSection(){}
  virtual ~CrossSection(){}
};

class MicroscopicCrossSection: public CrossSection
{
protected:
  Section *pairFormation=nullptr;
  Section *photoIonization=nullptr;
  Section *coherentScattering=nullptr;
  Section *incoherentScattering=nullptr;
public:

  void GetCrossSection(const double photonEnergy)
  {
    double total=0;
    vector<pair<unsigned, double>> reactions;
    const vector<Section*> sections={pairFormation, photoIonization, coherentScattering, incoherentScattering};

    for(const auto &section:sections)
      {
	for(const auto &reaction:section->GetValue(photonEnergy))
	  {
	    total+=reaction.second;
	    cout<<reaction.first<<" "<<reaction.second<<'\n';
	    reactions.emplace_back(reaction);
	    // cout<<reaction[0]<<" "<<reaction[1]<<'\n';
	  }
	cout<<" "<<'\n';
      }
  }

  MicroscopicCrossSection(){}
  MicroscopicCrossSection(Tape *tape)
  {
    pairFormation=tape->MF23->pairFormation;
    photoIonization=tape->MF23->photoIonization;
    coherentScattering=tape->MF23->coherentScattering;
    incoherentScattering=tape->MF23->incoherentScattering;
  }
  ~MicroscopicCrossSection(){}
};

#endif
