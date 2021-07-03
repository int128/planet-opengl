/*
 * 描画モジュール.
 *
 * Author:  Hidetake Iwata
 * Time-stamp:  <2003-07-27 21:59:23 umeda>
 */
#include <math.h>
#include <stdio.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "glwrapper.h"
#include "tentai.h"
#include "draw.h"


/* 繰り返し. */
#define  FILL3( f )    f, f, f


/* ライト. */
struct Light lightDefault = { {0.0, 0.0, 0.0, 1.0},
			      { FILL3( 0.8 ), 0.0},
			      { FILL3( 0.8 ), 0.0},
			      { FILL3( 0.8 ), 0.0} };

/* 平面を照らすライト.
struct Light lightFlat = { {0.0, 0.0, 0.0, 1.0},
			   { FILL3( 1.0 ), 0.0},
			   { FILL3( 1.0 ), 0.0},
			   { FILL3( 1.0 ), 0.0} }; */

/* 地球. */
struct Material earthMaterial = { { FILL3( 0.8 ), 1.0},
				  { FILL3( 0.5 ), 1.0},
				  { FILL3( 0.8 ), 1.0},
				  2.0};

/* もう一つの天体. */
struct Material colaMaterial = { { FILL3( 0.3 ), 1.0},
				 { FILL3( 0.4 ), 1.0},
				 { FILL3( 0.5 ), 1.0},
				 2.0 };

/* 輪. */
struct Material ringMaterial = { { FILL3( 0.2 ), 1.0},
				 { FILL3( 0.3 ), 1.0},
				 { FILL3( 0.7 ), 1.0},
				 2.0};

/* 地球の軌跡. */
struct Material rec1Material = { {0.1, 0.3, 0.6, 1.0},
				 { FILL3( 0.1 ), 1.0},
				 { FILL3( 0.8 ), 1.0},
				 2.0};

/* 軌跡. */
struct Material rec2Material = { {0.6, 0.1, 0.3, 1.0},
				 { FILL3( 0.1 ), 1.0},
				 { FILL3( 0.8 ), 1.0},
				 2.0};

/* 地球の位置. */
struct Material stat1Material = { {0.4, 0.7, 1.0, 1.0},
				  { FILL3( 0.1 ), 1.0},
				  { FILL3( 0.0 ), 1.0},
				  2.0};

/* もう一つの位置. */
struct Material stat2Material = { {1.0, 0.4, 0.7, 1.0},
				  { FILL3( 0.1 ), 1.0},
				  { FILL3( 0.0 ), 1.0},
				  2.0};

/* shadow. */
struct Material shadowMaterial = { { 0.2, 0.25, 0.2, 1.0},
				   { FILL3( 0.1 ), 1.0},
				   { FILL3( 0.1 ), 1.0},
				   2.0};

/* another shadow. */
struct Material neoMaterial = { { FILL3( 0.7 ), 1.0},
				{ FILL3( 0.5 ), 1.0},
				{ FILL3( 0.9 ), 1.0},
				2.0};

/* 平面. */
struct Material flatMaterial = { { FILL3( 0.1 ), 1.0},
				 { FILL3( 0.1 ), 1.0},
				 { FILL3( 0.6 ), 1.0},
				 2.0};

/* 2天体を結ぶプレート. */
struct Material plateMaterial = { { FILL3( 0.6 ), 1.0},
				  { FILL3( 0.1 ), 1.0},
				  { FILL3( 0.8 ), 1.0},
				  2.0};

/* 2天体を結ぶ線分. */
struct Material centerLineMaterial = { { FILL3( 0.5 ), 1.0},
				       { FILL3( 0.1 ), 1.0},
				       { FILL3( 0.4 ), 1.0},
				       2.0};


/* 視点の初期位置. */
struct Point camera = { 5.0, 5.0, 10.0 };
struct Point cameraBias = { 3.0, 2.0, 4.0 };


/* 注視点. */
struct Point lookAt = { 0.0, 0.0, 0.0 };
struct Point lookAtBias = { 0.0, 0.0, 0.0 };


/* テクスチャセット. */
struct Texture earthTexture = { "earth-256x128.ppm", 256, 128 };
struct Texture colaTexture = { "coca-cola-256x256.ppm", 256, 256 };
struct Texture matrixTexture = { "matrixnext-256x256.ppm", 256, 256 };
struct Texture matrixbgTexture = { "matrixbg-256x256.ppm", 256, 256 };
struct Texture matrixSmithTexture = { "matrix-smith-256x256.ppm", 256, 256 };


