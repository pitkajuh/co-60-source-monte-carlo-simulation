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
  // Probability distribution function of Co-60 gamma energies.
  if (RNG (0, 1)<=0.85) return 1.173;
  return 1.332;
}

void ParseCells2 (Cell *cellHead, const int sourceActivity)
{
  Cell *current=cellHead;
  bool b=0;
  // Coordinate photonInitialPosition=cellHead->GetInitialPosition (Co60PDF ());
  // photonInitialPosition.print ();
  // std::cout<<current->name<<" "<<b<<'\n';
  const int activityRandom=PoissonRNG (sourceActivity, 1);
  Coordinate photonInitialPosition;

  for (int i=0; i<activityRandom; i++)
    {
      photonInitialPosition=cellHead->GetInitialPosition (Co60PDF ());
      // photonInitialPosition.print ();

      while (current!=nullptr and !b)
	{
	  b=current->CellTest (photonInitialPosition);
	  std::cout<<current->name<<" "<<b<<'\n';
	  current=current->next;
	}
      // if !b, initialize surface tracking
    }
  // delete current;
  cout<<" "<<'\n';
}

void GetCount (Cell *cellHead, const double volume, const int N)
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

void MonteCarlo (const int N, const double xMin, const double xMax, const double yMin, const double yMax, const double zMin, const double zMax, Cell *cell, const int sourceActivity)
{
  int n=0;
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
  const int N=10;
  const double xMin=-0.665;
  const double xMax=0.665;
  const double yMin=-0.665;
  const double yMax=0.665;
  const double zMin=-0.665;
  const double zMax=0.665;
  const int sourceActivity=100;
  Surface *S1=new Cylinder (0.412);
  Surface *S2=new Cylinder (0.475);
  Surface *S22=new Cylinder (0.5);
  Surface *S3=new PlaneX (-0.665);
  Surface *S4=new PlaneX (0.665);
  Surface *S5=new PlaneY (-0.665);
  Surface *S6=new PlaneY (0.665);
  Surface *S7=new PlaneZ (0.665);
  Surface *S71=new PlaneZ (0.665);
  Surface *S8=new PlaneZ (-0.665);
  Surface *S81=new PlaneZ (-0.665);

  Surface *S77=new PlaneZ (0.665);
  Surface *S88=new PlaneZ (-0.665);

  Surface *S9=new PlaneZ (0.2);
  Surface *S10=new PlaneZ (0);

  Material *steel1=new Steel (7.874);
  Material *steel2=new Steel (7.874);
  Material *steel3=new Steel (7.874);
  Material *steel4=new Steel (7.874);

  Cell *source=new CellCylinderTruncatedZ ("Source", S1, S9, S10, steel1);
  Cell *cladding=new CellCylinderTruncatedZ ("Cladding", S2, S7, S81, steel2);
  Cell *moderator=new CellCylinderTruncatedZ ("Moderator", S22, S77, S88, steel3);
  Cell *coolant=new CellBox3D ("Coolant", S3, S4, S6, S5, S71, S8, steel4);

  source->next=cladding;
  cladding->next=moderator;
  moderator->next=coolant;
  MonteCarlo (N, xMin, xMax, yMin, yMax, zMin, zMax, source, sourceActivity);
  delete source;
  return 0;
}
