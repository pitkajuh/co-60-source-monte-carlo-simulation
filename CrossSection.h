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

  static bool sortFunction(const pair<unsigned, double> &p1, const pair<unsigned, double> &p2)
  {
    return p1.second>p2.second;
  }
public:

  pair<double, vector<pair<unsigned, double>>> GetCrossSection(const double photonEnergy)
  {
    double total=0;
    vector<pair<unsigned, double>> reactions;
    pair<double, vector<pair<unsigned, double>>> result;
    const vector<Section*> sections={pairFormation, photoIonization, coherentScattering, incoherentScattering};

    for(const auto &section:sections)
      {
	for(const auto &reaction:section->GetValue(photonEnergy))
	  {
	    total+=reaction.second;
	    cout<<"Reaction "<<reaction.first<<" "<<reaction.second<<'\n';
	    reactions.emplace_back(reaction);
	  }
	cout<<" "<<'\n';
      }
    std::sort(reactions.begin(), reactions.end(), sortFunction);
    return result;
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
