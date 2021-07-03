/*
 * OpenGL core wrapper.
 *
 * Author:  Hidetake Iwata
 * Time-stamp:  <2003-07-19 19:41:20 umeda>
 */
#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "glwrapper.h"


/* ライトのセット. */
void enableLight( GLenum lightnum, struct Light *light )
{
  glLightfv( lightnum, GL_AMBIENT, light->amb );
  glLightfv( lightnum, GL_DIFFUSE, light->dif );
  glLightfv( lightnum, GL_SPECULAR, light->spe );
  glLightfv( lightnum, GL_POSITION, light->pos );
  glEnable( lightnum );
}


/* 材質のセット. */
void setMaterial( struct Material *mat )
{
  glMaterialfv( GL_FRONT, GL_AMBIENT, mat->amb );
  glMaterialfv( GL_FRONT, GL_DIFFUSE, mat->dif );
  glMaterialfv( GL_FRONT, GL_SPECULAR, mat->spe );
  glMaterialf( GL_FRONT, GL_SHININESS, mat->shine );
}


/* テクスチャの生成. */
void createTexture( struct Texture *tex )
{
  int j;
  char buf[128];
  FILE *fp;
  
  /* 読み込み. */
  fp = fopen( tex->filename, "r" );
  if ( !fp ) {
    fprintf(stderr, "テクスチャ %s を読めません\n", tex->filename);
    exit(0);
  }
  tex->image = (GLubyte*)malloc( tex->width * tex->height * 3 * sizeof(GLubyte) );
  for(j = 0 ; j < 3; j++) {
    fgets(buf, sizeof(buf),fp);
    if(*buf == '#') j--;
  }
  fread( tex->image, tex->width * tex->height * 3, 1, fp );
  fclose( fp );
  
  /* 初期化. */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, tex->id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, tex->image);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
