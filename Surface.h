#ifndef SURFACE_H
#define SURFACE_H

#include "Coordinate.h"
#include <iostream>
using std::cout;

const bool GetSurfaceTest(const double v)
  {
    // Return true if the point is inside(<0) or on(==0) the surface.
    if(v<=0) return 1;
    return 0;
  }

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

  virtual const double SurfaceEquation(const Coordinate &p)=0;
  virtual const double SurfaceDistance(const Coordinate &positionAt, Coordinate &directionTo)=0;
  virtual const bool SurfaceTest(const Coordinate &p)=0;
  virtual ~Surface(){}
};

class Cylinder: public Surface
{
public:
  Coordinate centeredAt;
  Cylinder(const double v, const Coordinate &at)
  {
    value=v;
    centeredAt=at;
    A=1;
    B=1;
    J=-v*v;
  }
  const double SurfaceDistance(const Coordinate &positionAt, Coordinate &directionTo)
  {
    const double x=positionAt.x;
    const double y=positionAt.y;
    const double x0=centeredAt.x;
    const double y0=centeredAt.y;
    // Direction vector by using direction cosine.
    const double size=directionTo.GetLength();
    const double u=directionTo.x/size;
    const double v=directionTo.y/size;

    K=(x-x0)*(x-x0)+(y-y0)*(y-y0)+J;
    L=2*(u*(x-x0)+v*(y-y0));
    M=u*u+v*v;

    const double inSqrt=L*L-4*M*K;

    // No solution exists, the surface is away from line-of-sight.
    if(inSqrt<0 or M==0) return -1;

    const double sqrtValue=pow(inSqrt, 0.5);
    const double optionPositive=(-L+sqrtValue)/(2*M);
    const double optionNegative=(-L-sqrtValue)/(2*M);

    // No solution exists, the surface is away from line-of-sight.
    if(optionPositive<0 and optionNegative<0) return -1;
    else if(optionPositive>0 and optionNegative<0 and optionPositive>optionNegative) return optionPositive;
    else return optionNegative;
  }
  const double SurfaceEquation(const Coordinate &p)
  {
    const double x=p.x;
    const double x0=centeredAt.x;
    const double y=p.y;
    const double y0=centeredAt.y;
    // Equation should be A*(x-x0)*(x-x0)+B*(y-y0)*(y-y0)+J, but A and B are omitted because they are 1.
    // cout<<"("<<x<<"-"<<x0<<")*("<<x<<"-"<<x0<<")+("<<y<<"-"<<y0<<")*("<<y<<"-"<<y0<<")+J="<<(x-x0)*(x-x0)+(y-y0)*(y-y0)+J<<'\n';
    // cout<<"cylinder r="<<J<<" "<<(x-x0)*(x-x0)+(y-y0)*(y-y0)+J<<'\n';
    return(x-x0)*(x-x0)+(y-y0)*(y-y0)+J;
  }
  const bool SurfaceTest(const Coordinate &p){return GetSurfaceTest(SurfaceEquation(p));}
};

class PlaneX: public Surface
{
public:
  PlaneX(const double v)
  {
    value=v;
    G=1;
    J=-v;
  }
  const double SurfaceEquation(const Coordinate &p)
  {
    // cout<<"x "<<p.x+J<<'\n';
    return p.x+J;
  }
  const double SurfaceDistance(const Coordinate &positionAt, Coordinate &directionTo)
  {
    const double x=positionAt.x;
    const double size=directionTo.GetLength();
    const double u=directionTo.x/size;
    const double numerator=x+J;

    if(u==0) return -1;

    return -numerator/u;
  }
  const bool SurfaceTest(const Coordinate &p){return GetSurfaceTest(SurfaceEquation(p));}
};

class PlaneY: public Surface
{
public:
  PlaneY(const double v)
  {
    value=v;
    H=1;
    J=-v;
  }
  const double SurfaceEquation(const Coordinate &p)
  {
    // cout<<"y "<<p.y+J<<'\n';
    return p.y+J;
  }
  const double SurfaceDistance(const Coordinate &positionAt, Coordinate &directionTo)
  {
    const double y=positionAt.y;
    const double size=directionTo.GetLength();
    const double v=directionTo.y/size;
    const double numerator=y+J;

    if(v==0) return -1;

    return -numerator/v;
  }
  const bool SurfaceTest(const Coordinate &p){return GetSurfaceTest(SurfaceEquation(p));}
};
class PlaneZ: public Surface
{
public:
  PlaneZ(const double v)
  {
    value=v;
    I=1;
    J=-v;
  }
  const double SurfaceEquation(const Coordinate &p)
  {
    // cout<<"z "<<p.z+J<<'\n';
    return p.z+J;
  }
  const double SurfaceDistance(const Coordinate &positionAt, Coordinate &directionTo)
  {
    const double z=positionAt.z;
    const double size=directionTo.GetLength();
    const double w=directionTo.z/size;
    const double numerator=z+J;

    if(w==0) return -1;

    return -numerator/w;
  }
  const bool SurfaceTest(const Coordinate &p){return GetSurfaceTest(SurfaceEquation(p));}
};

#endif
