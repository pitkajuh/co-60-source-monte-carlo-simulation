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
    const string sub=record.substr(0, 10);
    const char empty=' ';
    const char s1=sub[0];
    const char s2=sub[9];
    // cout<<record<<'\n';
    // if(v.size()>0) cout<<v[v.size()-1]<<" SIZE "<<record.size()<<'\n';

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
	// cout<<"RECORD;"<<record<<" "<<record.size()<<'\n';
	cout<<record<<'\n';
	  record=record.substr(0, 66);
	  r.CreateRecord(record, records);
	// cout<<"AAO "<<records.size()<<'\n';
	// cout<<" "<<'\n';


	records.clear();

      }
    from=tape.tellg();
    cout<<" "<<'\n';
  }

  Records(){}
  ~Records(){}
};

#endif
