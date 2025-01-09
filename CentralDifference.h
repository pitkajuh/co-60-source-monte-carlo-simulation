#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "PhotonAngularDistribution.h"
#include "Matrix.h"
#include "GaussSeidel.h"
#include "LU.h"

struct CentralDifference
{
private:
  void subs(Matrix m)
  {
    std::ofstream file;
    file.open("centraldifference.txt");
    unsigned k=0;
    for(const auto &i: m.matrix)
      {
	for(const auto &j: i)
	  {
	    // cout<<j<<'\n';
	    // file<<j*r[k];
	    file<<j;

	    if(k<i.size()-1) file<<';';
	    k++;
	  }
	k=0;
	file<<'\n';
      }
  }
  void saveFile(double from, const double to, const double delta, const string &file)
  {
    std::ofstream file1;
    file1.open(file);

    while(from<=to)
      {
	file1<<from;
	file1<<'\n';
	from+=delta;
      }
  }
  void write()
  {
    for(const auto &i: discretized.matrix)
      {
	for(const auto &j: i)
	  {
	    cout<<j<<'\n';
	  }
      }
  }
  PhotonAngularDistribution *distribution=nullptr;

  void cd()
  {
    unsigned i=1;
    unsigned j=1;
    double mu=-1;
    Records &crossSection=distribution->tape->MF23->incoherentScattering->recordsAll[0].r;
    const double Emax=2.00000E+6;
    const unsigned size=100;
    const double deltaMu=(double) 2.0/size;
    double E=crossSection.energy[0];
    double Eprime=E/(1+(E/m_e)*(1-mu));
    vector<double> result;
    result.reserve(size);
    double deltaEnergy=(double)Emax/size;
    const double width=deltaEnergy/2;
    double d2sigmadmudE;
    // gridmu.init(size);
    discretized.init(size);
    vector<double> row;
    row.reserve(size);
    // discretized.matrix.reserve(size);

    saveFile(-1+deltaMu, 1, deltaMu, "mu.txt");
    saveFile(1+deltaEnergy, Emax, deltaEnergy, "E.txt");

    cout<<"Central difference "<<"E "<<E<<" Eprime "<<Eprime<<" delta "<<deltaEnergy<<" "<<size<<'\n';


    while(j<size)
      {
	while(i<size)
	  {
	    Eprime=E/(1+(E/m_e)*(1-mu));
	    d2sigmadmudE=(distribution->Getd2sigma(E, Eprime+deltaEnergy, mu+deltaMu, width)
			  -distribution->Getd2sigma(E, Eprime+deltaEnergy, mu-deltaMu, width)
			  -distribution->Getd2sigma(E, Eprime-deltaEnergy, mu+deltaMu, width)
			  +distribution->Getd2sigma(E, Eprime-deltaEnergy, mu-deltaMu, width)
			  )/(4*deltaEnergy*deltaMu);
	    row.emplace_back(d2sigmadmudE);
	    i++;
	    E+=deltaEnergy;
	  }

	discretized.emplace_back(row);
	row.clear();
	i=1;
	E=crossSection.energy[0];
	mu+=deltaMu;
	j++;
      }
  }
public:
  vector<double> gridE;
  vector<double> E1;
  Matrix gridmu;
  Matrix discretized;

  CentralDifference(){}
  CentralDifference(PhotonAngularDistribution *d)
  {
    this->distribution=d;
    cd();
    // GaussSeidel gs(discretized, gridE, E1);
    // write();
    LU lu(discretized, gridE, E1);
  }
  ~CentralDifference(){}
};

#endif
