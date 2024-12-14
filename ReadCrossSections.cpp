#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "CrossSection.h"

using std::cout;
using std::ifstream;
using std::streampos;
using std::stod;

void print(const vector<string> &v)
{
  string s="";
  for(const auto &i: v){s=s+";"+i;}
  cout<<s<<" SIZE "<<v.size()<<'\n';
  assert(v.size()==5);
}

void print(const vector<double> &v)
{
  string s="";
  for(const auto &i: v){s=s+";"+std::to_string(i);}
  cout<<s<<" SIZE "<<v.size()<<'\n';
  assert(v.size()==5);
}

void LineSplit(string &line, vector<double> &v)
{
  const string sub=line.substr(0, 10);
  const char empty=' ';
  const char s1=sub[0];
  const char s2=sub[sub.size()-1];

  if(line.size()>10)
    {
      line=line.substr(11, line.size());

      if(s1==empty and s2==empty) v.push_back(0);
      else v.push_back(stod(sub));

      if(line.size()>=10) LineSplit(line, v);
    }
  else if(line.size()==10 and s1==empty and s2==empty) v.push_back(0);
  else v.push_back(stod(line));
}

map<double, vector<double>> ReadENDF(ifstream &endf, streampos &from, const string &id)
{
  bool found=false;
  double energy;
  string line;
  string line2;
  int linesSkip=3;
  int size;
  const int idSize=id.size();
  vector<double> v;
  v.reserve(6);
  map<double, vector<double>> ENDFmap;
  endf.seekg(from);

  while(getline(endf, line))
    {
      size=line.size();
      line2=line.substr(size-idSize, size);
      // cout<<line<<'\n';
      if(found and line.substr(size-idSize, size)!=id) break;
      else if(line2==id and linesSkip==0)
	{
	  line=line.substr(1, size-idSize-1);
	  LineSplit(line, v);
	  energy=v[0];
	  v.erase(v.begin()+0);
	  ENDFmap[energy]=v;
	  // print(v);
	  v.clear();
	  found=true;
	}
      else if(line2==id) linesSkip--;
    }
  from=endf.tellg();
  return ENDFmap;
}

void Split(string &line, vector<string> &v)
{
  const int at=distance(line.begin(), find(line.begin(), line.end(), ' '));
  const int size=line.size();

  if(at>0 and at!=size)
    {
      v.push_back(line.substr(0, at));
      line=line.substr(at, line.size());
      Split(line, v);
    }
  else if(at==0)
    {
      string line3=line.substr(at+1, line.size());
      Split(line3, v);
    }
}

vector<string> GetReactions(ifstream &endf, streampos &from)
{
  bool found=0;
  string line;
  string line2;
  const string MFMT="MF/MT";
  const string warning="Warning";
  int at;
  vector<string> v;
  vector<string> result;

  while(getline(endf, line))
    {
      if(line.substr(2, warning.size())==warning) break;
      else if(found and line[1]!='=')
	{
	  line2=line.substr(2, line.size());
	  Split(line2, v);
	  at=distance(v[0].begin(), find(v[0].begin(), v[0].end(), '/'));
	  result.push_back(v.back()+v[0].substr(0, at)+v[0].substr(at+1, v[0].size()));
	  cout<<result.back()<<'\n';
	  v.clear();
	}
      else if(line.substr(2, MFMT.size())==MFMT) found=true;
    }
  from=endf.tellg();
  return result;
}

CrossSections ParseEndf(ifstream &endf, streampos &from)
{
  vector<CrossSection> crossSections;
  const vector<string> reactions=GetReactions(endf, from);

  for(const auto &reaction: reactions)
    {
      cout<<"FIND "<<reaction<<'\n';
      CrossSection crossSection(reaction, ReadENDF(endf, from, reaction));
      crossSections.push_back(crossSection);
    }
  cout<<" "<<'\n';
  from=0;
  return {crossSections};
}

void GetCrossSection()
{
  ifstream steel("./cross-sections/photoat-026_Fe_000.endf");
  ifstream nitrogen("./cross-sections/photoat-007_N_000.endf");
  ifstream sodium("./cross-sections/photoat-011_Na_000.endf");
  ifstream titanium("./cross-sections/photoat-022_Ti_000.endf");
  ifstream iodine("./cross-sections/photoat-053_I_000.endf");

  streampos from=0;
  ParseEndf(steel, from);
  ParseEndf(nitrogen, from);
  ParseEndf(sodium, from);
  ParseEndf(titanium, from);
  ParseEndf(iodine , from);
}
