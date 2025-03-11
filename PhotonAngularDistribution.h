#ifndef PHOTONANGULARDISTRIBUTION_H
#define PHOTONANGULARDISTRIBUTION_H

#include "Tape.h"
#include "math.h"

const double m_e=0.51099895069E+06;
const double r_e=2.8179403227E-15;
const double h=4.135667696e-15;
const double c=299792458;

class PhotonAngularDistribution
{
protected:
  void CreateVector(const double from, const double to, const double delta, vector<double> &v)
  {
    double f=from;

    while(f<to)
      {
	v.emplace_back(f);
	f+=delta;
      }
  }

  void saveFile(const string &file, const vector<double> &v)
  {
    std::ofstream file1;
    file1.open(file);

    for(const auto &w:v)
      {
	file1<<w;
	file1<<'\n';
      }
  }

  void subs(const vector<vector<double>> &m, const string &name)
  {
    unsigned k=0;
    std::ofstream file;
    file.open(name+"centraldifference.txt");

    for(const auto &i: m)
      {
	for(const auto &j: i)
	  {
	    file<<j;

	    if(k<i.size()-1) file<<';';
	    k++;
	  }
	k=0;
	file<<'\n';
      }
  }

  double Gaussian(const double x, const double width)
  {
    return exp(-x*x/(2*width*width))/(width*sqrt(2*M_PI));
  }

  double DiracDelta(const double x, const double width)
  {
    // Dirac delta is approximated as Gaussian function
    return Gaussian(x, width);
  }

  double x(const double E, const double mu)
  {
    return (E/(h*c))*sqrt((1-mu)/2);
  }
  void Trapezoidal(const unsigned N, double xFrom, const double xTo, double yFrom, const double yTo)
  {
    double dsigmadmu;
    double y;
    vector<double> row;
    const double deltaX=(double)(xTo-xFrom)/N;
    const double deltaY=(double)(yTo-yFrom)/N;
    double x=xFrom;
    row.reserve(N);

    while(yFrom<yTo)
      {
	y=GetE(yFrom);

	while(xFrom<xTo)
	  {
	    dsigmadmu=0.5*deltaX*(Getdsigma(yFrom, xFrom-deltaX, y)
				 +Getdsigma(yFrom, xFrom, y));
	    row.emplace_back(dsigmadmu);
	    xFrom+=deltaX;
	  }
	result.emplace_back(row);
	row.clear();
	yFrom+=deltaY;
	xFrom=x;
      }
  }

  void Create(const double xFrom, const double xTo, const double yFrom, const double yTo, const unsigned N, const string &name)
  {
    const double deltaX=(double)(xTo-xFrom)/N;
    const double deltaY=(double)(yTo-yFrom)/N;
    result.reserve(N);
    CreateVector(xFrom, xTo, deltaX, X);
    CreateVector(yFrom, yTo, deltaY, Y);
    saveFile(name+"mu.txt", X);
    saveFile(name+"E.txt", Y);
    Trapezoidal(N, xFrom, xTo, yFrom, yTo);
    subs(result, name);
  }

public:
  vector<vector<double>> result;
  vector<double> X;
  vector<double> Y;

  virtual double Getd2sigma(const double E, const double Eprime, const double mu, const double width)=0;
  virtual double Getdsigma(const double E, const double mu, const double sigma)=0;
  virtual void CreateCumulative(const double E, const unsigned N)=0;
  virtual double GetE(const double E)=0;
  PhotonAngularDistribution(){}
  virtual ~PhotonAngularDistribution(){}
};

class IncoherentAngularDistribution: public PhotonAngularDistribution
{
 private:
  Section *incoherentFunction=nullptr;
  Section *incoherent=nullptr;

  double Eprimev(const double E, const double mu)
  {
    return E/(1+(E/m_e)*(1-mu));
  }

  double GetE(const double E) override
  {
    return incoherent->GetLibraryValue(E, 504);
  }

  double KleinNishinaCrossSection(const double E, const double mu)
  {
    const double Eprime=Eprimev(E, mu);
    const double k=E/m_e;
    const double kprime=Eprime/m_e;
    const double kk=kprime/k;
    const double muprime=1-mu;
    return M_PI*r_e*r_e*kk*kk*(1+mu*mu+k*kprime*muprime*muprime);
  }

  double dsigmadmu(const double E, const double mu)
  {
    return incoherentFunction->GetLibraryValue(x(E, mu), 504)*KleinNishinaCrossSection(E, mu);
  }

  double d2sigmadEdmu(const double E, const double Eprime, const double mu, const double width)
  {
    return dsigmadmu(E, mu)*DiracDelta(Eprime-Eprimev(E, mu), width);
  }

 public:
  double Getd2sigma(const double E, const double Eprime, const double mu, const double width) override
  {
    return d2sigmadEdmu(E, Eprime, mu, width);
  }