/* 視点切り替え. */
int lookAtMode = 3;
int cameraMode = 8;


/* 軌跡の記録. */
#define RECMAX   512
static struct Point moveRecord[2][RECMAX];
static int recFirstTime = 1;
static int recFrame = 0;
static int recIndex = 0;


/* マウス. */
extern GLint x_mem1;
extern GLint y_mem1;


/* 動作するか. */
int fMoving = 0;

/* パラメータ表示. */
int fShowParam = 1;

/* プレート表示. */
int fShowPlate = 0;

/* Neo表示. */
int fShowNeo = 0;

/* 地球表示. */
int fShowEarth = 1;

/* もう一つの天体表示. */
int fShowCola = 1;

/* 反射あり. */
int fShowReflect = 0;

/* ベクトル表示. */
int fShowVector = 1;

/* 影表示. */
int fShowShadow = 1;


/* 2天体の平面距離. */
static float flatDistance;

/* 2天体の距離. */
static float distance;

/* 2天体の中心座標. */
static struct Point centerPoint;

/* 万有引力の大きさ. */
static float forceSize;


/* Point型への代入. */
inline static void setPoint( struct Point *d, struct Point *s )
{
  d->x = s->x;
  d->y = s->y;
  d->z = s->z;
}


inline static void glTranslatePoint( struct Point *p )
{
  glTranslatef( p->x, p->z, p->y );
}


/* ステンシルバッファ有効. */
static void beginReflectBase()
{
  /* Don't update color or depth. */
  glDisable(GL_DEPTH_TEST);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  /* Draw 1 into the stencil buffer. */
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
}


/* ステンシルバッファへのレンダリング. */
static void beginReflect()
{
  /* Re-enable update of color and depth. */ 
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  /* Now, only render where stencil is set to 1. */
  glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  glPushMatrix();
  /* The critical reflection step: Reflect dinosaur through the floor
     (the Y=0 plane) to make a relection. */
  glScalef(1.0, -1.0, 1.0);
}


/* ステンシルバッファ無効. */
static void endReflect()
{
  glPopMatrix();
  glDisable(GL_STENCIL_TEST);  
}


/* 各種の計算. */
static void calculatePoints()
{
  float	xx, yy, zz;
  xx = tentai[0].p.x - tentai[1].p.x;
  yy = tentai[0].p.y - tentai[1].p.y;
  zz = tentai[0].p.z - tentai[1].p.z;
  
  flatDistance = sqrt( xx * xx + yy * yy );
  distance = sqrt( xx * xx + yy * yy + zz * zz );
  
  centerPoint.x = ( tentai[0].p.x + tentai[1].p.x ) / 2;
  centerPoint.y = ( tentai[0].p.y + tentai[1].p.y ) / 2;
  centerPoint.z = ( tentai[0].p.z + tentai[1].p.z ) / 2;
  
  xx = forceVector.x;
  yy = forceVector.y;
  zz = forceVector.z;
  forceSize = sqrt( xx * xx + yy * yy + zz * zz );
}


/* 視点の切り替え. */
void changeView()
{
  /* 注視点の切り替え. */
  switch( lookAtMode ) {
  case 1:
    /* 天体1を見る. */
    setPoint( &lookAt, &tentai[0].p );
    break;
    
  case 2:
    /* 天体2を見る. */
    setPoint( &lookAt, &tentai[1].p );
    break;
  
  case 3:
    /* 2天体の中心を見る. */
    setPoint( &lookAt, &centerPoint );
    break;
  }
  
  /* 視点の切り替え. */
  switch( cameraMode ) {
  case 5:
    /* 天体1. */
    setPoint( &camera, &tentai[0].p );
    break;
    
  case 6:
    /* 天体2. */
    setPoint( &camera, &tentai[1].p );
    break;
    
  case 7:
    /* 2天体の中心. */
    setPoint( &camera, &centerPoint );
    break;
    
  case 8:
    /* 天体1. */
    camera.x = tentai[0].p.x + (tentai[0].p.x - tentai[1].p.x);
    camera.y = tentai[0].p.y + (tentai[0].p.y - tentai[1].p.y);
    camera.z = tentai[0].p.z + (tentai[0].p.z - tentai[1].p.z);
    break;
    
  case 9:
    /* 天体2. */
    camera.x = tentai[1].p.x + (tentai[1].p.x - tentai[0].p.x);
    camera.y = tentai[1].p.y + (tentai[1].p.y - tentai[0].p.y);
    camera.z = tentai[1].p.z + (tentai[1].p.z - tentai[0].p.z);
    break;
  }
}


