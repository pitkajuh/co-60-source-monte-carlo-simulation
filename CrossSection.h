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

    for(const auto& [energy, cs]: crossSection)
      {
	if(energy>=photonEnergy)
	  {
	    energyCurrent=energy;
	    break;
	  }
	energyPrevious=energy;
      }

    if(energyPrevious==0) return 0;

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
private:
  static bool sortFunction(const pair<unsigned, double> &p1, const pair<unsigned, double> &p2)
  {
    return p1.second>p2.second;
  }

  pair<double, vector<pair<unsigned, double>>> GetProbability(const double photonEnergy)
  {
    vector<pair<unsigned, double>> probabilities;
    probabilities.reserve(MF23.size());
    double total=0;
    double e=0;

    for(auto &c: MF23)
      {
	e=c.GetCrossSection(photonEnergy);
	total+=e;
	probabilities.push_back({c.MT, e});
      }
    std::sort(probabilities.begin(), probabilities.end(), sortFunction);
    return {total, probabilities};
  }
public:
  CrossSection totalCrossSection;
  vector<CrossSection> MF23;
  vector<CrossSection> MF27;

  void CreatePDF(const double photonEnergy)
  {
   pair<double, vector<pair<unsigned, double>>> probabilities=GetProbability(photonEnergy);

   for(const auto &p: probabilities.second)
     {
       cout<<p.first<<" "<<p.second/probabilities.first<<'\n';
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
