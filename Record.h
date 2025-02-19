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
  void AddToMap(const Record &r, map<double, double> &map1)
  {
    const vector<double> v=r.recordv;

    for(unsigned i=1; i<v.size(); i+=2)
      {
	map1[v[i-1]]=v[i];
      }
  }
  map<double, double> GetRecords(ifstream &tape, streampos &from, const string &MF, const string &MT)
  {
    string record;
    string id;
    Record r;
    unsigned size2;
    map<double, double> map1;
    tape.seekg(from);
    std::ofstream file;
    file.open("text/"+MT+MF+".txt");
    const string MFstr=MT+MF;
    const unsigned size=MFstr.size();
    cout<<"ID="<<MFstr<<" "<<map1.size()<<'\n';

    while(getline(tape, record))
      {
	size2=record.size();
	id=record.substr(size2-size, size2);
	if(id!=MFstr) break;
	record=record.substr(0, 66);
	r.CreateRecord(record);
	r.SaveRecord(file);
	AddToMap(r, map1);
	r.clear();
      }

    file.close();
    from=tape.tellg();
    cout<<"ID="<<MFstr<<" SIZE "<<map1.size()<<'\n';
    return map1;
  }

  Records(){}
  ~Records(){}
};

#endif
