#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <utility>
#include "math.h"
#include "Material.h"
#include "Cell.h"
#include "Coordinate.h"
#include "RadioNuclide.h"

using std::string;
using std::cout;
using std::map;

void ParseCells (Cell *cellHead, const Coordinate &p)
{
  Cell *current=cellHead;
  bool b=0;
  // Coordinate photonInitialPosition=cellHead->GetInitialPosition ();

  while (current!=nullptr and !b)
    {
      b=current->CellTest (p);
      current=current->next;
    }
}

void SurfaceTracking(Coordinate &photonInitialPosition, Cell *cellHead, RadioNuclide *radioNuclide)
{
  Coordinate direction=RandomEmissionDirection();
  Coordinate positionNew=photonInitialPosition+direction*(-1/cellHead->material->GetMu (radioNuclide->PDF()))*log (RNG (0, 1));
  Cell *current=cellHead;
  bool collision=0;
  double distance=0;
  // direction=RandomEmissionDirection();
  // positionNew=photonInitialPosition;
  // positionNew=photonInitialPosition+direction*(-1/source->material->GetMu (Co60PDF()))*log (RNG (0, 1));
  positionNew.print();

  while (current!=nullptr and !collision)
    {
      collision=current->CellTest (positionNew);
      distance=current->CellDistanceTest (positionNew, direction);
      std::cout<<current->name<<" "<<collision<<" "<<distance<<'\n';
      current=current->next;
    }
  cout<<" "<<'\n';
}

void ParseCells2 (Cell *cellHead, RadioNuclide *radioNuclide)
{
  const unsigned activityRandom=PoissonRNG (radioNuclide->GetActivity(), 1);
  Coordinate photonInitialPosition;

  for (unsigned i=0; i<activityRandom; i++)
    {
      photonInitialPosition=cellHead->GetInitialPosition ();
      // photonInitialPosition.print();
      SurfaceTracking(photonInitialPosition, cellHead, radioNuclide);
    }
}

void GetCount (Cell *cellHead, const double volume, const unsigned N)
{
  Cell *current=cellHead;
  double total=0;
  double vol=0;

  while (current!=nullptr)
    {
      vol=(double) current->hits*volume/N;
      // cout<<current->name<<" "<<vol<<" "<<current->hits<<'\n';
      cout<<current->name<<" "<<current->hits<<'\n';
      total+=vol;
      current=current->next;
    }
  cout<<"Total volume "<<total<<", "<<volume<<"\n";
}

void MonteCarlo (const unsigned N, const double xMin, const double xMax, const double yMin, const double yMax, const double zMin, const double zMax, Cell *cell, RadioNuclide *radioNuclide)
{
  unsigned n=0;
  const double volume=(xMax-xMin)*(yMax-yMin)*(zMax-zMin);

  while (n<N)
    {
      // p=GenerateRandom (xMin, xMax, yMin, yMax, zMin, zMax);
      // ParseCells (cell, p);
      ParseCells2 (cell, radioNuclide);
      n++;
    }
  printf ("---------------------------------------\n");

  GetCount (cell, volume, N);
}

int main ()
{
  const unsigned N=10;
  const double xMin=-0.665;
  const double xMax=0.665;
  const double yMin=-0.665;
  const double yMax=0.665;
  const double zMin=-0.665;
  const double zMax=0.665;
  const unsigned sourceActivity=100;

  Material *steel1=new Steel (7.874);
  Material *steel2=new Steel (7.874);
  Material *steel3=new Steel (7.874);

  Coordinate centeredAt(0, 0, 0);

  Cell *source=new CellCylinderTruncatedZ ("Source", 2e-2, 2e-2, -2e-2, steel1, centeredAt);
  Cell *cladding=new CellCylinderTruncatedZ ("Cladding", 6e-2, 6e-2, -6e-2, steel2, centeredAt);
  Cell *outside=new CellBox3D ("Outside world", -1, 1, 1, -1, 1, -1, steel3);

  source->next=cladding;
  cladding->next=outside;

  RadioNuclide *co60=new Co60(sourceActivity);

  MonteCarlo (N, xMin, xMax, yMin, yMax, zMin, zMax, source, co60);
  delete source;
  delete co60;
  return 0;
}
