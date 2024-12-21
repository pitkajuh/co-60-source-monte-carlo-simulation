#ifndef TAPE_H
#define TAPE_H

#include <algorithm>
#include "File.h"

class Tape
{
 private:
  /* void ReadTape(ifstream &tape, streampos &from) */
  /* { */
  /*   cout<<"ReadTape"<<'\n'; */
  /*   string record; */
  /*   tape.seekg(from); */

  /*   while(getline(tape, record)) */
  /*     { */
  /* 	cout<<record<<'\n'; */
  /*     } */
  /*   tape.close(); */
  /* } */
 public:
   /* File *MF23=new File23; */
  File *MF23;
  File *MF27=new File27;

  Tape(){}
  Tape(const string &tape)
    {
      ifstream endf(tape);
      /* streampos from=76; */
      streampos from=0;
      MF23=new File23(endf, from);
      /* ReadTape(endf, from); */
    }
  ~Tape()
    {
      delete MF23;
      delete MF27;
    }
};


#endif
