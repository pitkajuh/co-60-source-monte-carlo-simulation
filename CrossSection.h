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
    const unsigned index=crossSection[energyPrevious].size()-1;
    return crossSection[energyPrevious][index]+(photonEnergy-energyPrevious)*(crossSection[energyCurrent][index]-crossSection[energyPrevious][index])/(energyCurrent-energyPrevious);
  }
  CrossSection(){}
  CrossSection(const pair<unsigned, unsigned> &reaction, const map<double, vector<double>> &map)
  {
    crossSection=map;
    MF=reaction.first;
    MT=reaction.second;
  }
};

class MicroscopicCrossSection: public CrossSection
{
public:

};

class MacroscopicCrossSection: public CrossSection
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
    double total=0;
    double p=0;
    vector<pair<unsigned, double>> crossSections;
    crossSections.reserve(MF23.size());

    for(auto &c: MF23)
      {
	p=c.GetCrossSection(photonEnergy);
	total+=p;
	crossSections.push_back({c.MT, p});
	cout<<c.MT<<" "<<p<<" "<<total<<'\n';
      }
    cout<<total<<'\n';
    std::sort(crossSections.begin(), crossSections.end(), sortFunction);
    return {total, crossSections};
  }
public:
  CrossSection totalCrossSection;
  vector<CrossSection> MF23;
  vector<CrossSection> MF27;

  void PDF(const double photonEnergy)
  {
   const pair<double, vector<pair<unsigned, double>>> crossSections=GetProbability(photonEnergy);
   const double crossSectionTotal=crossSections.first;
   const double crossSectionRandom=RNG(0, 1)*crossSectionTotal;
   double i1;
   double i2;

   for(unsigned crossSection=1; crossSection<crossSections.second.size(); crossSection++)
     {
       i1=crossSections.second[crossSection-1].second;
       i2=crossSections.second[crossSection].second;

       if(i2<crossSectionRandom and crossSectionRandom<=i1)
	 {
	   cout<<"AOE "<<crossSections.second[crossSection-1].first<<" "<<i2<<"<"<<crossSectionRandom<<"<="<<i1<<'\n';
	   break;
	 }
     }
  }
  CrossSections(){}
  CrossSections(const vector<CrossSection> &file23, const vector<CrossSection> &file27)
  {
    totalCrossSection=file23[0];
    MF23=file23;
    MF23.erase(MF23.begin()+0);
    MF27=file27;
  }
};

#endif
