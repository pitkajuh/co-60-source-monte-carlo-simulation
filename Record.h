#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using std::cout;
using std::vector;
using std::ifstream;
using std::streampos;
using std::map;

struct Record
{
public:
  vector<double> recordv;

  void clear(){recordv.clear();}
  void CreateRecord(string &record)
  {
    recordv.reserve(6);
    const string sub=record.substr(0, 11);
    const char empty=' ';
    const char s1=sub[0];
    const char s2=sub[9];

    if(record.size()>10)
      {
	record=record.substr(11, record.size());

	if(s1==empty and s2==empty) recordv.emplace_back(0);
	else recordv.emplace_back(stod(sub));

	if(record.size()>=10) CreateRecord(record);
      }
    else if(record.size()==10 and s1==empty and s2==empty) recordv.emplace_back(0);
    else recordv.emplace_back(stod(record));
  }
  double GetEnergy()
  {
    const double energy=recordv[0];
    recordv.erase(recordv.begin()+0);
    return energy;
  }
  Record(){}
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
  map<double, Record> map1;

  void GetRecords(ifstream &tape, streampos &from, const string &MF, const string &MT)
  {
    string record;
    string id;
    const string MFstr=MT+MF;
    Record r;
    tape.seekg(from);

    while(getline(tape, record))
      {
	id=record.substr(record.size()-MFstr.size(), record.size());
	if(id!=MFstr) break;
	record=record.substr(0, 66);
	r.CreateRecord(record);
	map1[r.GetEnergy()]=r;
	r.clear();
      }
    from=tape.tellg();
    cout<<" "<<'\n';
  }

  Records(){}
  ~Records(){}
};

#endif
