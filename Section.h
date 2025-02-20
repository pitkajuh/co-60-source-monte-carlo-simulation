#ifndef SECTION_H
#define SECTION_H

#include "Record.h"

using std::pair;

class Section
{
protected:
  void FindSection(ifstream &tape, streampos &from, const string &MT, const string &MF)
  {
    string record;
    string id;
    unsigned size;
    tape.seekg(from);
    const string id2=MT+MF;

    getline(tape, record);
    tape.seekg(from);

    while(getline(tape, record))
      {
	size=record.size();
	id=record.substr(size-id2.size(), size);

	if(id==id2) break;
      }

    from=tape.tellg();
    from+=76*2;
  }
  double GetValue1(const double photonEnergy, map<double, double> &map1)
  {
    double energyPrevious=0;
    double energyCurrent=0;

    for(const auto& [energy, record]: map1)
      {
	if(energy>=photonEnergy)
	  {
	    energyCurrent=energy;
	    break;
	  }
	energyPrevious=energy;
      }

    if(energyPrevious==0) return 0;
    const double previousValue=map1[energyPrevious];
    return previousValue+(photonEnergy-energyPrevious)*(map1[energyCurrent]-previousValue)/(energyCurrent-energyPrevious);
  }
public:
  Records records;
  map<unsigned, map<double, double>> sections;

  double GetLibraryValue(const double photonEnergy, const unsigned MT)
  {
    map<double, double> map1=sections[MT];
    return GetValue1(photonEnergy, map1);
  }

  vector<pair<unsigned, double>> GetValue(const double photonEnergy)
  {
    vector<pair<unsigned, double>> crossSections;
    pair<unsigned, double> p;
    double energy;

    for(auto &[MT, recordsMT]: sections)
      {
	energy=GetValue1(photonEnergy, recordsMT);
	p={MT, energy};
	crossSections.emplace_back(p);
      }
    return crossSections;
  }
  virtual ~Section(){}
};

class PairFormation: public Section
{
public:
  PairFormation(){}
  PairFormation(const string &MT, ifstream &tape, streampos &from, const vector<string> &MFs)
  {
    for(const auto &MF: MFs)
      {
	FindSection(tape, from, MT, MF);
	sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
      }
  }
  ~PairFormation(){}
};

class PhotoIonization: public Section
{
public:
  double ionizationEnergy;

  PhotoIonization(){}
  PhotoIonization(const string &MT, ifstream &tape, streampos &from, const vector<string> &MFs)
  {
    for(const auto &MF: MFs)
      {
	FindSection(tape, from, MT, MF);
	sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
      }
  }
  ~PhotoIonization(){}
};

class Scattering: public Section
{
public:
};

class CoherentScattering: public Scattering
{
public:
  CoherentScattering(){}
  CoherentScattering(const string &MT, ifstream &tape, streampos &from, const string &MF)
  {
    FindSection(tape, from, MT, MF);
    sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
  }
  ~CoherentScattering(){}
};

class IncoherentScattering: public Scattering
{
public:
  IncoherentScattering(){}
  IncoherentScattering(const string &MT, ifstream &tape, streampos &from, const string &MF)
  {
    FindSection(tape, from, MT, MF);
    sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
  }
  ~IncoherentScattering(){}
};

class Factor: public Section
{
public:
};

class CoherentFactor: public Factor
{
public:
  CoherentFactor(){}
  CoherentFactor(const string &MT, ifstream &tape, streampos &from, const string &MF)
  {
    FindSection(tape, from, MT, MF);
    sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
  }
  ~CoherentFactor(){}
};

class IncoherentFactor: public Factor
{
public:
  IncoherentFactor(){}
  IncoherentFactor(const string &MT, ifstream &tape, streampos &from, const string &MF)
  {
    FindSection(tape, from, MT, MF);
    sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
  }
  ~IncoherentFactor(){}
};

class ImaginaryFactor: public Factor
{
public:
  ImaginaryFactor(){}
  ImaginaryFactor(const string &MT, ifstream &tape, streampos &from, const string &MF)
  {
    FindSection(tape, from, MT, MF);
    sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
  }
  ~ImaginaryFactor(){}
};

class RealFactor: public Factor
{
public:
  RealFactor(){}
  RealFactor(const string &MT, ifstream &tape, streampos &from, const string &MF)
  {
    FindSection(tape, from, MT, MF);
    sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
  }
  ~RealFactor(){}
};

#endif
