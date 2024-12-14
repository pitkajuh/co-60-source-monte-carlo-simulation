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

double CellSearch(Cell *cellHead, const Coordinate &mfp, Coordinate &photonDirection)
{
  Cell *current=cellHead;
  bool collision=0;

  while(current!=nullptr and !collision)
    {
      collision=current->CellTest(mfp);
      current=current->next;
    }
  return current->CellDistanceTest(mfp, photonDirection);
}

void SurfaceTracking(Coordinate &photonFrom, const double photonToDistance, Coordinate &photonDirection, Cell *cellHead, RadioNuclide *radioNuclide)
{
  Coordinate photonTo=photonDirection*photonToDistance;
  Coordinate mfp=photonFrom+photonTo;
  Coordinate surfaceLocation;
  Cell *current=cellHead;
  bool collision=0;
  double boundaryDistance=0;
  double collisionDistance=0;

  mfp.print();

  while(current!=nullptr and !collision)
    {
      collision=current->CellTest(mfp);

      boundaryDistance=current->CellDistanceTest(photonFrom, photonDirection);
      collisionDistance=mfp.GetLength();

      if(collisionDistance<boundaryDistance)
	{
	  cout<<"No boundary crossed!"<<'\n';
	  // Call physics routine
	}
      else
	{
	  cout<<"Boundary crossed!"<<'\n';
	  surfaceLocation=photonDirection*boundaryDistance;
	  SurfaceTracking(surfaceLocation, photonToDistance, photonDirection, current->next, radioNuclide);
	}

      std::cout<<current->name<<" "<<collision<<" distance to surface "<<boundaryDistance<<" collision at "<<collisionDistance<<'\n';
      current=current->next;
    }
  cout<<" "<<'\n';
}

void ParseCells2(Cell *cellHead, RadioNuclide *radioNuclide, const unsigned time)
{
  const unsigned activityRandom=PoissonRNG(radioNuclide->GetActivity(), time);
  double energy;
  double photonToDistance;
  Coordinate photonFrom;
  Coordinate photonTo;
  Coordinate direction;

  for(unsigned i=0; i<activityRandom; i++)
    {
      direction=RandomEmissionDirection();
      photonFrom=cellHead->GetInitialPosition();
      energy=radioNuclide->PDF();
      photonToDistance=(-1/cellHead->material->GetMu(energy))*log(RNG(0, 1));
      SurfaceTracking(photonFrom, photonToDistance, direction, cellHead, radioNuclide);
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
      ParseCells2(cell, radioNuclide, time);
      t++;
    }
  printf("---------------------------------------\n");

  GetCount(cell, time);
}

int main()
{
  // const unsigned time=1;
  const unsigned sourceActivity=100;
  const double v3=1;
  Material *steel1=new Steel(7.874);
  Material *steel2=new Steel(7.874);
  Material *steel3=new Steel(7.874);

  Coordinate centeredAt(0, 0, 0);
  const double v1=0.1;
  const double v2=0.2;

  Cell *source=new CellCylinderTruncatedZ("Source", v1, v1, -v1, steel1, centeredAt);
  Cell *cladding=new CellCylinderTruncatedZ("Cladding", v2, v1, -v1, steel2, centeredAt);
  Cell *outside=new CellBox3D("Outside world", -v3, v3, v3, -v3, v2, -v2, steel3);

  source->next=cladding;
  cladding->next=outside;


    // Material *steel4=new Steel(7.874);
  // Material *steel5=new Steel(7.874);
  // const double sourceWidth=1e-6;
  // const double sourceRadius=35e-3/2;

  // const double sourceCladdingWidth=3e-3;
  // const double sourceCladdingRadius=50e-3/2;

  // const double sourceAirBelowHeight=1e-3;
  // const double sourceAirAboveHeight=2e-3;


  // // const double calibrationToolOuterRadius=89e-3;
  // // const double calibrationToolInnerRadius=79e-3;
  // // const double calibrationToolSecondInnerRadius=62e-3;
  // const double calibrationToolBottomWidth=1e-3;

  // // Source and its metal cladding
  // Cell *sourceAirBelow=new CellCylinderTruncatedZ("Source air below", sourceRadius, sourceAirBelowHeight, calibrationToolBottomWidth, steel1, centeredAt);
  // Cell *source=new CellCylinderTruncatedZ("Source", sourceRadius, sourceWidth+sourceAirBelowHeight, sourceAirBelowHeight+calibrationToolBottomWidth, steel2, centeredAt);
  // Cell *sourceAirAbove=new CellCylinderTruncatedZ("Source air above", sourceRadius, sourceAirAboveHeight+sourceAirBelowHeight+sourceWidth, sourceAirBelowHeight+sourceWidth, steel3, centeredAt);

  // Cell *sourceCladding=new CellCylinderTruncatedZ("Source cladding", sourceCladdingRadius, sourceCladdingWidth, 0, steel5, centeredAt);

  // Cell *outside=new CellBox3D("Outside world", -v3, v3, v3, -v3, v3, -v3, steel4);

  // source->next=sourceAirBelow;
  // sourceAirBelow->next=sourceAirAbove;
  // sourceAirAbove->next=sourceCladding;
  // sourceCladding->next=outside;

  RadioNuclide *co60=new Co60(sourceActivity);
  GetCrossSection();
  // MonteCarlo(time, source, co60);

  delete source;
  delete co60;

  return 0;
}
