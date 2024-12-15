#ifndef CROSSSECTION_H
#define CROSSSECTION_H

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "math.h"

using std::cout;
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

    if(energyPrevious==0) return 0;

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

  pair<double, vector<double>> GetProbability(const double photonEnergy)
  {
    vector<double> probabilities;
    probabilities.reserve(MF23.size());
    // const double rng=RNG(0, 1);
    // double ee=20e6;
    // double crossSectionTotal=totalCrossSection.GetCrossSection(photonEnergy);
    // double crossSectionTotal=totalCrossSection.GetCrossSection(ee);
    double total=0;
    double e=0;
    // std::cout<<crossSectionTotal<<'\n';

    for(auto &c: MF23)
      {
	e=c.GetCrossSection(photonEnergy);
	// e=c.GetCrossSection(ee);
	total+=e;
	probabilities.push_back(e);
	// std::cout<<c.MT<<" "<<c.MF<<" "<<e<<" "<<total<<" "<<total/crossSectionTotal<<'\n';
      }
    std::sort(probabilities.begin(), probabilities.end());
    return {total, probabilities};
  }
  void CreatePDF(const double photonEnergy)
  {
   const pair<double, vector<double>> probabilities=GetProbability(photonEnergy);

   for(const auto &p: probabilities.second)
     {
       cout<<p/probabilities.first<<'\n';
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
