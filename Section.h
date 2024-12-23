#ifndef SECTION_H
#define SECTION_H

#include "Record.h"

class Section
{
protected:
  void FindSection(ifstream &tape, streampos &from, const string &MT, const string &MF)
  {
    cout<<"ReadTape "<<MT<<MF<<" from "<<from<<" At "<<tape.tellg()<<'\n';
    string record;
    string id;
    unsigned size;
    tape.seekg(from);
    const string id2=MT+MF;

    getline(tape, record);

    if(record[74]=='0' and record[71]=='0') changeFile=1;

    tape.seekg(from);

    while(getline(tape, record))
      {
	size=record.size();
	id=record.substr(size-id2.size(), size);

	if(id==id2 or changeFile) break;
      }

    from=tape.tellg();

    if(!changeFile) from+=76*2;
  }
public:
  bool changeFile=0;
  Records records;
  vector<Records> recordsAll;
  vector<double> GetValue(const double photonEnergy)
  {
    vector<double> energies;
    for(auto &record: recordsAll){energies.emplace_back(record.GetValue(photonEnergy));}
    return energies;
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
	records.GetRecords(tape, from, MF, MT);
	recordsAll.emplace_back(records);
      }
  }
  ~PairFormation(){}
};

class PhotoIonization: public Section
{
public:
  PhotoIonization(){}
  PhotoIonization(const string &MT, ifstream &tape, streampos &from, const vector<string> &MFs)
  {
    for(const auto &MF: MFs)
      {
	FindSection(tape, from, MT, MF);
	records.GetRecords(tape, from, MF, MT);
	recordsAll.emplace_back(records);
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
    records.GetRecords(tape, from, MF, MT);
    recordsAll.emplace_back(records);
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
    records.GetRecords(tape, from, MF, MT);
    recordsAll.emplace_back(records);
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
    records.GetRecords(tape, from, MF, MT);
    recordsAll.emplace_back(records);
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
    records.GetRecords(tape, from, MF, MT);
    recordsAll.emplace_back(records);
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
    records.GetRecords(tape, from, MF, MT);
    recordsAll.emplace_back(records);
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
    records.GetRecords(tape, from, MF, MT);
    recordsAll.emplace_back(records);
  }
  ~RealFactor(){}
};

#endif
