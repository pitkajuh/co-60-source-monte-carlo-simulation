#ifndef FILE_H
#define FILE_H

#include "Section.h"

class File
{
protected:
  void print(const vector<string> &v)
  {
    cout<<"PRINT"<<'\n';
    string s="";
    for(const auto &i: v)
      {
	s+=i;
	s+=";";
      }
    cout<<s<<'\n';
  }
public:
  File(){}
  virtual ~File(){}
};

class File23: public File
{
public:
  Section *coherentScattering=nullptr;
  Section *incoherentScattering=nullptr;
  Section *pairFormation=nullptr;
  Section *photoIonization=nullptr;

  File23(){}
  File23(ifstream &tape, streampos &from, const vector<string> &MFs, const string &MT)
  {
    coherentScattering=new CoherentScattering(MT, tape, from, MFs[0]);
    incoherentScattering=new IncoherentScattering(MT, tape, from, MFs[1]);
    const vector<string> MFsPair{MFs.begin()+2, MFs.begin()+4};
    pairFormation=new PairFormation(MT, tape, from, MFsPair);
    const vector<string> MFsIon{MFs.begin()+4, MFs.end()};
    photoIonization=new PhotoIonization(MT, tape, from, MFsIon);
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
  Section *coherentFactor=nullptr;
  Section *incoherentFactor=nullptr;
  Section *imaginaryFactor=nullptr;
  Section *realFactor=nullptr;

  File27(){}
  File27(ifstream &tape, streampos &from, const vector<string> &MFs, const string &MT)
  {
    coherentFactor=new CoherentFactor(MT, tape, from, MFs[0]);
    incoherentFactor=new IncoherentFactor(MT, tape, from, MFs[1]);
    imaginaryFactor=new ImaginaryFactor(MT, tape, from, MFs[2]);
    realFactor=new RealFactor(MT, tape, from, MFs[3]);
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
