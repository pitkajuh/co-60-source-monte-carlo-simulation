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
#include "ReadCrossSections.h"

using std::string;
using std::cout;
using std::map;

void ParseCells(Cell *cellHead, const Coordinate &p)
{
  Cell *current=cellHead;
  bool b=0;
  // Coordinate photonInitialPosition=cellHead->GetInitialPosition();

  while(current!=nullptr and !b)
    {
      b=current->CellTest(p);
      current=current->next;
    }
}

void SurfaceTracking(Coordinate &photonFrom, Coordinate &photonTo, Coordinate &photonDirection, Cell *cellHead, RadioNuclide *radioNuclide)
{
  Cell *current=cellHead;
  bool collision=0;
  double collisionDistance=0;
  double pathLength=0;
  photonFrom.print();
  photonTo.print();

  while(current!=nullptr and !collision)
    {
      collision=current->CellTest(photonFrom+photonTo);
      collisionDistance=current->CellDistanceTest(photonFrom+photonTo, photonDirection);
      pathLength=DistanceBetween(photonFrom, photonTo);

      std::cout<<current->name<<" "<<collision<<" distance to surf "<<collisionDistance<<" D="<<pathLength<<'\n';

      current=current->next;
    }
  cout<<" "<<'\n';
}

void ParseCells2(Cell *cellHead, RadioNuclide *radioNuclide, const unsigned time)
{
  const unsigned activityRandom=PoissonRNG(radioNuclide->GetActivity(), time);
  Coordinate photonFrom;
  Coordinate photonTo;
  Coordinate direction;

  for(unsigned i=0; i<activityRandom; i++)
    {
      direction=RandomEmissionDirection();
      photonFrom=cellHead->GetInitialPosition();
      photonTo=direction*(-1/cellHead->material->GetMu(radioNuclide->PDF()))*log(RNG(0, 1));
      SurfaceTracking(photonFrom, photonTo, direction, cellHead, radioNuclide);
    }
}

void GetCount(Cell *cellHead, const unsigned N)
{
  Cell *current=cellHead;

  while(current!=nullptr)
    {
      cout<<current->name<<" "<<current->hits<<'\n';
      current=current->next;
    }
}

void MonteCarlo(const unsigned time, Cell *cell, RadioNuclide *radioNuclide)
{
  unsigned t=0;

  while(t<time)
    {
      // p=GenerateRandom(xMin, xMax, yMin, yMax, zMin, zMax);
      // ParseCells(cell, p);
      ParseCells2(cell, radioNuclide, time);
      t++;
    }
  printf("---------------------------------------\n");

  GetCount(cell, time);
}

int main()
{
  const unsigned time=1;
  const unsigned sourceActivity=100;

  Material *steel1=new Steel(7.874);
  Material *steel2=new Steel(7.874);
  Material *steel3=new Steel(7.874);

  Coordinate centeredAt(0, 0, 0);
  const double v1=0.1;
  const double v2=0.2;
  const double v3=1;
  Cell *source=new CellCylinderTruncatedZ("Source", v1, v1, -v1, steel1, centeredAt);
  Cell *cladding=new CellCylinderTruncatedZ("Cladding", v2, v2, -v2, steel2, centeredAt);
  Cell *outside=new CellBox3D("Outside world", -v3, v3, v3, -v3, v3, -v3, steel3);

  source->next=cladding;
  cladding->next=outside;

  RadioNuclide *co60=new Co60(sourceActivity);
  GetCrossSection();
  MonteCarlo(time, source, co60);

  delete source;
  delete co60;

  return 0;
}
