/*
 * 描画モジュール.
 *
 * Author:  Hidetake Iwata
 * Time-stamp:  <2003-07-24 12:43:35 e601008>
 */
#ifndef _E2_DRAW_H_
#define _E2_DRAW_H_
#include "glwrapper.h"


/* ライト. */
extern struct Light lightDefault;
extern struct Light lightFlat;


/* exports は必要ない.
extern struct Material earthMaterial;
extern struct Material rec1Material;

extern struct Material teapotMaterial;
extern struct Material rec2Material;
extern struct Material ringMaterial;

extern struct Material flatMaterial;
extern struct Material plateMaterial;
extern struct Material vectorMaterial; */


/* 視点と注視点. */
extern struct Point camera;
extern struct Point lookAt;

extern struct Point cameraBias;
extern struct Point lookAtBias;


/* テクスチャセット. */
extern struct Texture earthTexture;
extern struct Texture colaTexture;
extern struct Texture matrixTexture;
extern struct Texture matrixbgTexture;
extern struct Texture matrixSmithTexture;


/* 描画ルーチン. */
extern void drawObject();


/* 動作するか. */
extern int fMoving;


/* パラメータ表示. */
extern int fShowParam;

/* Neo表示. */
extern int fShowNeo;

/* 地球表示. */
extern int fShowEarth;

/* もう一つの天体表示. */
extern int fShowCola;

/* プレート表示. */
extern int fShowPlate;

/* 反射あり. */
extern int fShowReflect;

/* ベクトル表示. */
extern int fShowVector;

/* 影表示. */
extern int fShowShadow;


/* 視点切り替え. */
extern int cameraMode;
extern int lookAtMode;



#endif /* _E2_DRAW_H_ */
