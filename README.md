# planet-opengl

This is a program to show 2 planets, written in C and OpenGL.
I wrote this program for a university class in 2003.

<img src="https://user-images.githubusercontent.com/321266/124353890-30d3b600-dc44-11eb-9fa3-fd1edb3b91ec.png" width="700" height="600">


## Getting Started

This can be run on macOS. Originally run on Linux.

```console
% make
cc -Wall -pipe -DGL_SILENCE_DEPRECATION   -c -o main.o main.c
cc -Wall -pipe -DGL_SILENCE_DEPRECATION   -c -o glwrapper.o glwrapper.c
cc -Wall -pipe -DGL_SILENCE_DEPRECATION   -c -o tentai.o tentai.c
cc -Wall -pipe -DGL_SILENCE_DEPRECATION   -c -o draw.o draw.c
cc   main.o glwrapper.o tentai.o draw.o  -framework GLUT -framework OpenGL -o main

% ./main
コマンドライン: ./main - -  - - -  - - -  - - -  - - -
天体質量1 =    1                  天体質量2 =    1
初期位置1 = (   1,    0,    0)    初期位置2 = (  -1,    0,    0)
初期速度1 = (   0,    1,    0)    初期速度2 = (   0,   -1,  0.2)

[xyz][XYZ] 視点の移動    [スペース] 一時停止    [ESC] 終了
[f] フルスクリーン切替   [r] 反射表示切替

[s] 影表示切替
[p] 位置表示切替         [v] ベクトル表示切替
[l] プレート表示切替     [n] 円柱表示切替
[e] 地球表示切替         [c] 天体2表示切替

[0] 注視点を固定         [4] 視点を固定
[1] 天体1                [5] 天体1
[2] 天体2                [6] 天体2
[3] 2天体の中心          [7] 2天体の中心
                         [8] 天体1の後ろ
                         [9] 天体2の後ろ
```

Press `l` and `n` to show a plate and cylinder.

<img src="https://user-images.githubusercontent.com/321266/124353947-77c1ab80-dc44-11eb-96aa-18595fc7fb29.png" width="700" height="600">


## Author

Hidetake Iwata, 2003.

This repository contains the images. Copyright belongs to each right holder.
