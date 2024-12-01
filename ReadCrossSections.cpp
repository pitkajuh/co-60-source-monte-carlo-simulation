#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::vector;
using std::string;
using std::cout;
using std::ifstream;
using std::streampos;
using std::map;

string LineSplit(string line)
{
  const string f="260023501";
  const int at=distance(line.begin(), find(line.begin(), line.end(), ' '));
  cout<<at<<'\n';
  return f;
}

void ReadENDF(ifstream &endf, streampos *from)
{
  string line;
  bool stop=false;
  const string f="260023501";

  while(getline(endf, line) and !stop)
    {
      // cout<<line<<'\n';
      LineSplit(line);
    }

}

void Read(ifstream &endf, streampos *from=nullptr)
{
  map<double, double> map;

  if(from==nullptr)
    {
      from=new streampos;
      *from=0;
      // Map=GetBin(bin, from);
      ReadENDF(endf, from);
      delete from;
    }
  else       ReadENDF(endf, from);
  // return Map;
}

void GetCrossSection()
{
  ifstream steel("./cross-sections/photoat-026_Fe_000.endf");
  streampos *begin=new streampos;
  *begin=0;
  Read(steel, begin);
  delete begin;
}
