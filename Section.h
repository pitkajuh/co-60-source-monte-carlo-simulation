#ifndef SECTION_H
#define SECTION_H

#include <fstream>
#include "Record.h"

using std::ifstream;
using std::streampos;

class Section
{
private:
  void GetSection(ifstream &tape, streampos &from)
  {
    cout<<"ReadTape"<<'\n';
    string record;
    tape.seekg(from);

    while(getline(tape, record))
      {
	cout<<record<<'\n';
      }
    tape.close();
  }
 public:
  unsigned MF;
  // vector<Record> records;
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
  CoherentScattering()
  {
    MF=502;
  }
  CoherentScattering(const unsigned MT)
  {
    MF=502;
  }
  ~CoherentScattering(){}
};

class IncoherentScattering: public Scattering
{
public:
  IncoherentScattering()
  {
    MF=504;

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
