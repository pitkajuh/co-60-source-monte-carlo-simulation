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

void print(const vector<string> &v)
{
  string s="";
  for(const auto &i: v){s=s+";"+i;}
  cout<<s<<" SIZE "<<v.size()<<'\n';
}

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

void Split(string &line, vector<string> &v)
{
  const int at=distance(line.begin(), find(line.begin(), line.end(), ' '));
  const int size=line.size();

  if(at>0 and at!=size)
    {
      string s2=line.substr(0, at);
      v.push_back(s2);
      line=line.substr(at, line.size());
      Split(line, v);
    }
  else if(at==0)
    {
      string line3=line.substr(at+1, line.size());
      Split(line3, v);
    }
}

vector<string> GetReactions(ifstream &endf, streampos *&from)
{
  bool found=false;
  string line;
  string line2;
  string line3;
  string MF;
  string MT;
  const string MFMT="MF/MT";
  const string warning="Warning";
  int linesSkip=1;
  int at;
  vector<string> v;
  vector<string> result;

  while(getline(endf, line))
    {
      line2=line.substr(2, MFMT.size());

      if(line.substr(2, warning.size())==warning) break;
      else if(found and linesSkip==0 and line[1]!='=')
	{
	  line3=line.substr(2, line.size());
	  Split(line3, v);
	  at=distance(v[0].begin(), find(v[0].begin(), v[0].end(), '/'));
	  MF=v[0].substr(0, at);
	  MT=v[0].substr(at+1, v[0].size());
	  result.push_back(v.back()+v[0].substr(0, at)+v[0].substr(at+1, v[0].size()));
	  // get MF and MT
	  cout<<result.back()<<'\n';
	  v.clear();
	}
      else if(found and linesSkip>0 and line[1]!='=') linesSkip--;
      else if(line2==MFMT) found=true;
    }
  return result;
}

void GetCrossSection()
{
  ifstream steel("./cross-sections/photoat-026_Fe_000.endf");
  streampos *begin=new streampos;
  *begin=0;
  const vector<string> reactions=GetReactions(steel, begin);

  for(const auto &reaction: reactions)
    {
      cout<<"FIND "<<reaction<<'\n';
      Read(steel, begin, reaction);
      cout<<" "<<'\n';
    }

  delete begin;
}
