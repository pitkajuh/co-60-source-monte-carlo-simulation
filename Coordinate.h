#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>

struct Coordinate
{
public:
  double x;
  double y;
  double z;
  void print ()
  {
    std::cout<<x<<" "<<y<<" "<<z<<'\n';
  }
  double GetLength(){return pow(x*x+y*y+z*z, 0.5);}
  void Set (const double xValue, const double yValue, const double zValue)
  {
    x=xValue;
    y=yValue;
    z=zValue;
  }
  Coordinate operator+(const Coordinate &coordinate)
  {
    Coordinate result;
    result.Set (x+coordinate.x, y+coordinate.y, z+coordinate.z);
    return result;
  }
  Coordinate operator*(const double v)
  {
    Coordinate result;
    result.Set (x*v, y*v, z*v);
    return result;
  }
};

const Coordinate GenerateRandom (const double xMin, const double xMax, const double yMin, const double yMax, const double zMin, const double zMax)
{
  Coordinate p;
  p.Set (RNG (xMin, xMax), RNG (yMin, yMax), RNG (zMin, zMax));
  return p;
}

Coordinate RandomEmissionDirection ()
{
  // Get random direction for isotropic source using spherical coordinates.
  const double azimuthalAngle=2*M_PI*RNG (0, 1);
  const double polarAngle=M_PI*RNG (0, 1);
  Coordinate unitVector;
  unitVector.Set (cos (azimuthalAngle)*sin (polarAngle), sin (azimuthalAngle)*sin (polarAngle), cos (polarAngle));
  return unitVector;
}

#endif
