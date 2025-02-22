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
    // reactions[0]={0, total};
    std::sort(reactions.begin(), reactions.end(), sortFunction);
    for(const auto &i:reactions)
      {
	cout<<i.first<<" "<<i.second<<'\n';
      }
    return {total, reactions};
  }

  void GetReaction(const double photonEnergy)
  {
    bool b1;
    // bool b2;
    // unsigned j=0;
    const pair<double, vector<pair<unsigned, double>>> crossSections=GetCrossSection(photonEnergy);
    const double crossSectionRandom=RNG(0, 1)*crossSections.first;

    for(unsigned i=1; i<crossSections.second.size(); i++)
     {
       b1=crossSectionRandom>crossSections.second[i].second;
       // b2=crossSectionRandom<=crossSections.second[i-1].second;
       // cout<<i<<"REACTION "<<crossSections.second[i-1].first<<'\n';
       // cout<<crossSections.second[i].second<<"<"<<crossSectionRandom<<"<="<<crossSections.second[i-1].second<<'\n';

       // if(b1 and b2)
       if(b1)
	 {
	   cout<<i<<" FINAL REACTION1 "<<crossSections.second[i-1].first<<'\n';
	   cout<<i<<" "<<crossSectionRandom<<">"<<crossSections.second[i].second<<'\n';
	   // cout<<crossSections.second[i].second<<"<"<<crossSectionRandom<<"<="<<crossSections.second[i-1].second<<'\n';
	   // j=i;
	   break;
	 }
     }
    // return crossSections[j];
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
