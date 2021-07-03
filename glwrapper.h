/*
 * OpenGL core wrapper.
 *
 * Author:  Hidetake Iwata
 * Time-stamp:  <2003-07-18 22:04:43 umeda>
 */
#ifndef _E2_GLWRAPPER_H_
#define _E2_GLWRAPPER_H_

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif


/* 光源設定値 */
struct Light
{
  float pos[4];   /* 座標 (x, y, z, w) */
  float amb[4];   /* 環境色 (r, g, b, a) */
  float dif[4];   /* 拡散色 (r, g, b, a) */
  float spe[4];   /* 鏡面光 (r, g, b, a) */
};


/* 材質の構造体. */
struct Material
{
  float amb[4];   /* 環境色 (r, g, b, a) */
  float dif[4];   /* 拡散色 (r, g, b, a) */
  float spe[4];   /* 鏡面光 (r, g, b, a) */
  float shine;    /* 光沢 */
};


/* テクスチャの構造体. */
struct Texture
{
  char *filename;
  int width, height;
  GLuint id;
  GLubyte *image;
};


void enableLight( GLenum lightnum, struct Light *light );
void setMaterial( struct Material *mat );
void createTexture( struct Texture *tex );


#endif /* _E2_GLWRAPPER_H_ */
