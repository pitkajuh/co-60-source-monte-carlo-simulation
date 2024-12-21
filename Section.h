#ifndef SECTION_H
#define SECTION_H

#include "Record.h"

class Section
{
protected:
  void GetSection(ifstream &tape, streampos &from, unsigned MT)
  {
    cout<<"ReadTape "<<MF<<'\n';
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
    // cout<<record<<'\n';
    // from+=76;
    from=tape.tellg();
    // from+=76;
    // cout<<record<<'\n';
  }
 public:
  unsigned MF;
  Records records;
  virtual ~Section(){}
};

class PairFormation: public Section
{
public:
  PairFormation(){MF=517;}
  ~PairFormation(){}
};

class PhotoIonization: public Section
{
public:
  PhotoIonization *next=nullptr;
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
    GetSection(tape, from, MT);
    records.GetRecord(tape, from, MF);
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
    GetSection(tape, from, MT);
    records.GetRecord(tape, from, MF);
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
