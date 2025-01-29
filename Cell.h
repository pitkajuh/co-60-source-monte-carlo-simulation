#ifndef CELL_H
#define CELL_H

#include "Surface.h"
#include "Material.h"

class Cell
{
public:
  unsigned hits=0;
  string name;
  Material *material=nullptr;
  Surface *surface=nullptr;
  Cell *next=nullptr;

  const bool GetBoolean(const bool isInCell)
  {
    if(isInCell)
      {
	hits++;
	return 1;
      }
    return 0;
  }
  virtual Coordinate GetInitialPosition()=0;
  virtual const bool CellTest(const Coordinate &p)=0;
  virtual const double CellDistanceTest(const Coordinate &positionAt, Coordinate &directionTo)=0;
  virtual ~Cell()
  {
    delete surface;
    delete next;
  }
};

class CellCylinderTruncatedZ: public Cell
{
public:
  Surface *wallZNegativeC=nullptr;
  Surface *wallZPositiveC=nullptr;

  Coordinate GetInitialPosition()
  {
    // Get a random location for photon emission on the cylindrical surface, its direction and where it lands.
    const double radial=surface->value*pow(RNG(0, 1), 0.5);
    const double azimuthalAngle=2*M_PI*RNG(0, 1);
    const double z=wallZNegativeC->value+(wallZPositiveC->value-wallZNegativeC->value)*RNG(0, 1);
    return {radial*cos(azimuthalAngle), radial*sin(azimuthalAngle), z};
  }
  CellCylinderTruncatedZ(const string &name1, const double radius, const double wallZPos, const double wallZNeg, Material *m, const Coordinate &centeredAt)
  {
    name=name1;
    surface=new Cylinder(radius, centeredAt);
    wallZNegativeC=new PlaneZ(wallZNeg);
    wallZPositiveC=new PlaneZ(wallZPos);
    material=m;
  }
  const bool CellTest(const Coordinate &p)
  {
    // const bool isInCell=surface->SurfaceTest(p) and !wallZNegativeC->SurfaceTest(p) and wallZPositiveC->SurfaceTest(p);
    const bool b1=surface->SurfaceTest(p) ;
    const bool b2=!wallZNegativeC->SurfaceTest(p);
    const bool b3=wallZPositiveC->SurfaceTest(p);
    // cout<<surface->SurfaceTest(p)<<" "<<!wallZNegativeC->SurfaceTest(p)<<" "<<wallZPositiveC->SurfaceTest(p)<<'\n';
    cout<<b1<<" "<<b2<<" "<<b3<<'\n';
    return GetBoolean(b1 and b2 and b3);
    // return GetBoolean(isInCell);
  }
  const double CellDistanceTest(const Coordinate &positionAt, Coordinate &directionTo)
  {
    const double distanceCylinder=surface->SurfaceDistance(positionAt, directionTo);
    const double distanceZNegative=wallZNegativeC->SurfaceDistance(positionAt, directionTo);
    const double distanceZPositive=wallZPositiveC->SurfaceDistance(positionAt, directionTo);
    const double distances[2]={distanceZNegative, distanceZPositive};
    double distance=distanceCylinder;

    // Find the distance to the surface that is the closest one, i.e. find the smallest value. The distance must be >0.
    for(const auto &i: distances)
      {
	if(i<distance and i>0) distance=i;
	else if(distance<0 and i>0) distance=i;
      }
    // cout<<distance<<'\n';
    return distance;
  }
  ~CellCylinderTruncatedZ()
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

  Coordinate GetInitialPosition()
  {
    // TODO
    Coordinate c;
    return c;
  }
  const bool CellTest(const Coordinate &p)
  {
    // const bool isInCell=!wallXNegative->SurfaceTest(p) and wallXPositive->SurfaceTest(p) and wallYPositive->SurfaceTest(p) and !wallYNegative->SurfaceTest(p) and wallZPositive->SurfaceTest(p) and !wallZNegative->SurfaceTest(p);
    const bool b1=!wallXNegative->SurfaceTest(p);
    const bool b2=wallXPositive->SurfaceTest(p) ;
    const bool b3=!wallYNegative->SurfaceTest(p);
    const bool b4=wallYPositive->SurfaceTest(p);
    const bool b5=!wallZNegative->SurfaceTest(p);
    const bool b6=wallZPositive->SurfaceTest(p);
    // cout<<!wallXNegative->SurfaceTest(p) <<" "<<wallXPositive->SurfaceTest(p) <<" "<<wallYPositive->SurfaceTest(p) <<" "<<!wallYNegative->SurfaceTest(p) <<" "<< wallZPositive->SurfaceTest(p)<<" "<<!wallZNegative->SurfaceTest(p)<<'\n';
    cout<<b1<<" "<<b2<<" "<<b3<<" "<<b4<<" "<<b5<<" "<<b6<<'\n';
    return GetBoolean(b1 and b2 and b3 and b4 and b4 and b5 and b6);
    // return GetBoolean(isInCell);
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

    // bool b1;
    // bool b2;
    // bool b3;
    // Find the distance to the surface that is the closest one, i.e. find the smallest value. The distance must be >0.
    for(const auto &i: distances)
      {
	// b1=i<distance;
	// b2=i>0;
	// b3=distance<0;
	// cout<<"try "<<distance<<" "<<i<<" "<<b1<<" "<<b2<<" "<<b3<<'\n';
	if(i<distance and i>0) distance=i;
	else if(distance<0 and i>0) distance=i;
      }
    // cout<<distance<<'\n';
    return distance;
  }
  CellBox3D(const string &name1, const double w1, const double w2, const double w3, const double w4, const double w5, const double w6, Material *m)
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
  ~CellBox3D()
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
