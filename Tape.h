#ifndef TAPE_H
#define TAPE_H

#include "File.h"

class Tape
{
 protected:
  void print(vector<string> v)
  {
    string s="";
    for(const auto &a: v)
      {
	s+=a;
	s+=";";
      }
    cout<<s<<'\n';
  }

  vector<string> GetMF(ifstream &tape, streampos &from, const string &MT)
  {
    bool found=0;
    string record;
    string MF;
    string MTstr;
    vector<string> v;

    while(getline(tape, record))
      {
	MF=record.substr(5, 3);
	MTstr=record.substr(2, 2);

	if(found and MTstr!=MT) break;
	else if(MTstr==MT and MF!="501" and MF!="522" and MF!="516")
	  {
	    record=record.substr(2, record.size());
	    v.emplace_back(MF);
	    found=1;
	  }
      }
    from=tape.tellg();
    return v;
  }
 public:
  File *MF23=nullptr;
  File *MF27=nullptr;

  Tape(){}
  Tape(const string &tape)
    {
      ifstream endf(tape);
      streampos from=76;
      const string MT23="23";
      const string MT27="27";
      const vector<string> MF23v=GetMF(endf, from, MT23);
      const vector<string> MF27v=GetMF(endf, from, MT27);
      MF23=new File23(endf, from, MF23v, MT23);
      MF27=new File27(endf, from, MF27v, MT27);
    }
  ~Tape()
    {
      delete MF23;
      delete MF27;
    }
};


#endif
