#ifndef RECORD_H
#define RECORD_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::ifstream;
using std::streampos;
using std::map;

struct Record
{
public:
  vector<double> recordv;

  void SaveRecord(std::ofstream &file)
  {
    for(const auto &i: recordv)
      {
	file<<i;
	file<<';';
      }

    file<<'\n';
  }

  void clear(){recordv.clear();}
  void CreateRecord(string &record)
  {
    unsigned size=record.size();
    const string sub=record.substr(0, 11);
    const char empty=' ';
    const char s1=sub[0];
    const char s2=sub[9];

    if(size>10)
      {
	record=record.substr(11, size);
	size=record.size();

	if(s1==empty and s2==empty) recordv.emplace_back(0);
	else recordv.emplace_back(stod(sub));

	if(size>=10) CreateRecord(record);
      }
    else if(size==10 and s1==empty and s2==empty) recordv.emplace_back(0);
    else recordv.emplace_back(stod(record));
  }
  double GetEnergy()
  {
    const double energy=recordv[0];
    recordv.erase(recordv.begin()+0);
    return energy;
  }
  Record(){recordv.reserve(6);}
  ~Record(){}
};

struct Records
{
protected:
  void printdouble(vector<double> v)
  {
    string s="";

    for(const auto &a: v)
      {
	s+=std::to_string(a);
	s+=";";
      }
    cout<<s<<'\n';
  }
public:
  map<double, double> map1;
  vector<double> energy;

  void clear(){map1.clear();}
  unsigned size(){return map1.size();}
  double GetValue(const double photonEnergy)
  {
    double energyPrevious=0;
    double energyCurrent=0;

    for(const auto& [energy, record]: map1)
      {
	if(energy>=photonEnergy)
	  {
	    energyCurrent=energy;
	    break;
	  }
	energyPrevious=energy;
      }

    if(energyPrevious==0) return 0;
    const double previousValue=map1[energyPrevious];
    return previousValue+(photonEnergy-energyPrevious)*(map1[energyCurrent]-previousValue)/(energyCurrent-energyPrevious);

  }
  void AddToMap(const Record &r)
  {
    const vector<double> v=r.recordv;

    for(unsigned i=1; i<v.size(); i+=2)
      {
	energy.emplace_back(v[i-1]);
	map1[v[i-1]]=v[i];
      }
  }
  void GetRecords(ifstream &tape, streampos &from, const string &MF, const string &MT)
  {
    string record;
    string id;
    const string MFstr=MT+MF;
    const unsigned size=MFstr.size();
    unsigned size2;
    Record r;
    tape.seekg(from);
    // std::ofstream file;
    // file.open(MT+MF+".txt");
    while(getline(tape, record))
      {
	size2=record.size();
	id=record.substr(size2-size, size2);
	if(id!=MFstr) break;
	record=record.substr(0, 66);
	r.CreateRecord(record);
	// r.SaveRecord(file);
	AddToMap(r);
	r.clear();
      }

    // file.close();
    from=tape.tellg();
    cout<<" "<<'\n';
  }

  Records(){}
  ~Records(){}
};

#endif