/* 効果. */
static void setEffect()
{
  glLightfv( GL_LIGHT0, GL_POSITION, lightDefault.pos );
  //glFogf( GL_FOG_END, distance / 2 );
}


/* 平面の生成 */
static void drawFlat()
{
  float i, j;
  const float size = 10.0f, step = 1.0f, z = -0.01f, mul = 0.5f;
  
  setMaterial( &flatMaterial );
  glBegin( GL_QUADS );
  for( i = -size ; i < size; i += step ) {
    for( j = -size ; j < size; j += step ) {
      glVertex3f( i, z, j );
      glVertex3f( i, z, j + step * mul );
      glVertex3f( i + step * mul, z, j + step * mul );
      glVertex3f( i + step * mul, z, j );
    }
  }
  glEnd();
}


/* プレート. */
static void drawPlate()
{
  GLUquadricObj*  q;
 
  glPushMatrix();
  glTranslatef( centerPoint.x, centerPoint.z, centerPoint.y );
  
  setMaterial( &plateMaterial );
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, matrixbgTexture.id );
  
  q = gluNewQuadric();
  gluQuadricTexture( q, GL_TRUE );
  glRotatef( 90.0, 1.0, 0.0, 0.0 );
  glRotatef( -90.0, 0.0, 0.0, 1.0 );
  gluDisk( q, 0, flatDistance / 2, 32, 32 );
  gluDeleteQuadric( q );
  
  glPopMatrix();
  glDisable( GL_TEXTURE_2D );
}


static void drawArrow( struct Point *s, struct Point *d )
{
  /*        1
   *       ／＼
   *    7／6  3＼2
   *     ‾|  |‾
   *       |  |
   *      5‾‾4
   */
  const float w = 0.1f;
  const struct Point c = { d->x/2, d->y/2, d->z/2 };
  
  float r = sqrt( d->x * d->x + d->y * d->y );
  float x1 = w * d->y / r, y1 = w * d->x / r;
  
  glBegin( GL_POLYGON );
  /* (1) */
  glVertex3f( s->x + d->x,
	      s->z + d->z,
	      s->y + d->y );
  /* (2) */
  glVertex3f( s->x + c.x - x1 - x1,
	      s->z + c.z,
	      s->y + c.y + y1 + y1 );
  /* (3) */
  glVertex3f( s->x + c.x - x1,
	      s->z + c.z,
	      s->y + c.y + y1 );
  /* (4) */
  glVertex3f( s->x - x1,
	      s->z,
	      s->y + y1 );
  /* (5) */
  glVertex3f( s->x + x1,
	      s->z,
	      s->y - y1 );
  /* (6) */
  glVertex3f( s->x + c.x + x1,
	      s->z + c.z,
	      s->y + c.y - y1 );
  /* (7) */
  glVertex3f( s->x + c.x + x1 + x1,
	      s->z + c.z,
	      s->y + c.y - y1 - y1 );
  glEnd();
}


/* ベクトル */
static void drawVector()
{
  //const float mul = distance;
  const float mul = 10.0f;
  struct Point mulf = { forceVector.x * mul, forceVector.y * mul, forceVector.z * mul };
  struct Point revf = { -forceVector.x * mul, -forceVector.y * mul, -forceVector.z * mul };
  
  /* 天体1 */
  setMaterial( &rec1Material );
  drawArrow( &tentai[0].p, &tentai[0].v );
  drawArrow( &tentai[0].p, &mulf );
  
  /* 天体2 */
  setMaterial( &rec2Material );
  drawArrow( &tentai[1].p, &tentai[1].v );
  drawArrow( &tentai[1].p, &revf );
}


