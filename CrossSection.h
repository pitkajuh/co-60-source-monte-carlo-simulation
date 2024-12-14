#ifndef CROSSSECTION_H
#define CROSSSECTION_H

#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;

struct CrossSection
{
 public:
  unsigned MF;
  unsigned MT;
  map<double, vector<double>> crossSection;
  CrossSection(){}
  CrossSection(const string &reaction, const map<double, vector<double>> &map)
  {
    crossSection=map;
    MF=std::stoi(reaction.substr(4, 2));
    MT=std::stoi(reaction.substr(6, 8));
  }
};

class CrossSections
{
public:
  CrossSection totalCrossSection;
  vector<CrossSection> crossSections={};
  CrossSections(){}
  CrossSections(const vector<CrossSection> &cS)
  {
    totalCrossSection=cS[0];
    crossSections=cS;
    crossSections.erase(crossSections.begin()+0);
  }
};

#endif
