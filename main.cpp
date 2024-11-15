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

void SurfaceTracking(const Coordinate &photonInitialPosition, Cell *cellHead)
{
  Cell *current=cellHead;
  bool collision=0;

  while (current!=nullptr and !collision)
    {
      collision=current->CellTest (photonInitialPosition);
      std::cout<<current->name<<" "<<collision<<'\n';
      current=current->next;
    }
  cout<<" "<<'\n';
}

void ParseCells2 (Cell *cellHead, const unsigned sourceActivity)
{
  const unsigned activityRandom=PoissonRNG (sourceActivity, 1);
  Coordinate photonInitialPosition;

  for (unsigned i=0; i<activityRandom; i++)
    {
      photonInitialPosition=cellHead->GetInitialPosition (Co60PDF ());
      photonInitialPosition.print();
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
      cout<<current->name<<" "<<vol<<'\n';
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
  Material *steel4=new Steel (7.874);

  Cell *source=new CellCylinderTruncatedZ ("Source", 0.412, 0.2, 0, steel1);
  Cell *cladding=new CellCylinderTruncatedZ ("Cladding", 0.475, 0.665, -0.665, steel2);
  Cell *moderator=new CellCylinderTruncatedZ ("Moderator", 0.5, 0.665, -0.665, steel3);
  Cell *coolant=new CellBox3D ("Coolant", -0.665, 0.665, 0.665, -0.665, 0.665, -0.665, steel4);

  source->next=cladding;
  cladding->next=moderator;
  moderator->next=coolant;
  MonteCarlo (N, xMin, xMax, yMin, yMax, zMin, zMax, source, sourceActivity);
  delete source;
  return 0;
}
