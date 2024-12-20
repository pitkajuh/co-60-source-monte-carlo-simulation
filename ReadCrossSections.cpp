#include <fstream>
#include <iostream>
#include <algorithm>
#include "CrossSection.h"
#include "Tape.h"

using std::cout;
using std::ifstream;
using std::streampos;
using std::stod;
using std::stoi;
using std::to_string;

void SplitRecord(string &record, vector<double> &v)
{
  const string sub=record.substr(0, 10);
  const char empty=' ';
  const char s1=sub[0];
  const char s2=sub[sub.size()-1];

  if(record.size()>10)
    {
      record=record.substr(11, record.size());

      if(s1==empty and s2==empty) v.push_back(0);
      else v.push_back(stod(sub));

      if(record.size()>=10) SplitRecord(record, v);
    }
  else if(record.size()==10 and s1==empty and s2==empty) v.push_back(0);
  else v.push_back(stod(record));
}

map<double, vector<double>> ReadENDF(ifstream &endf, streampos &from, const string &id)
{
  bool found=false;
  double energy;
  string record;
  string record2;
  int recordsSkip=3;
  int size;
  const int idSize=id.size();
  vector<double> v;
  v.reserve(6);
  map<double, vector<double>> ENDFmap;
  endf.seekg(from);

  while(getline(endf, record))
    {
      size=record.size();
      record2=record.substr(size-idSize, size);

      if(found and record.substr(size-idSize, size)!=id) break;
      else if(record2==id and recordsSkip==0)
	{
	  record=record.substr(1, size-idSize-1);
	  SplitRecord(record, v);
	  energy=v[0];
	  v.erase(v.begin()+0);
	  ENDFmap[energy]=v;
	  v.clear();
	  found=true;
	}
      else if(record2==id) recordsSkip--;
    }
  from=endf.tellg();
  return ENDFmap;
}

void Split(string &record, vector<string> &v)
{
  const int at=distance(record.begin(), find(record.begin(), record.end(), ' '));
  const int size=record.size();

  if(at>0 and at!=size)
    {
      v.push_back(record.substr(0, at));
      record=record.substr(at, record.size());
      Split(record, v);
    }
  else if(at==0)
    {
      string record3=record.substr(at+1, record.size());
      Split(record3, v);
    }
}

pair<string, vector<pair<unsigned, unsigned>>> GetReactions(ifstream &endf, streampos &from)
{
  int at;
  bool found=0;
  string record;
  string record2;
  string MF;
  const string MFMT="MF/MT";
  const string warning="Warning";
  vector<string> v;
  vector<pair<unsigned, unsigned>> result;
  pair<unsigned, unsigned> MFMTpair;
  pair<string, vector<pair<unsigned, unsigned>>> resultf;

  while(getline(endf, record))
    {
      if(record.substr(2, warning.size())==warning) break;
      else if(found and record[1]!='=')
	{
	  record2=record.substr(2, record.size());
	  Split(record2, v);
	  at=distance(v[0].begin(), find(v[0].begin(), v[0].end(), '/'));

	  MFMTpair={stoi(v[0].substr(0, at)), stoi(v[0].substr(at+1, v[0].size()))};
	  MF=v.back();
	  result.push_back(MFMTpair);
	  // result.push_back(v.back()+v[0].substr(0, at)+v[0].substr(at+1, v[0].size()));
	  // cout<<v[0].substr(0, at)+v[0].substr(at+1, v[0].size())<<"v[0].substr(0, at)+v[0].substr(at+1, v[0].size())"<<'\n';
	  v.clear();
	}
      else if(record.substr(2, MFMT.size())==MFMT) found=true;
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
  Tape *ENDFtape=new Tape;
  vector<CrossSection> file23={};
  vector<CrossSection> file27={};
  const pair<string, vector<pair<unsigned, unsigned>>> reactions=GetReactions(endf, from);
  const string material=reactions.first;

  for(const auto &reaction: reactions.second)
    {
      CrossSection crossSection(reaction, ReadENDF(endf, from, material+to_string(reaction.first)+to_string(reaction.second)));

      if(reaction.first==23 and reaction.second!=516 and reaction.second!=522)
	{
	  // cout<<23<<'\n';
	  file23.push_back(crossSection);
	}
      // else if(reaction.first!=516 and reaction.first!=522 and reaction.first!=515)
      else
	{
	  // cout<<27<<'\n';
	  file27.push_back(crossSection);
	}
    }
  from=0;
  // cout<<"END"<<'\n';
  delete ENDFtape;
  endf.close();
  return {file23, file27};
}

CrossSections GetMaterialCrossSection(const string &endf)
{
  ifstream endfStream(endf);
  streampos from=0;
  return ParseEndf(endfStream, from);
}
