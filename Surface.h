#ifndef SURFACE_H
#define SURFACE_H

class Surface
{
public:
  double value;

  virtual const double SurfaceEquation (const Coordinate &p)=0;
  const bool SurfaceTest (const Coordinate &p)
  {
    // Return true if the point is inside (<0) or on (==0) the surface.
    if (SurfaceEquation (p)<=0) return 1;
    return 0;
  }
};

class Cylinder: public Surface
{
public:
  Cylinder (const double v){value=v;}
  const double SurfaceEquation (const Coordinate &p){return p.y*p.y+p.x*p.x-value*value;}
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
