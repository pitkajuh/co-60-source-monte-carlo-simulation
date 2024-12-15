#include <fstream>
#include <iostream>
#include <algorithm>
#include "CrossSection.h"

using std::cout;
using std::ifstream;
using std::streampos;
using std::stod;
using std::stoi;
using std::to_string;

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

      if(found and line.substr(size-idSize, size)!=id) break;
      else if(line2==id and linesSkip==0)
	{
	  line=line.substr(1, size-idSize-1);
	  LineSplit(line, v);
	  energy=v[0];
	  v.erase(v.begin()+0);
	  ENDFmap[energy]=v;
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

pair<string, vector<pair<unsigned, unsigned>>> GetReactions(ifstream &endf, streampos &from)
{
  int at;
  bool found=0;
  string line;
  string line2;
  string MF;
  const string MFMT="MF/MT";
  const string warning="Warning";
  vector<string> v;
  vector<pair<unsigned, unsigned>> result;
  pair<unsigned, unsigned> MFMTpair;
  pair<string, vector<pair<unsigned, unsigned>>> resultf;

  while(getline(endf, line))
    {
      if(line.substr(2, warning.size())==warning) break;
      else if(found and line[1]!='=')
	{
	  line2=line.substr(2, line.size());
	  Split(line2, v);
	  at=distance(v[0].begin(), find(v[0].begin(), v[0].end(), '/'));

	  MFMTpair={stoi(v[0].substr(0, at)), stoi(v[0].substr(at+1, v[0].size()))};
	  MF=v.back();
	  result.push_back(MFMTpair);
	  // result.push_back(v.back()+v[0].substr(0, at)+v[0].substr(at+1, v[0].size()));
	  // cout<<v[0].substr(0, at)+v[0].substr(at+1, v[0].size())<<"v[0].substr(0, at)+v[0].substr(at+1, v[0].size())"<<'\n';
	  v.clear();
	}
      else if(line.substr(2, MFMT.size())==MFMT) found=true;
    }
  resultf={MF, result};
  from=endf.tellg();
  return resultf;
}

// CrossSection *GetCrossSection(const string &reaction, const map<double, vector<double>> &map)
// {
//   // const unsigned MF=std::stoi(reaction.substr(4, 2));
//   const unsigned MT=std::stoi(reaction.substr(6, 8));

//   if(MT==501)
//     {
//       return new TotalCrossSection;
//     }
//   // else if(MT==502)
//   //   {

//   //   }
//   // else if(MT==504)
//   //   {

//   //   }
//   return nullptr;
// }

CrossSections ParseEndf(ifstream &endf, streampos &from)
{
  vector<CrossSection> crossSections23={};
  vector<CrossSection> crossSections27={};
  const pair<string, vector<pair<unsigned, unsigned>>> reactions=GetReactions(endf, from);
  const string material=reactions.first;

  for(const auto &reaction: reactions.second)
    {
      // cout<<reaction<<'\n';
      // MF=reaction.substr(4, 2);
      CrossSection crossSection(reaction, ReadENDF(endf, from, material+to_string(reaction.first)+to_string(reaction.second)));
      // cout<<"MF;"<<std::stoi(reaction.substr(4, 2))<<";"<<'\n';
      // cout<<"AEAOE "<<material+to_string(reaction.first)+to_string(reaction.second)<<'\n';
      // crossSections23.push_back(crossSection);
      if(reaction.first==23)
	{
	  // cout<<23<<'\n';
	  crossSections23.push_back(crossSection);
	}
      else
	{
	  // cout<<27<<'\n';
	  crossSections27.push_back(crossSection);
	}
    }
  from=0;
  // cout<<"END"<<'\n';
  return {crossSections23, crossSections27};
}

CrossSections GetMaterialCrossSection(const string &endf)
{
  ifstream endfStream(endf);
  streampos from=0;
  return ParseEndf(endfStream, from);
}
