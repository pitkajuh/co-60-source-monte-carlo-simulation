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

double Co60PDF ()
{
  // Probability distribution function of Co-60 gamma energies (MeV).
  if (RNG (0, 1)<=0.85) return 1.173;
  return 1.332;
}

void SurfaceTracking(Coordinate &photonInitialPosition, Cell *cellHead)
{
  Coordinate direction;
  Coordinate positionNew;
  Cell *current=cellHead;
  bool collision=0;
  double distance=0;
  // bool direction=0;

  while (current!=nullptr and !collision)
    {
      direction=RandomEmissionDirection();

      // photonInitialPosition.print();
      // positionNew=photonInitialPosition+direction*(-1/current->material->GetMu (Co60PDF()))*log (RNG (0, 1));

      // positionNew=direction*(-1/current->material->GetMu (Co60PDF()))*log (RNG (0, 1));
      // positionNew.print();

      positionNew=photonInitialPosition+direction*(-1/current->material->GetMu (Co60PDF()))*log (RNG (0, 1));
      // positionNew.print();

      collision=current->CellTest (positionNew);

      // direction.print();
      distance=current->CellDistanceTest (positionNew, direction);
      // collision=current->CellTest (photonInitialPosition);
      // std::cout<<current->name<<" "<<collision<<" "<<distance<<'\n';
      current=current->next;
    }
  // cout<<" "<<'\n';
}

void ParseCells2 (Cell *cellHead, const unsigned sourceActivity)
{
  const unsigned activityRandom=PoissonRNG (sourceActivity, 1);
  Coordinate photonInitialPosition;

  for (unsigned i=0; i<activityRandom; i++)
    {
      photonInitialPosition=cellHead->GetInitialPosition (Co60PDF ());
      // photonInitialPosition.print();
      SurfaceTracking(photonInitialPosition, cellHead);
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

void MonteCarlo (const unsigned N, const double xMin, const double xMax, const double yMin, const double yMax, const double zMin, const double zMax, Cell *cell, const unsigned sourceActivity)
{
  unsigned n=0;
  // Coordinate p;
  const double volume=(xMax-xMin)*(yMax-yMin)*(zMax-zMin);

  while (n<N)
    {
      // p=GenerateRandom (xMin, xMax, yMin, yMax, zMin, zMax);
      // ParseCells (cell, p);
      ParseCells2 (cell, sourceActivity);
      n++;
    }
  printf ("---------------------------------------\n");

  GetCount (cell, volume, N);
}

int main ()
{
  const unsigned N=100;
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
  Material *steel4=new Steel (7.874);

  Coordinate centeredAt;
  centeredAt.Set(0, 0, 0);

  Cell *source=new CellCylinderTruncatedZ ("Source", 0.412e-2, 0.2e-2, 0, steel1, centeredAt);
  Cell *cladding=new CellCylinderTruncatedZ ("Cladding", 0.475e-2, 0.665e-2, -0.665e-2, steel2, centeredAt);
  Cell *moderator=new CellCylinderTruncatedZ ("Moderator", 0.5e-2, 0.665e-2, -0.665e-2, steel3, centeredAt);
  Cell *coolant=new CellBox3D ("Coolant", -0.665e-2, 0.665e-2, 0.665e-2, -0.665e-2, 0.665e-2, -0.665e-2, steel4);

  source->next=cladding;
  cladding->next=moderator;
  moderator->next=coolant;

  MonteCarlo (N, xMin, xMax, yMin, yMax, zMin, zMax, source, sourceActivity);
  delete source;
  return 0;
}
