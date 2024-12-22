#ifndef SECTION_H
#define SECTION_H

#include "Record.h"

class Section
{
protected:

  void GetSection(ifstream &tape, streampos &from, unsigned MT, unsigned MF)
  {
    cout<<"ReadTape "<<MF<<" "<<MT<<'\n';
    string record;
    string id;
    unsigned size;
    tape.seekg(from);
    const string id2=std::to_string(MT)+std::to_string(MF);

    while(getline(tape, record))
      {
	size=record.size();
	id=record.substr(size-id2.size(), size);
	if(id==id2) break;
      }
    from=tape.tellg();
  }
 public:
  unsigned MF;
  Records records;
  virtual ~Section(){}
};

class PairFormation: public Section
{
private:
  void MultipleMF(ifstream &tape, streampos &from, const unsigned MT, const vector<unsigned> &MFs)
  {
    for(const auto &MF: MFs)
      {
	// cout<<MF<<'\n';
	GetSection(tape, from, MT, MF);
	records.GetRecord(tape, from, MF, MT);
      }
  }
public:
  PairFormation(){}
  PairFormation(const unsigned MT, ifstream &tape, streampos &from)
  {
    // MF=517;
    const vector<unsigned> MFs={515, 517};
    // GetSection(tape, from, MT);
    // records.GetRecord(tape, from, MF, MT);
    MultipleMF(tape, from, MT, MFs);
  }
  ~PairFormation(){}
};

class PhotoIonization: public Section
{
public:
  PhotoIonization *next=nullptr;
  PhotoIonization(){}
  PhotoIonization(const unsigned MT, ifstream &tape, streampos &from)
  {
    MF=100;
    cout<<tape.tellg()<<'\n';
    GetSection(tape, from, MT, MF);
    records.GetRecord(tape, from, MF, MT);
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
  CoherentScattering(const unsigned MT, ifstream &tape, streampos &from)
  {
    MF=502;
    GetSection(tape, from, MT, MF);
    records.GetRecord(tape, from, MF, MT);
  }
  ~CoherentScattering(){}
};

class IncoherentScattering: public Scattering
{
public:
  IncoherentScattering(){}
  IncoherentScattering(const unsigned MT, ifstream &tape, streampos &from)
  {
    MF=504;
    GetSection(tape, from, MT, MF);
    records.GetRecord(tape, from, MF, MT);
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
};

class IncoherentFactor: public Factor
{
public:
};

class ImaginaryFactor: public Factor
{
public:
};

class RealFactor: public Factor
{
public:
};

#endif