/* 軌跡 */
static void drawRecording()
{
  int i;
  
  /* 天体1 */
  setMaterial( &rec1Material );
  glBegin( GL_LINE_STRIP );
  
  if( !recFirstTime )
    for( i = recIndex; i < RECMAX; i++ )
      glVertex3f( moveRecord[0][i].x, moveRecord[0][i].z, moveRecord[0][i].y );
  for( i = 0; i < recIndex; i++ )
    glVertex3f( moveRecord[0][i].x, moveRecord[0][i].z, moveRecord[0][i].y );
  glEnd();
  
  /* 天体2 */
  setMaterial( &rec2Material );
  glBegin(GL_LINE_STRIP);
  
  if( !recFirstTime )
    for( i = recIndex; i < RECMAX; i++ )
      glVertex3f( moveRecord[1][i].x, moveRecord[1][i].z, moveRecord[1][i].y );
  for( i = 0; i < recIndex; i++ )
    glVertex3f( moveRecord[1][i].x, moveRecord[1][i].z, moveRecord[1][i].y );
  glEnd();
  
  /* 2天体を結ぶ線分 */
  setMaterial( &centerLineMaterial );
  
  glBegin( GL_LINES );
  glVertex3f( tentai[0].p.x, tentai[0].p.z, tentai[0].p.y );
  glVertex3f( tentai[1].p.x, tentai[1].p.z, tentai[1].p.y );
  glEnd();
}


/* 天体1の生成 */
static void drawStar1()
{
  GLUquadricObj*  q;
  
  glPushMatrix();
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, earthTexture.id );
  setMaterial( &earthMaterial );
  
  q = gluNewQuadric();
  gluQuadricTexture( q, GL_TRUE );
  
  glTranslatef( tentai[0].p.x, tentai[0].p.z, tentai[0].p.y );
  glRotatef( recFrame * 4, 0.0, 1.0, 0.0 );
  gluSphere( q, 0.5, 32, 16 );
  
  gluDeleteQuadric( q );
  glDisable( GL_TEXTURE_2D );
  glPopMatrix();  
}


/* 天体2の生成 */
static void drawStar2()
{
  GLUquadricObj*  q;
  glPushMatrix();
  
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, colaTexture.id );
  setMaterial( &colaMaterial );
  
  q = gluNewQuadric();
  gluQuadricTexture( q, GL_TRUE );
  
  glTranslatef( tentai[1].p.x, tentai[1].p.z, tentai[1].p.y );
  glRotatef( recFrame * 4, 0.0, 1.0, 0.0 );
  gluSphere( q, 0.5, 32, 16 );
  
  /* 円盤. */
  glDisable( GL_TEXTURE_2D );
  gluQuadricTexture( q, GL_FALSE );
  setMaterial( &ringMaterial );
  
  glRotatef( 90.0, 1.0, 0.0, 0.0 );
  //glutSolidTorus( 0.1, 0.9, 32, 32 );
  gluDisk( q, 0.7, 0.9, 32, 32 );
  
  gluDeleteQuadric( q );
  glPopMatrix();
}


/* another shadow. */
static void drawNeo()
{
  GLUquadricObj*  q;
  
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, matrixTexture.id );
  setMaterial( &neoMaterial );
  
  q = gluNewQuadric();
  gluQuadricTexture( q, GL_TRUE );
  
  /* 天体1 */
  glPushMatrix();
  glTranslatePoint( &tentai[0].p );
  glRotatef( 90.0, 1.0, 0.0, 0.0 );
  if( tentai[0].p.z > 0 )
    gluCylinder( q, 0.5, 0.5, tentai[0].p.z, 32, 32 );
  else {
    glRotatef( 180.0, 0.0, 1.0, 0.0 );
    gluCylinder( q, 0.5, 0.5, -tentai[0].p.z, 32, 32 );
  }
  glPopMatrix();
  
  /* 天体2 */
  glBindTexture( GL_TEXTURE_2D, matrixSmithTexture.id );
  glPushMatrix();
  glTranslatePoint( &tentai[1].p );
  glRotatef( 90.0, 1.0, 0.0, 0.0 );
  if( tentai[1].p.z > 0 )
    gluCylinder( q, 0.5, 0.5, tentai[1].p.z, 32, 32 );
  else {
    glRotatef( 180.0, 0.0, 1.0, 0.0 );
    gluCylinder( q, 0.5, 0.5, -tentai[1].p.z, 32, 32 );
  }
  glPopMatrix();
  
  gluDeleteQuadric( q );
  glDisable( GL_TEXTURE_2D );
}


