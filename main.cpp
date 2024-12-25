#include <iostream>
#include <cmath>
#include <utility>
#include "math.h"
#include "Material.h"
#include "Cell.h"
#include "Tape.h"
#include "Coordinate.h"
#include "RadioNuclide.h"
#include "ReadCrossSections.h"
#include "Reactions.h"
#include "PhotonAngularDistribution.h"

using std::cout;

void PhysicsRoutine(Cell *cell, double &photonEnergy)
{
  cout<<"PhysicsRoutine "<<'\n';
  cell->material->crossSections.PDF(photonEnergy);
}

void SurfaceTracking(Cell *cellHead, Photon photon)
{
  bool collision=0;
  Coordinate surfaceLocation;
  Cell *current=cellHead;
  double boundaryDistance=0;
  double collisionDistance=0;

  photon.mfp.print();

  while(current!=nullptr and !collision)
    {
      collision=current->CellTest(photon.mfp);

      boundaryDistance=current->CellDistanceTest(photon.origin, photon.direction);
      collisionDistance=photon.mfp.GetLength();

      if(collisionDistance<boundaryDistance)
	{
	  cout<<"No boundary crossed! "<<photon.energy<<'\n';
	  // Call physics routine
	  PhysicsRoutine(current, photon.energy);
	}
      else
	{
	  cout<<"Boundary crossed! "<<photon.energy<<'\n';
	  surfaceLocation=photon.direction*boundaryDistance;
	  // SurfaceTracking(current->next, gamma);
	}

      std::cout<<current->name<<" "<<collision<<" distance to surface "<<boundaryDistance<<" collision at "<<collisionDistance<<'\n';
      current=current->next;
    }
  cout<<" "<<'\n';
}

void ParseCells2(Cell *cellHead, RadioNuclide *radioNuclide, const unsigned time)
{
  const unsigned activityRandom=PoissonRNG(radioNuclide->GetActivity(), time);

  for(unsigned i=0; i<activityRandom; i++)
    {
      Photon gamma(cellHead, radioNuclide);
      SurfaceTracking(cellHead, gamma);
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
  // unsigned t=0;

  // while(t<time)
  //   {
  //     ParseCells2(cell, radioNuclide, time);
  //     t++;
  //   }
  // printf("---------------------------------------\n");

  // GetCount(cell, time);
}

int main()
{
  // const unsigned time=1;
  // const unsigned sourceActivity=100;
  // const double v3=1;
  // Material *steel1=new Steel(7.874);
  // Material *steel2=new Steel(7.874);
  // Material *steel3=new Steel(7.874);

  // Coordinate centeredAt(0, 0, 0);
  // const double v1=0.1;
  // const double v2=0.2;

  // Cell *source=new CellCylinderTruncatedZ("Source", v1, v1, -v1, steel1, centeredAt);
  // Cell *cladding=new CellCylinderTruncatedZ("Cladding", v2, v1, -v1, steel2, centeredAt);
  // Cell *outside=new CellBox3D("Outside world", -v3, v3, v3, -v3, v2, -v2, steel3);

  // source->next=cladding;
  // cladding->next=outside;


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

  // RadioNuclide *co60=new Co60(sourceActivity);
  // // GetCrossSection();
  // MonteCarlo(time, source, co60);

  Tape *steelENDF=new Tape("./cross-sections/photoat-026_Fe_000.endf");
  // Tape *nitrogenENDF=new Tape("./cross-sections/photoat-007_N_000.endf");
  // Tape *sodiumENDF=new Tape("./cross-sections/photoat-011_Na_000.endf");
  // Tape *titaniumENDF=new Tape("./cross-sections/photoat-022_Ti_000.endf");
  // Tape *iodineENDF=new Tape("./cross-sections/photoat-053_I_000.endf");
  // steelENDF->Get(1332e3);

  PhotonAngularDistribution *steelAngular=new IncoherentAngularDistribution(steelENDF);
  delete steelAngular;
  delete steelENDF;
  // delete nitrogenENDF;
  // delete sodiumENDF;
  // delete titaniumENDF;
  // delete iodineENDF;

  // delete source;
  // delete co60;

  return 0;
}
