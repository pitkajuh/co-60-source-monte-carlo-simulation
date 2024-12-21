#ifndef FILE_H
#define FILE_H

#include "Section.h"

class File
{
 public:
  unsigned MT;

  virtual void AddSection()=0;
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

  void AddSection() override
  {

  }
  File23(){}
  File23(ifstream &tape, streampos &from)
  {
    MT=23;
    coherentScattering=new CoherentScattering(MT, tape, from);
    incoherentScattering=new IncoherentScattering(MT, tape, from);
    pairFormation=new PairFormation(MT, tape, from);
  photoIonization=new PhotoIonization;
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
  Section *coherentFactor=new CoherentFactor;
  Section *incoherentFactor=new IncoherentFactor;
  Section *imaginaryFactor=new ImaginaryFactor;
  Section *realFactor=new RealFactor;

  void AddSection() override
  {

  }
  ~File27()
  {
    delete coherentFactor;
    delete incoherentFactor;
    delete imaginaryFactor;
    delete realFactor;
  }
};

#endif
