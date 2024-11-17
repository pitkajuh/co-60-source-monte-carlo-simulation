#ifndef CELL_H
#define CELL_H

#include <string>
#include "Surface.h"
#include "Material.h"

using std::string;

class Cell
{
public:
  int hits=0;
  string name;
  bool isInCell=false;
  Material *material=nullptr;
  Surface *surface=nullptr;
  Cell *next=nullptr;

  const bool GetBoolean ()
  {
    if(isInCell )
      {
	hits++;
	return 1;
      }
    return 0;
  }
  virtual Coordinate GetInitialPosition (const double energy)=0;
  virtual const bool CellTest (const Coordinate &p)=0;
  virtual const double CellDistanceTest(const Coordinate &positionAt, Coordinate &directionTo)=0;
  virtual ~Cell ()
  {
    delete material;
    delete surface;
    delete next;
  }
};

class CellCylinderTruncatedZ: public Cell
{
public:
  Surface *wallZNegativeC=nullptr;
  Surface *wallZPositiveC=nullptr;

  Coordinate GetInitialPosition (const double energy)
  {
    // Get a random location for photon emission on the cylindrical surface, its direction and where it lands.
    const double radius=surface->value*pow(RNG(0, 1), 0.5);
    const double azimuthalAngle=2*M_PI*RNG (0, 1);
    const double z=wallZNegativeC->value+(wallZPositiveC->value-wallZNegativeC->value)*RNG (0, 1);
    Coordinate coordinateCylinder;
    coordinateCylinder.Set (radius*cos (azimuthalAngle), radius*sin (azimuthalAngle), z);
    // return coordinateCylinder+RandomEmissionDirection ()*(-1/material->GetMu (energy))*log (RNG (0, 1));
    return coordinateCylinder;
  }
  CellCylinderTruncatedZ (const string &name1, const double radius, const double wallZPos, const double wallZNeg, Material *m, const Coordinate &centeredAt)
  {
    name=name1;
    surface=new Cylinder(radius, centeredAt);
    wallZNegativeC=new PlaneZ(wallZNeg);
    wallZPositiveC=new PlaneZ(wallZPos);
    material=m;
  }
  const bool CellTest (const Coordinate &p)
  {
    isInCell=surface->SurfaceTest (p) and !wallZNegativeC->SurfaceTest (p) and wallZPositiveC->SurfaceTest (p);
    return GetBoolean ();
  }
  const double CellDistanceTest(const Coordinate &positionAt, Coordinate &directionTo)
  {
    const double distanceCylinder=surface->SurfaceDistance(positionAt, directionTo);
    const double distanceZNegative=wallZNegativeC->SurfaceDistance(positionAt, directionTo);
    const double distanceZPositive=wallZPositiveC->SurfaceDistance(positionAt, directionTo);
    const double distances[2]={distanceZNegative, distanceZPositive};
    double distance=distanceCylinder;

    for(const auto &i: distances){if(i>distance) distance=i;}
    return distance;
  }
  ~CellCylinderTruncatedZ ()
  {
    delete wallZNegativeC;
    delete wallZPositiveC;
  }
};

class CellBox3D: public Cell
{
public:
  // Three dimensional box cell has six walls.
  Surface *wallXNegative=nullptr;
  Surface *wallXPositive=nullptr;
  Surface *wallYPositive=nullptr;
  Surface *wallYNegative=nullptr;
  Surface *wallZNegative=nullptr;
  Surface *wallZPositive=nullptr;

  Coordinate GetInitialPosition (const double energy)
  {
    // TODO
    Coordinate c;
    return c;
  }
  const bool CellTest (const Coordinate &p)
  {
    isInCell=!wallXNegative->SurfaceTest (p) and wallXPositive->SurfaceTest (p) and wallYPositive->SurfaceTest (p) and !wallYNegative->SurfaceTest (p) and wallZPositive->SurfaceTest (p) and !wallZNegative->SurfaceTest (p);
    return GetBoolean ();
  }
  const double CellDistanceTest(const Coordinate &positionAt, Coordinate &directionTo)
  {
    const double distanceWallXNegative=wallXNegative->SurfaceDistance(positionAt, directionTo);
    const double distanceWallXPositive=wallXPositive->SurfaceDistance(positionAt, directionTo);
    const double distanceWallYPositive=wallYPositive->SurfaceDistance(positionAt, directionTo);
    const double distanceWallYNegative=wallYNegative->SurfaceDistance(positionAt, directionTo);
    const double distanceWallZNegative=wallZNegative->SurfaceDistance(positionAt, directionTo);
    const double distanceWallZPositive=wallZPositive->SurfaceDistance(positionAt, directionTo);
    const double distances[5]={distanceWallXPositive, distanceWallYPositive,
			                              distanceWallYNegative, distanceWallZNegative, distanceWallZPositive};
    double distance=distanceWallXNegative;

    for(const auto &i: distances){if(i>distance) distance=i;}
    return distance;
  }
  CellBox3D (const string &name1, const double w1, const double w2, const double w3, const double w4, const double w5, const double w6, Material *m)
  {
    material=m;
    name=name1;
    wallXNegative=new PlaneX(w1);
    wallXPositive=new PlaneX(w2);
    wallYPositive=new PlaneY(w3);
    wallYNegative=new PlaneY(w4);
    wallZPositive=new PlaneZ(w5);
    wallZNegative=new PlaneZ(w6);
  }
  ~CellBox3D ()
  {
    delete wallXNegative;
    delete wallXPositive;
    delete wallYPositive;
    delete wallYNegative;
    delete wallZNegative;
    delete wallZPositive;
  }
};

#endif
