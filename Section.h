#ifndef SECTION_H
#define SECTION_H

#include "Record.h"

class Section
{
protected:
  void FindSection(ifstream &tape, streampos &from, const unsigned MT, const unsigned MF)
  {
    cout<<"ReadTape "<<MF<<" "<<MT<<" from "<<from<<" At "<<tape.tellg()<<'\n';
    string record;
    string id;
    unsigned size;
    tape.seekg(from);
    const string id2=std::to_string(MT)+std::to_string(MF);
    string id3;

    getline(tape, record);

    if(record[74]=='0' and record[71]=='0')
      {
	changeFile=1;
	cout<<"END FILE "<<record[74]<<" "<<record[71]<<'\n';
      }

    tape.seekg(from);

    while(getline(tape, record))
      {
	size=record.size();
	id=record.substr(size-id2.size(), size);
	id3=id.substr(0, 2);

	if(id==id2 or changeFile) break;
      }
    from=tape.tellg();

    if(!changeFile) from+=76*2;
  }
 public:
  bool changeFile=0;
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
	FindSection(tape, from, MT, MF);
	records.GetRecord(tape, from, MF, MT);
      }
  }
public:
  PairFormation(){}
  PairFormation(const unsigned MT, ifstream &tape, streampos &from)
  {
    const vector<unsigned> MFs={515, 517};
    MultipleMF(tape, from, MT, MFs);
  }
  ~PairFormation(){}
};

class PhotoIonization: public Section
{
private:
  void MultipleMF(ifstream &tape, streampos &from, const unsigned MT, const vector<unsigned> &MFs)
  {
    for(const auto &MF: MFs)
      {
	if(!changeFile)
	  {
	    FindSection(tape, from, MT, MF);
	    records.GetRecord(tape, from, MF, MT);
	  }
      }
  }
public:
  PhotoIonization(){}
  PhotoIonization(const unsigned MT, ifstream &tape, streampos &from)
  {
    vector<unsigned> MFs;
    MFs.reserve(572-534);
    for(unsigned i=534; i<573; i++){MFs.emplace_back(i);}
    MultipleMF(tape, from, MT, MFs);
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
    FindSection(tape, from, MT, MF);
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
    FindSection(tape, from, MT, MF);
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
  CoherentFactor(){}
  CoherentFactor(const unsigned MT, ifstream &tape, streampos &from)
  {
    MF=502;
    FindSection(tape, from, MT, MF);
    records.GetRecord(tape, from, MF, MT);
  }
  ~CoherentFactor(){}
};

class IncoherentFactor: public Factor
{
public:
  IncoherentFactor(){}
  IncoherentFactor(const unsigned MT, ifstream &tape, streampos &from)
  {
    MF=504;
    FindSection(tape, from, MT, MF);
    records.GetRecord(tape, from, MF, MT);
  }
  ~IncoherentFactor(){}
};

class ImaginaryFactor: public Factor
{
public:
  ImaginaryFactor(){}
  ImaginaryFactor(const unsigned MT, ifstream &tape, streampos &from)
  {
    MF=505;
    FindSection(tape, from, MT, MF);
    records.GetRecord(tape, from, MF, MT);
  }
  ~ImaginaryFactor(){}
};

class RealFactor: public Factor
{
public:
  RealFactor(){}
  RealFactor(const unsigned MT, ifstream &tape, streampos &from)
  {
    MF=506;
    FindSection(tape, from, MT, MF);
    records.GetRecord(tape, from, MF, MT);
  }
  ~RealFactor(){}
};

#endif
