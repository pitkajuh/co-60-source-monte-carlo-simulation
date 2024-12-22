#ifndef TAPE_H
#define TAPE_H

#include "File.h"

class Tape
{
 public:
  File *MF23;
  File *MF27=nullptr;

  Tape(){}
  Tape(const string &tape)
    {
      ifstream endf(tape);
      streampos from=0;
      MF23=new File23(endf, from);
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
