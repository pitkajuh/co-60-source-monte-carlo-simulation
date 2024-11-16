#ifndef SURFACE_H
#define SURFACE_H

#include <cmath>
#include "Coordinate.h"

class Surface
{
public:
  double value;
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
  void DistanceSet()
  {
    // K=Ax2 + By2 + Cz2 + Dxy + Eyz + F xz + Gx + Hy + Iz + J;
    // L=2(Aux + Bvy + Cwz) + D(vx + uy) + E(wy + vz) + F (wx + uz)+ Gu + Hv + Iw;
    // M=Au2 + Bv2 + Cw2 + Duv + Evw + F uw;
  }
  const double Distance(const Coordinate &p)
  {
    // const double L=0;
    // const double M=0;
    // const double K=0;
    const double inSqrt=L*L-4*M*K;
    // Check if inSqrt negative.
    const double sqrtValue=pow(inSqrt, 0.5);
    const double optionPositive=-L+sqrtValue;
    // const double optionNegative=-L-sqrtValue;
    // Check which option is valid
    const double result=optionPositive;
    return result/(2*M);

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
    J=-1;
  }
  const double SurfaceEquation (const Coordinate &p){return (p.y-centeredAt.y)*(p.y-centeredAt.y)+(p.x-centeredAt.x)*(p.x-centeredAt.x)-value*value;}
};

class PlaneX: public Surface
{
public:
  PlaneX (const double v){value=v;}
  const double SurfaceEquation (const Coordinate &p){return p.x-value;}
};

class PlaneY: public Surface
{
public:
  PlaneY (const double v){value=v;}
  const double SurfaceEquation (const Coordinate &p){return p.y-value;}
};

class PlaneZ: public Surface
{
public:
  PlaneZ (const double v){value=v;}
  const double SurfaceEquation (const Coordinate &p){return p.z-value;}
};

#endif
