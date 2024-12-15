#ifndef CROSSSECTION_H
#define CROSSSECTION_H

#include <map>
#include <vector>
#include <string>
#include "math.h"

using std::map;
using std::vector;
using std::string;
using std::pair;

class CrossSection
{
 public:
  unsigned MF;
  unsigned MT;
  map<double, vector<double>> crossSection;
  double GetCrossSection(const double photonEnergy)
  {
    double energyPrevious=0;
    double energyCurrent=0;
    // bool not1=0;

    for(const auto& [energy, cs]: crossSection)
      {
	if(energy>=photonEnergy)
	  {
	    energyCurrent=energy;
	    // not1=1;
	    break;
	  }
	energyPrevious=energy;
      }

    if(energyPrevious==0) return -1;

    // std::cout<<"FOUND?"<<not1<<" "<<photonEnergy<<" "<<energyPrevious<<" ;"<<energyCurrent<<";"<<'\n';
    return crossSection[energyPrevious][0]+(photonEnergy-energyPrevious)*(crossSection[energyCurrent][0]-crossSection[energyPrevious][0])/(energyCurrent-energyPrevious);
  }
  CrossSection(){}
  CrossSection(const pair<unsigned, unsigned> &reaction, const map<double, vector<double>> &map)
  {
    crossSection=map;
    MF=reaction.first;
    MT=reaction.second;
  }
};

class TotalCrossSection: public CrossSection
{
public:

};

class CrossSections
{
public:
  CrossSection totalCrossSection;
  vector<CrossSection> MF23;
  vector<CrossSection> MF27;
  void GetProbability(const double photonEnergy)
  {
    // const double rng=RNG(0, 1);
    double crossSectionTotal=totalCrossSection.GetCrossSection(photonEnergy);
    std::cout<<crossSectionTotal<<'\n';

    for(auto &c: MF23)
      {
	std::cout<<c.MT<<" "<<c.MF<<" "<<c.GetCrossSection(photonEnergy)/crossSectionTotal<<'\n';
      }
  }
  CrossSections(){}
  CrossSections(const vector<CrossSection> &cS23, const vector<CrossSection> &cS27)
  {
    totalCrossSection=cS23[0];
    MF23=cS23;
    MF23.erase(MF23.begin()+0);
    MF27=cS27;
  }
};

#endif
