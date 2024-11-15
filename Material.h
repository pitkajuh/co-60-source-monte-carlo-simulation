#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>

using std::map;
using std::pair;

class Material
{
public:
  double density=0; // in g/cm3
  virtual const double GetMu (const double photonEnergy)=0;
  virtual ~Material (){}
};

class Steel: public Material
{
public:
  Steel (const double d){density=d;}
  // Taken from https://physics.nist.gov/PhysRefData/XrayMassCoef/ElemTab/z26.html
  // Energy (MeV), μ/ρ (cm2/g), μ_en/ρ (cm2/g)
  map<double, pair<double, double>> muMap=
    {
      {1.00000E-03, pair<double, double>(9.085E+03, 9.052E+03)},
      {1.50000E-03, pair<double, double>(3.399E+03, 3.388E+03)},
      {2.00000E-03, pair<double, double>(1.626E+03, 1.620E+03)},
      {3.00000E-03, pair<double, double>(5.576E+02, 5.535E+02)},
      {4.00000E-03, pair<double, double>(2.567E+02, 2.536E+02)},
      {5.00000E-03, pair<double, double>(1.398E+02, 1.372E+02)},
      {6.00000E-03, pair<double, double>(8.484E+01, 8.265E+01)},
      {7.11200E-03, pair<double, double>(5.319E+01, 5.133E+01)}, // is this an error?
      {7.11200E-03, pair<double, double>(4.076E+02, 2.978E+02)}, // is this an error?
      {8.00000E-03, pair<double, double>(3.056E+02, 2.316E+02)},
      {1.00000E-02, pair<double, double>(1.706E+02, 1.369E+02)},
      {1.50000E-02, pair<double, double>(5.708E+01, 4.896E+01)},
      {2.00000E-02, pair<double, double>(2.568E+01, 2.260E+01)},
      {3.00000E-02, pair<double, double>(8.176E+00, 7.251E+00)},
      {4.00000E-02, pair<double, double>(3.629E+00, 3.155E+00)},
      {5.00000E-02, pair<double, double>(1.958E+00, 1.638E+00)},
      {6.00000E-02, pair<double, double>(1.205E+00, 9.555E-01)},
      {8.00000E-02, pair<double, double>(5.952E-01, 4.104E-01)},
      {1.00000E-01, pair<double, double>(3.717E-01, 2.177E-01)},
      {1.50000E-01, pair<double, double>(1.964E-01, 7.961E-02)},
      {2.00000E-01, pair<double, double>(1.460E-01, 4.825E-02)},
      {3.00000E-01, pair<double, double>(1.099E-01, 3.361E-02)},
      {4.00000E-01, pair<double, double>(9.400E-02, 3.039E-02)},
      {5.00000E-01, pair<double, double>(8.414E-02, 2.914E-02)},
      {6.00000E-01, pair<double, double>(7.704E-02, 2.836E-02)},
      {8.00000E-01, pair<double, double>(6.699E-02, 2.714E-02)},
      {1.00000E+00, pair<double, double>(5.995E-02, 2.603E-02)},
      {1.25000E+00, pair<double, double>(5.350E-02, 2.472E-02)},
      {1.50000E+00, pair<double, double>(4.883E-02, 2.360E-02)},
      {2.00000E+00, pair<double, double>(4.265E-02, 2.199E-02)},
      {3.00000E+00, pair<double, double>(3.621E-02, 2.042E-02)},
      {4.00000E+00, pair<double, double>(3.312E-02, 1.990E-02)},
      {5.00000E+00, pair<double, double>(3.146E-02, 1.983E-02)},
      {6.00000E+00, pair<double, double>(3.057E-02, 1.997E-02)},
      {8.00000E+00, pair<double, double>(2.991E-02, 2.050E-02)},
      {1.00000E+01, pair<double, double>(2.994E-02, 2.108E-02)},
      {1.50000E+01, pair<double, double>(3.092E-02, 2.221E-02)},
      {2.00000E+01, pair<double, double>(3.224E-02, 2.292E-02)},
    };
  const double GetMu (const double photonEnergy)
  {
    double energyPrevious=0;
    double energyCurrent=0;

    for (const auto& [energy, mu]: muMap)
      {
	if (energy>=photonEnergy)
	  {
	    energyCurrent=energy;
	    break;
	  }
	energyPrevious=energy;
      }
    // Use linear interpolation to get the mass attenuation value corresponding to photonEnergy and turn it to linear attenuation coefficient.
    return density*(muMap[energyPrevious].first+(photonEnergy-energyPrevious)* (muMap[energyCurrent].first-muMap[energyPrevious].first)/(energyCurrent-energyPrevious));
  }
};

#endif
