/*
 * 2天体の動き.
 *
 * Author:  Hidetake Iwata
 * Time-stamp:  <2003-07-18 21:59:10 umeda>
 */
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <math.h>
#include "tentai.h"


/* 天体. */
struct Tentai tentai[2];
struct Point forceVector;


static void calculateForce()
{
  float	r, xx, yy,zz,mmg;
  const float g = 4.0;
  
  xx = tentai[0].p.x - tentai[1].p.x;
  yy = tentai[0].p.y - tentai[1].p.y;
  zz = tentai[0].p.z - tentai[1].p.z;
  
  mmg = tentai[0].m * tentai[1].m * g;
  r = sqrt( xx*xx + yy*yy + zz*zz );
  
  forceVector.x = -mmg * xx / (r * r * r);
  forceVector.y = -mmg * yy / (r * r * r);
  forceVector.z = -mmg * zz / (r * r * r);
}


static void applyToTentai( struct Tentai *t, const float dt, float sign )
{
  t->p1.x = t->v.x * dt + t->p.x;
  t->p1.y = t->v.y * dt + t->p.y;
  t->p1.z = t->v.z * dt + t->p.z;
  
  t->v1.x = sign * forceVector.x * dt / t->m + t->v.x;
  t->v1.y = sign * forceVector.y * dt / t->m + t->v.y;
  t->v1.z = sign * forceVector.z * dt / t->m + t->v.z;
  
  t->p.x = t->p1.x;
  t->p.y = t->p1.y;
  t->p.z = t->p1.z;
  
  t->v.x = t->v1.x;
  t->v.y = t->v1.y;
  t->v.z = t->v1.z;
}


/* 位置の計算. */
void calculatePosition()
{
  const float dt = 0.05;
  
  calculateForce();
  applyToTentai( &tentai[0], dt, 1 );
  applyToTentai( &tentai[1], dt, -1 );
}

