#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include <iostream>
#include <fstream>

using std::cout;
using std::vector;
using std::ifstream;
using std::streampos;

struct Record
{
public:
  void CreateRecord(string &record, vector<double> &v)
  {
    const string sub=record.substr(0, 11);
    const char empty=' ';
    const char s1=sub[0];
    const char s2=sub[9];

    if(record.size()>10)
      {
	record=record.substr(11, record.size());

	if(s1==empty and s2==empty) v.emplace_back(0);
	else v.push_back(stod(sub));

	if(record.size()>=10) CreateRecord(record, v);
      }
    else if(record.size()==10 and s1==empty and s2==empty) v.emplace_back(0);
    else v.emplace_back(stod(record));
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
  vector<double> records;
  void GetRecord(ifstream &tape, streampos &from, unsigned MF)
  {
    records.reserve(6);
    from+=76*2;
    string record;
    string MFstr=std::to_string(MF);
    Record r;
    tape.seekg(from);

    while(getline(tape, record))
      {
	if(record.substr(record.size()-MFstr.size(), record.size())!=MFstr) break;
	record=record.substr(0, 66);
	r.CreateRecord(record, records);
	printdouble(records);

	records.clear();

      }
    from=tape.tellg();
    cout<<" "<<'\n';
  }

  Records(){}
  ~Records(){}
};

#endif
