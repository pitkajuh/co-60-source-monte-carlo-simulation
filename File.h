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
  virtual vector<pair<unsigned, double>> Get(const double photonEnergy)=0;
  virtual ~File(){}
};

class File23: public File
{
public:
  Section *coherentScattering=nullptr;
  Section *incoherentScattering=nullptr;
  Section *pairFormation=nullptr;
  Section *photoIonization=nullptr;

  vector<pair<unsigned, double>> Get(const double photonEnergy) override
  {
    const vector<pair<unsigned, double>> coherentScatteringValue=coherentScattering->GetValue(photonEnergy);
    const vector<pair<unsigned, double>> incoherentScatteringValue=incoherentScattering->GetValue(photonEnergy);
    const vector<pair<unsigned, double>> pairFormationValue=pairFormation->GetValue(photonEnergy);
    const vector<pair<unsigned, double>> photoIonizationValue=photoIonization->GetValue(photonEnergy);
    vector<pair<unsigned, double>> v;
    v.reserve(coherentScatteringValue.size()+incoherentScatteringValue.size()+pairFormationValue.size()+photoIonizationValue.size());
    v.insert(v.begin(), coherentScatteringValue.begin(), coherentScatteringValue.end());
    v.insert(v.end(), incoherentScatteringValue.begin(), incoherentScatteringValue.end());
    v.insert(v.end(), pairFormationValue.begin(), pairFormationValue.end());
    v.insert(v.end(), photoIonizationValue.begin(), photoIonizationValue.end());
    return v;
  }
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
  Section *incoherentFunction=nullptr;
  Section *imaginaryFactor=nullptr;
  Section *realFactor=nullptr;

  vector<pair<unsigned, double>> Get(const double photonEnergy) override
  {
    const vector<pair<unsigned, double>> coherentFactorValue=coherentFactor->GetValue(photonEnergy);
    const vector<pair<unsigned, double>> incoherentFunctionValue=incoherentFunction->GetValue(photonEnergy);
    const vector<pair<unsigned, double>> imaginaryFactorValue=imaginaryFactor->GetValue(photonEnergy);
    const vector<pair<unsigned, double>> realFactorValue=realFactor->GetValue(photonEnergy);
    vector<pair<unsigned, double>> v;
    v.reserve(coherentFactorValue.size()+incoherentFunctionValue.size()+imaginaryFactorValue.size()+realFactorValue.size());
    v.insert(v.begin(), coherentFactorValue.begin(), coherentFactorValue.end());
    v.insert(v.end(), incoherentFunctionValue.begin(), incoherentFunctionValue.end());
    v.insert(v.end(), imaginaryFactorValue.begin(), imaginaryFactorValue.end());
    v.insert(v.end(), realFactorValue.begin(), realFactorValue.end());
    return v;
  }
  File27(){}
  File27(ifstream &tape, streampos &from, const vector<string> &MFs, const string &MT)
  {
    coherentFactor=new CoherentFactor(MT, tape, from, MFs[0]);
    incoherentFunction=new IncoherentFactor(MT, tape, from, MFs[1]);
    imaginaryFactor=new ImaginaryFactor(MT, tape, from, MFs[2]);
    realFactor=new RealFactor(MT, tape, from, MFs[3]);
  }
  ~File27()
  {
    delete coherentFactor;
    delete incoherentFunction;
    delete imaginaryFactor;
    delete realFactor;
  }
};

#endif