  double Getdsigma(const double E, const double mu, const double sigma) override
  {
    return 2*M_PI*dsigmadmu(E, mu)/sigma;
  }

  void CreateCumulative(const double E, const unsigned N) override
  {
  }

  IncoherentAngularDistribution(){}
  IncoherentAngularDistribution(Tape *tape, const double xFrom, const double xTo, const double yFrom, const double yTo, const unsigned N, const string &name)
  {
    incoherentFunction=tape->MF27->incoherentFunction;
    incoherent=tape->MF23->incoherentScattering;
    Create(xFrom, xTo, yFrom, yTo, N, name);
  }
  ~IncoherentAngularDistribution(){}
};

class CoherentAngularDistribution: public PhotonAngularDistribution
{
 private:
  Section *coherentFactor=nullptr;
  Section *imaginaryFactor=nullptr;
  Section *realFactor=nullptr;
  Section *coherent=nullptr;
  double xMax=0;
  double aMax=0;
  map<double, double> cumulative;

  double GetAngle()
  {
    const double a=RNG(0, 1)*aMax;
    double aprev=0;
    double acurrent=0;
    double xprev=0;
    double xcurrent=0;
    // for(const auto& [x1, x2]: cumulative)
    //   {
    // 	cout<<"Show "<<x1<<" "<<x2<<'\n';
    //   }
    // if(a<=cumulative.begin()->second)
    //   {
    // 	cout<<"a<=cumulative.begin()->second"<<'\n';
    // 	double r=xprev+((xcurrent-xprev)*(a-aprev))/(acurrent);
    //   }

    for(const auto& [x, A]: cumulative)
      {
	cout<<"i "<<a<<" "<<A<<" max "<<aMax<<'\n';
	if(A>=a)
	  {
	    acurrent=A;
	    xcurrent=x;
	    break;
	  }
	aprev=A;
	xprev=x;
      }
    double r=xprev+((xcurrent-xprev)*(a-aprev))/(acurrent-aprev);
    cout<<xprev<<" "<<r<<" "<<xcurrent<<'\n';
    cout<<aprev<<" "<<a<<" "<<acurrent<<'\n';
    return r;
  }

  double GetE(const double E) override
  {
    return coherent->GetLibraryValue(E, 502);
  }

  double ThomsonCrossSection(const double mu)
  {
    return M_PI*r_e*r_e*(1+mu*mu);
  }

  double dsigmadmu(const double E, const double mu)
  {
    const double F=coherentFactor->GetLibraryValue(x(E, mu), 502);
    const double Fprime=realFactor->GetLibraryValue(E, 506);
    const double Fprimeprime=imaginaryFactor->GetLibraryValue(E, 505);
    const double FF=F+Fprime;
    return ThomsonCrossSection(mu)*(FF*FF+Fprimeprime*Fprimeprime);
  }

  double d2sigmadEdmu(const double E, const double Eprime, const double mu, const double width)
  {
    return dsigmadmu(E, mu)*DiracDelta(Eprime-E, width);
  }
 public:
  vector<double> cumulativeFormFactor;

  double Getd2sigma(const double E, const double Eprime, const double mu, const double width) override
  {
    return d2sigmadEdmu(E, Eprime, mu, width);
  }

  double Getdsigma(const double E, const double mu, const double sigma) override
  {
    return 2*M_PI*dsigmadmu(E, mu)/sigma;
  }

  void CreateCumulative(const double E, const unsigned N) override
  {
    double x=xMax/N;
    double v1=0;
    double v2=0;
    const double deltaX=1000;

    while(x<xMax)
      {
	v1=coherentFactor->GetLibraryValue(x-deltaX, 502);
	v2=coherentFactor->GetLibraryValue(x, 502);
	cumulative[x]=cumulative[x-deltaX]+0.5*deltaX*(v1*v1+v2*v2);
	x+=deltaX;
      }
    aMax=cumulative[x-deltaX];
    // saveFile("coherent.txt", Amax);
  }

  CoherentAngularDistribution(){}
  CoherentAngularDistribution(Tape *tape, const double xFrom, const double xTo, const double yFrom, const double yTo, const unsigned N, const string &name)
  {
    coherentFactor=tape->MF27->coherentFactor;
    imaginaryFactor=tape->MF27->imaginaryFactor;
    realFactor=tape->MF27->realFactor;
    coherent=tape->MF23->coherentScattering;
    // Create(xFrom, xTo, yFrom, yTo, N, name);
    // cumulativeFormFactor=CreateCumulative(100, N);
    xMax=yTo/(h*c);
    CreateCumulative(yTo, N);
    GetAngle();
    // cout<<mu<<'\n';
    // set Aprime=aA_max where a is RNG
    // find x^2 corresponding Aprime A'=A(x^2, Z)
  }
  ~CoherentAngularDistribution(){}
};

#endif
