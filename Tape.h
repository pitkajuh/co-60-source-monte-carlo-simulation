#ifndef TAPE_H
#define TAPE_H

#include <fstream>
#include <algorithm>
#include "File.h"

class Tape
{
 private:

 public:
  File *MF23=new File23;
  File *MF27=new File27;

  Tape(){}
  /* Tape(ifstream &tape) */
  /*   { */
  /*     streampos from=0; */
  /*   } */
  ~Tape()
    {
      delete MF23;
      delete MF27;
    }
};


#endif
