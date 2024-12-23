#ifndef TAPE_H
#define TAPE_H

#include "File.h"

class Tape
{
 protected:
  vector<string> GetMF(ifstream &tape, streampos &from, const unsigned MT)
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

	if(found and MTstr!=std::to_string(MT)) break;
	else if(MTstr==std::to_string(MT) and MF!="501" and MF!="522" and MF!="516")
	  {
	    record=record.substr(2, record.size());
	    cout<<record<<" "<<MF<<'\n';
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
      const vector<string> MF23=GetMF(endf, from, 23);
      cout<<"GET 27 "<<from<<'\n';
      const vector<string> MF27=GetMF(endf, from, 27);
      /* MF23=new File23(endf, from); */
      /* cout<<"start MF27 "<<from<<'\n'; */

      /* MF27=new File27(endf, from); */
    }
  ~Tape()
    {
      delete MF23;
      delete MF27;
    }
};


#endif
