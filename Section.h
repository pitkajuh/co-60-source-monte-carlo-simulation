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

  }
 public:
  // unsigned MT;
  // vector<Record> records;
};

class PairFormation: public Section
{
public:
};

class PhotoIonization: public Section
{
public:
};

class Scattering: public Section
{
public:
};

class Factor: public Section
{
public:
};

class CoherentScattering: public Scattering
{
public:
};

class IncoherentScattering: public Scattering
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
