#ifndef SURFACE_H
#define SURFACE_H

#include "Coordinate.h"

class Surface
{
public:
  double value;
  // For general quadratic surface S(x, y, z)=Ax^2+By^2+Cz^2+Dxy+Eyz+F zx+Gx+Hy+Iz+J
  double A=0;
  double B=0;
  double C=0;
  double D=0;
  double E=0;
  double F=0;
  double G=0;
  double H=0;
  double I=0;
  double J=0;
  double L=0;
  double M=0;
  double K=0;

  virtual const double SurfaceEquation (const Coordinate &p)=0;
  virtual const double SurfaceDistance(const Coordinate &positionAt, const Coordinate &directionTo)=0;
  const double Distance(const Coordinate &positionAt, Coordinate &directionTo)
  {
    const double x=positionAt.x;
    const double y=positionAt.y;
    const double z=positionAt.z;
    // Direction vector by using direction cosine.
    const double size=directionTo.GetLength();
    const double u=directionTo.x/size;
    const double v=directionTo.y/size;
    const double w=directionTo.z/size;

    K=A*x*x+B*y*y+C*z*z+D*x*y+E*y*z+F*x*z+G*x+H*y+I*z+J;
    L=2*(A*u*x+B*v*y+C*w*z)+D*(v*x+u*y)+E*(w*y+v*z)+F*(w*x+u*z)+G*u+H*v+I*w;
    M=A*u*u+B*v*v+C*w*w+D*u*v+E*v*w+F*u*w;
    const double inSqrt=L*L-4*M*K;

    // No solution exists, the surface is away from line-of-sight.
    if(inSqrt<0) return 0;

    const double sqrtValue=pow(inSqrt, 0.5);
    const double optionPositive=-L+sqrtValue;
    const double optionNegative=-L-sqrtValue;

    // No solution exists, the surface is away from line-of-sight.
    if(optionPositive<0 and optionNegative<0) return 0;
    else if(optionPositive>0 and optionNegative<0) return optionPositive/(2*M);
    else return optionNegative/(2*M);
  }
  const bool SurfaceTest (const Coordinate &p)
  {
    // Return true if the point is inside (<0) or on (==0) the surface.
    if (SurfaceEquation (p)<=0) return 1;
    return 0;
  }
  virtual ~Surface(){}
};

class Cylinder: public Surface
{
public:
  Coordinate centeredAt;
  Cylinder (const double v, const Coordinate &at)
  {
    value=v;
    centeredAt=at;
    A=1;
    B=1;
    J=-v*v;
  }
  const double SurfaceDistance(const Coordinate &positionAt, const Coordinate &directionTo)
  {
    return 0;
  }
  const double SurfaceEquation (const Coordinate &p)
  {
    const double x=p.x;
    const double x0=centeredAt.x;
    const double y=p.y;
    const double y0=centeredAt.y;
    // Equation should be A*(x-x0)*(x-x0)+B*(y-y0)*(y-y0)+J, but A and B are omitted because they are 1.
    return (x-x0)*(x-x0)+(y-y0)*(y-y0)+J;
  }
};

class PlaneX: public Surface
{
public:
  PlaneX (const double v)
  {
    value=v;
    G=1;
    J=-v;
  }
  const double SurfaceEquation (const Coordinate &p){return p.x+J;}
  const double SurfaceDistance(const Coordinate &positionAt, const Coordinate &directionTo)
  {
    return 0;
  }
};

class PlaneY: public Surface
{
public:
  PlaneY (const double v)
  {
    value=v;
    H=1;
    J=-v;
  }
  const double SurfaceEquation (const Coordinate &p){return p.y+J;}
  const double SurfaceDistance(const Coordinate &positionAt, const Coordinate &directionTo)
  {
    return 0;
  }
};

class PlaneZ: public Surface
{
public:
  PlaneZ (const double v)
  {
    value=v;
    I=1;
    J=-v;
  }
  const double SurfaceEquation (const Coordinate &p){return p.z+J;}
  const double SurfaceDistance(const Coordinate &positionAt, const Coordinate &directionTo)
  {
    return 0;
  }
};

#endif
