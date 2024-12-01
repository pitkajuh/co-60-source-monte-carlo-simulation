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
using std::stod;

void print(const vector<double> &v)
{
  string s="";
  for(const auto &i: v){s=s+";"+std::to_string(i);}
  cout<<s<<" SIZE "<<v.size()<<'\n';
}

void FillVector(vector<double> &v){while(v.size()<6) v.push_back(0);}

void LineSplit(string line, vector<double> &v)
{
  const int at=distance(line.begin(), find(line.begin(), line.end(), ' '));
  const string lineSplit=line.substr(0, at);
  const string line2=line.substr(at+1, line.size());
  const int size=line2.size();

  if(at==0) FillVector(v);
  else v.push_back(stod(lineSplit));

  if(at!=size) LineSplit(line2, v);
  else if(size>0) v.push_back(stod(line2));
}

void ReadENDF(ifstream &endf, streampos *&from, const string &id)
{
  string line;
  string line2;
  bool found=false;
  vector<double> v;
  int linesSkip=3;
  int size;
  const int idSize=id.size();

  while(getline(endf, line))
    {
      size=line.size();
      line2=line.substr(size-idSize, size);

      if(found and line.substr(size-idSize, size)!=id) break;
      else if(line2==id and linesSkip==0)
	{
	  line=line.substr(1, size-idSize-1);
	  LineSplit(line, v);
	  // Add vector to map
	  print(v);
	  v.clear();
	  found=true;
	}
      else if (line2==id) linesSkip--;
      else cout<<"END"<<'\n';
    }
}

void Read(ifstream &endf, streampos *&from, const string &id)
{
  map<double, double> map;

  if(from==nullptr)
    {
      from=new streampos;
      *from=0;
      ReadENDF(endf, from, id);
      delete from;
    }
  else
    {
      ReadENDF(endf, from, id);
    }
  // return Map;
}

vector<string> GetMFMT(const string &element, const vector<string> &reactions, const string &MF)
{
  vector<string> MFMT;

  for(const auto &reaction: reactions)
    {
      MFMT.push_back(element+MF+reaction);
      // cout<<element+MF+reaction<<'\n';
    }
  return MFMT;
}

void GetCrossSection()
{
  const vector<string> reactions={"501", "502", "504", "515", "516", "517", "522", "534", "535", "536", "537", "538", "539", "540", "541", "542", "543"};
  const string MF="23";
  const string element="2600";
  const vector<string> reactions2=GetMFMT(element, reactions, MF);
  ifstream steel("./cross-sections/photoat-026_Fe_000.endf");
  streampos *begin=new streampos;
  *begin=0;
  cout<<*begin<<'\n';
  for(const auto &reaction: reactions2)
    {
      cout<<"FIND "<<reaction<<'\n';
      Read(steel, begin, reaction);
      cout<<" "<<'\n';
    }

  cout<<*begin<<'\n';
  delete begin;
}