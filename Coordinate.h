#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>
#include <string>

struct Coordinate
{
public:
  double x;
  double y;
  double z;

  double GetLength(){return pow(x*x+y*y+z*z, 0.5);}
  void print()
  {
    std::cout<<"("<<x<<" "<<y<<" "<<z<<" size "<<GetLength()<<")"<<'\n';
  }
  std::string Show()
  {
    return std::to_string(x)+", "+std::to_string(x)+", "+std::to_string(x);
  }
  void Set(const double xValue, const double yValue, const double zValue)
  {
    x=xValue;
    y=yValue;
    z=zValue;
  }
  Coordinate operator-(const Coordinate &coordinate){return {x-coordinate.x, y-coordinate.y, z-coordinate.z};}
  Coordinate operator+(const Coordinate &coordinate){return {x+coordinate.x, y+coordinate.y, z+coordinate.z};}
  Coordinate operator*(const Coordinate &c){return {x*c.x, y*c.y, z*c.z};}
  Coordinate operator*(const double v){return {x*v, y*v, z*v};}
  Coordinate(){}
  Coordinate(const double xValue, const double yValue, const double zValue)
  {
    x=xValue;
    y=yValue;
    z=zValue;
  }
};

const Coordinate GenerateRandom(const double xMin, const double xMax, const double yMin, const double yMax, const double zMin, const double zMax)
{
  return {RNG(xMin, xMax), RNG(yMin, yMax), RNG(zMin, zMax)};
}

double DistanceBetween(Coordinate &from, Coordinate &to){return pow((to.x-from.x)*(to.x-from.x)+(to.y-from.y)*(to.y-from.y)+(to.z-from.z)*(to.z-from.z), 0.5);}

Coordinate RandomEmissionDirection()
{
  // Create an unit vector pointing in random direction using spherical coordinates. The radius of the sphere is not sampled because the sampling is done from the origin of the sphere.
  const double azimuthalAngle=2*M_PI*RNG(0, 1);
  const double polarAngle=M_PI*RNG(0, 1);
  return {cos(azimuthalAngle)*sin(polarAngle), sin(azimuthalAngle)*sin(polarAngle), cos(polarAngle)};
}

#endif
