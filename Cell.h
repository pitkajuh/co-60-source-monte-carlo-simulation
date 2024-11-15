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
    if (isInCell )
      {
	hits++;
	return 1;
      }
    return 0;
  }
  virtual Coordinate GetInitialPosition (const double energy)=0;
  // virtual Coordinate GetPhotonPosition (const Coordinate &from, const Coordinate &to)=0;
  virtual const bool CellTest (const Coordinate &p)=0;
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
    const double azimuthalAngle=2*M_PI*RNG (0, 1);
    const double z=(wallZPositiveC->value-wallZNegativeC->value)*RNG (0, 1);
    Coordinate coordinateCylinder;
    coordinateCylinder.Set (surface->value*cos (azimuthalAngle), surface->value*sin (azimuthalAngle), z);
    // return coordinateCylinder+RandomEmissionDirection ()*(-1/material->GetMu (energy))*log (RNG (0, 1));
    return coordinateCylinder;
  }
  // Coordinate GetPhotonPosition (const Coordinate &from, const Coordinate &to)
  // {
  //   return GetInitialPosition ()+RandomEmissionDirection ()*(-1/linearAttenuationCoefficient)*log (RNG (0, 1));
  // }
  // CellCylinderTruncatedZ (const string &name1, Surface *c, Surface *wallZPos, Surface *wallZNeg, Material *m)
  CellCylinderTruncatedZ (const string &name1, const double radius, const double wallZPos, const double wallZNeg, Material *m)
  {
    name=name1;
    surface=new Cylinder(radius);
    wallZNegativeC=new PlaneZ(wallZNeg);
    wallZPositiveC=new PlaneZ(wallZPos);
    material=m;
  }
  const bool CellTest (const Coordinate &p)
  {
    isInCell=surface->SurfaceTest (p) and !wallZNegativeC->SurfaceTest (p) and wallZPositiveC->SurfaceTest (p);
    return GetBoolean ();
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
  // Coordinate GetPhotonPosition (const Coordinate &from, const Coordinate &to)
  // {
  //   // TODO
  //   Coordinate c;
  //   return c;
  // }
  const bool CellTest (const Coordinate &p)
  {
    isInCell=!wallXNegative->SurfaceTest (p) and wallXPositive->SurfaceTest (p) and wallYPositive->SurfaceTest (p) and !wallYNegative->SurfaceTest (p) and wallZPositive->SurfaceTest (p) and !wallZNegative->SurfaceTest (p);
    return GetBoolean ();
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
