/*
 * 2天体の動き.
 *
 * Author:  Hidetake Iwata
 * Time-stamp:  <2003-07-21 17:21:39 umeda>
 */
#ifndef _E2_TENTAI_H_
#define _E2_TENTAI_H_


/* 座標. */
struct Point {
  float x, y, z;
};


/* 座標. */
struct Pointd {
  double x, y, z;
};


/* 天体. */
struct Tentai {
  float m;
  struct Point p;    // 位置ベクトル.
  struct Point v;    // 速度ベクトル.
  struct Point p1;   // 位置ベクトル (数値積分用).
  struct Point v1;   // 速度ベクトル (数値積分用).
};


/* 2天体. */
extern struct Tentai tentai[2];

/* 万有引力. */
extern struct Point forceVector;


/* 位置を計算. */
extern void calculatePosition();


#endif /* _E2_TENTAI_H_ */
