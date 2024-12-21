#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include <iostream>
#include <fstream>
using std::cout;
using std::vector;
using std::ifstream;
using std::streampos;

class Record
{
public:
  // vector<double> records;
  // records.reserve(6);
  // Record(){}
  Record(ifstream &tape, streampos &from)
  {

  }
  ~Record(){}
};

#endif
