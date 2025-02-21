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

  vector<pair<unsigned, double>> GetCrossSection(const double photonEnergy)
  {
    double total=0;
    vector<pair<unsigned, double>> reactions={{0, 0}};
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
    reactions[0]={0, total};
    std::sort(reactions.begin(), reactions.end(), sortFunction);
    for(const auto &i:reactions)
      {
	cout<<i.first<<" "<<i.second<<'\n';
      }
    return reactions;
  }

  void GetReaction(const double photonEnergy)
  {
    bool b1;
    bool b2;
    double i1;
    double i2;
    const vector<pair<unsigned, double>> crossSections=GetCrossSection(photonEnergy);
    const double crossSectionRandom=RNG(0, 1)*crossSections[0].second;

    for(unsigned i=1; i<crossSections.size(); i++)
     {
       i1=crossSections[i-1].second;
       i2=crossSections[i].second;
       b1=crossSectionRandom>i2;
       b2=crossSectionRandom<=i1;

       if(b1 and b2)
	 {
	   cout<<i<<" REACTION1 "<<crossSections[i].first<<'\n';
	   cout<<i2<<"<"<<crossSectionRandom<<"<="<<i1<<'\n';
	   break;
	 }
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