/* 影をつける. */
static void drawShadow()
{
  GLUquadricObj*  q;
  
  setMaterial( &shadowMaterial );
  q = gluNewQuadric();
  
  /* 天体1 */
  glPushMatrix();
  glTranslatef( tentai[0].p.x, 0.0, tentai[0].p.y );
  glRotatef( 90.0, 1.0, 0.0, 0.0 );
  gluDisk( q, 0.0, 0.5, 32, 32 );
  glPopMatrix();
  
  /* 天体2 */
  glPushMatrix();
  glTranslatef( tentai[1].p.x, 0.0, tentai[1].p.y );
  glRotatef( 90.0, 1.0, 0.0, 0.0 );
  gluDisk( q, 0.0, 0.5, 32, 32 );
  glPopMatrix();
  
  gluDeleteQuadric( q );
}


/* 軌跡の記録. */
static void recordPosition()
{
  const int i = recIndex;
  if( recFrame % 3 == 0 ) {
    setPoint( &moveRecord[0][i], &tentai[0].p );
    setPoint( &moveRecord[1][i], &tentai[1].p );
    
    if( recIndex < RECMAX - 1 )
      recIndex++;
    else {
      recIndex = 0;
      if( recFirstTime )
	recFirstTime = 0;
    }
  }
  recFrame++;
}


static void drawString( char *s )
{
  while( *s )
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *s++ );
}


/* パラメータの表示. */
static void drawStatus()
{
  char buf[128];
  
  /* 天体1 */
  setMaterial( &stat1Material );
  glRasterPos3f( tentai[0].p.x,
		 tentai[0].p.z + 1.0,
		 tentai[0].p.y );
  
  sprintf( buf, "(% .3f, % .3f, % .3f)",
	   tentai[0].p.x, tentai[0].p.y, tentai[0].p.z );
  drawString( buf );
  
  /* 天体2 */
  setMaterial( &stat2Material );
  glRasterPos3f( tentai[1].p.x,
		 tentai[1].p.z + 1.0,
		 tentai[1].p.y );
  
  sprintf( buf, "(% .3f, % .3f, % .3f)",
	   tentai[1].p.x, tentai[1].p.y, tentai[1].p.z );
  drawString( buf );
  
  /* ベクトル */
  setMaterial( &centerLineMaterial );
  glRasterPos3f( centerPoint.x,
		 centerPoint.z + 1.0,
		 centerPoint.y );
  
  sprintf( buf, "(% .3f, % .3f, % .3f) = %.3f",
	   forceVector.x, forceVector.y, forceVector.z,
	   forceSize );
  drawString( buf );  
}


void drawObject()
{
  calculatePoints();
     
  /* 視点. */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45, 4.0/3.0, 0.1, 500 );
  changeView();
  gluLookAt( camera.x + cameraBias.x, camera.z + cameraBias.z, camera.y + cameraBias.y,
	     lookAt.x + lookAtBias.x, lookAt.z + lookAtBias.z, lookAt.y + lookAtBias.y,
	     0.0, 1.0, 0.0 );
  
  /* ライト位置. */
  lightDefault.pos[0] = centerPoint.x;
  lightDefault.pos[1] = centerPoint.z;
  lightDefault.pos[2] = centerPoint.y;
  lightDefault.pos[3] = 0.0f;
  
  setEffect();
  
  /* 動かすか? */
  if( fMoving ) {
    calculatePosition();
    recordPosition();
  }
  
  /* 描画. */
  //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  drawRecording();
  
  /* 反射. */
  if( fShowReflect ) {
    beginReflectBase();
    drawFlat();
    
    beginReflect();
    
    drawRecording();
    if( fShowVector )
      drawVector();
    if( fShowEarth )
      drawStar1();
    if( fShowCola )
      drawStar2();
    if( fShowPlate )
      drawPlate();
    if( fShowNeo )
      drawNeo();
    
    endReflect();
  }
  
  /* 透明なもの. */
  glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );
  glDepthMask( GL_FALSE );
  
  drawFlat();
  if( fShowVector )
    drawVector();
  if( fShowEarth )
    drawStar1();
  if( fShowCola )
    drawStar2();
  if( fShowParam )
    drawStatus();
  if( fShowPlate )
    drawPlate();
  if( fShowNeo )
    drawNeo();
  if( fShowShadow )
    drawShadow();
  
  glDepthMask(GL_TRUE);
  glBlendFunc(GL_ONE, GL_ZERO);
}
