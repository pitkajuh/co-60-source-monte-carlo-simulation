#ifndef SECTION_H
#define SECTION_H

#include "Record.h"

using std::pair;

struct CrossSectionPair
{
public:
  unsigned MT;
  Records r;

  double GetValue(const double photonEnergy, map<double, double> &map1)
  {
    return r.GetValue(photonEnergy, map1);
  }
  CrossSectionPair(){}
  ~CrossSectionPair(){}
};

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
public:
  Records records;
  vector<CrossSectionPair> recordsAll;
  map<unsigned, map<double, double>> sections;

  vector<pair<unsigned, double>> GetValue(const double photonEnergy)
  {
    vector<pair<unsigned, double>> crossSections;
    crossSections.reserve(recordsAll.size());
    pair<unsigned, double> p;


    // for(const auto &section: sections)
    //   {

    //   }

    for(auto &record: recordsAll)
      {
	p={record.MT, record.r.GetValue(photonEnergy)};
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
    CrossSectionPair p;

    for(const auto &MF: MFs)
      {
	FindSection(tape, from, MT, MF);
	sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
	p.MT=std::stoi(MF);
	p.r=records;
	recordsAll.emplace_back(p);
      }
    records.clear();
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
    CrossSectionPair p;

    for(const auto &MF: MFs)
      {
	FindSection(tape, from, MT, MF);
	sections[std::stoi(MF)]=records.GetRecords(tape, from, MF, MT);
	p.MT=std::stoi(MF);
	p.r=records;
	recordsAll.emplace_back(p);
	// ionizationEnergy=recordsAll.back().r.energy[0];
	ionizationEnergy=records.map1.begin()->first;
	cout<<MF<<" ionizationEnergy "<<ionizationEnergy<<'\n';
	records.clear();
      }
    // for(const auto &i:recordsAll)
    //   {
    // 	cout<<"MT "<<i.MT<<'\n';
    // 	for(const auto &j:i.r.energy)
    // 	  {
    // 	    cout<<j<<'\n';
    // 	  }
    // 	cout<<" "<<'\n';
    //   }
    // ionizationEnergy=recordsAll[0].r.energy[0];
    // cout<<"ionizationEnergy "<<ionizationEnergy<<'\n';
    records.clear();
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
    CrossSectionPair p;
    p.MT=std::stoi(MF);
    p.r=records;
    recordsAll.emplace_back(p);
    records.clear();
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
    CrossSectionPair p;
    p.MT=std::stoi(MF);
    p.r=records;
    recordsAll.emplace_back(p);
    records.clear();
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
    CrossSectionPair p;
    p.MT=std::stoi(MF);
    p.r=records;
    recordsAll.emplace_back(p);
    records.clear();
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
    CrossSectionPair p;
    p.MT=std::stoi(MF);
    p.r=records;
    recordsAll.emplace_back(p);
    records.clear();
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
    CrossSectionPair p;
    p.MT=std::stoi(MF);
    p.r=records;
    recordsAll.emplace_back(p);
    records.clear();
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
    CrossSectionPair p;
    p.MT=std::stoi(MF);
    p.r=records;
    recordsAll.emplace_back(p);
    records.clear();
  }
  ~RealFactor(){}
};

#endif
