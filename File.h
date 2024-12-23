#ifndef FILE_H
#define FILE_H

#include "Section.h"

class File
{
protected:

public:
  string MT;

  File(){}
  virtual ~File(){}
};

class File23: public File
{
public:
  Section *coherentScattering;
  Section *incoherentScattering;
  Section *pairFormation;
  Section *photoIonization;

  File23(){}
  File23(ifstream &tape, streampos &from, const vector<string> &MFs)
  {
    MT="23";
    // GetMF(tape, from, MT);
    coherentScattering=new CoherentScattering(MT, tape, from, MFs[0]);
    incoherentScattering=new IncoherentScattering(MT, tape, from, MFs[1]);
    pairFormation=new PairFormation(MT, tape, from);
    photoIonization=new PhotoIonization(MT, tape, from);
  }
  ~File23()
  {
    delete pairFormation;
    delete photoIonization;
    delete incoherentScattering;
    delete coherentScattering;
  }
};

class File27: public File
{
public:
  // Section *coherentFactor;
  // Section *incoherentFactor;
  // Section *imaginaryFactor;
  // Section *realFactor;

  // File27(){}
  // File27(ifstream &tape, streampos &from, const vector<string> &MFs)
  // {
  //   MT=27;
  //   coherentFactor=new CoherentFactor(MT, tape, from);
  //   incoherentFactor=new IncoherentFactor(MT, tape, from);
  //   imaginaryFactor=new ImaginaryFactor(MT, tape, from);
  //   realFactor=new RealFactor(MT, tape, from);
  // }
  // ~File27()
  // {
  //   delete coherentFactor;
  //   delete incoherentFactor;
  //   delete imaginaryFactor;
  //   delete realFactor;
  // }
};

#endif
