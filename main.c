#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "glwrapper.h"
#include "tentai.h"
#include "draw.h"

#define  WINDOW_WIDTH   1024
#define  WINDOW_HEIGHT  768
#define  STATE_DOWN     0
#define  STATE_UP       1


// マウス移動用.
static GLdouble spin_x = 0.0;
static GLdouble spin_y = 0.0;

GLint x_mem1 = 0;
GLint y_mem1 = 0;
static GLint x_mem2 = 0;
static GLint y_mem2 = 0;

static GLint dragLeft;


void mouse(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON) {
    if(state==GLUT_DOWN) {
	x_mem2 = x_mem1;
	y_mem2 = y_mem1;
	x_mem1 = x;
	y_mem1 = y;
	dragLeft=STATE_DOWN;
      }
    else
      dragLeft=STATE_UP;
  }
}


void motion(int x, int y)
{
  if(dragLeft == STATE_DOWN) {
    if(x_mem1 != x)
      spin_x -= 300 * 2.0 * (x_mem1 - x) / WINDOW_WIDTH;
    if(y_mem1 != y)
      spin_y -= 300 * 2.0 * (y_mem1 - y) / WINDOW_HEIGHT;
    
    if(spin_x > 360)
      spin_x -= 360;
    else if(spin_x < -360)
      spin_x += 360;
    
    x_mem2 = x_mem1;
    y_mem2 = y_mem1;
    x_mem1 = x;
    y_mem1 = y;
  }
  
  glutPostRedisplay();
}


void keyboard( unsigned char key, int x, int y )
{
  switch( key ) {
  case '\x1b':
    exit(0);
    break;
    
  case 'f':
    glutFullScreen(); break;
    
  case ' ':
    fMoving = !fMoving; break;
    
  case '0':
  case '1':
  case '2':
  case '3':
    lookAtMode = key - '0';
    break;
    
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    cameraMode = key - '0';
    break;
    
  case '\t':
    cameraBias.x = cameraBias.y = cameraBias.z = 0;
    lookAtBias.x = lookAtBias.y = lookAtBias.z = 0;
    break;
    
  case 'p':
    fShowParam = !fShowParam; break;
  case 'l':
    fShowPlate = !fShowPlate; break;
  case 'n':
    fShowNeo = !fShowNeo; break;
  case 'e':
    fShowEarth = !fShowEarth; break;
  case 'c':
    fShowCola = !fShowCola; break;
  case 'r':
    fShowReflect = !fShowReflect; break;
  case 'v':
    fShowVector = !fShowVector; break;
  case 's':
    fShowShadow = !fShowShadow; break;

  case 'X':
    cameraBias.x -= 1; break;
  case 'x':
    cameraBias.x += 1; break;
    
  case 'Y':
    cameraBias.y -= 1; break;
  case 'y':
    cameraBias.y += 1; break;
    
  case 'Z':
    cameraBias.z -= 1; break;
  case 'z':
    cameraBias.z += 1; break;
  }
}


void handleMenu( int value )
{
  if( value )
    keyboard( (unsigned char)value, 0, 0 );
}


void idle(void)
{
  glutPostRedisplay();
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  drawObject();
  glutSwapBuffers();
}


void initializeTexture()
{
  GLuint textureIDs[3];
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CW);
  
  glGenTextures( 5, textureIDs );
  
  /* コカコーラ. */
  colaTexture.id = textureIDs[0];
  createTexture( &colaTexture );
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  /* 地球. */
  earthTexture.id = textureIDs[1];
  createTexture( &earthTexture );
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  matrixTexture.id = textureIDs[2];
  createTexture( &matrixTexture );
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  matrixbgTexture.id = textureIDs[3];
  createTexture( &matrixbgTexture );
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  matrixSmithTexture.id = textureIDs[4];
  createTexture( &matrixSmithTexture );
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void initializeLight()
{
  glLightModelf( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
  glEnable( GL_LIGHTING );
  //glEnable( GL_NORMALIZE );
  glEnable( GL_DEPTH_TEST );
  
  enableLight( GL_LIGHT0, &lightDefault );
  //enableLight( GL_LIGHT1, &lightFlat );
}


static void initializeMenu()
{
  glutCreateMenu( handleMenu );

  glutAddMenuEntry( "Enable fullscreen", 'f' );
  glutAddMenuEntry( "----", 0 );
  glutAddMenuEntry( "Toggle reflection", 'r' );
  glutAddMenuEntry( "Toggle shadow", 's' );
  glutAddMenuEntry( "----", 0 );
  glutAddMenuEntry( "Toggle position", 'p' );
  glutAddMenuEntry( "Toggle vector", 'v' );
  glutAddMenuEntry( "Toggle plate", 'l' );
  glutAddMenuEntry( "Toggle Neo and Smith", 'n' );
  glutAddMenuEntry( "Toggle earth", 'e' );
  glutAddMenuEntry( "Toggle coca-cola", 'c' );
  glutAddMenuEntry( "----", 0 );
  glutAddMenuEntry( "Look at free", '0' );
  glutAddMenuEntry( "Look at earth", '1' );
  glutAddMenuEntry( "Look at coca-cola", '2' );
  glutAddMenuEntry( "Look at center", '3' );
  glutAddMenuEntry( "----", 0 );
  glutAddMenuEntry( "Look from free", '4' );
  glutAddMenuEntry( "Look from earth", '5' );
  glutAddMenuEntry( "Look from coca-cola", '6' );
  glutAddMenuEntry( "Look from center", '7' );
  glutAddMenuEntry( "Look from behind earth", '8' );
  glutAddMenuEntry( "Look from behind coca-cola", '9' );
  
  glutAttachMenu( GLUT_RIGHT_BUTTON );
}


void initialize()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  dragLeft = STATE_UP;
  initializeMenu();
  
  /* 混色を有効にする. */
  glEnable( GL_BLEND );
  //glEnable( GL_LINE_SMOOTH );
  //glEnable( GL_FOG );
  
  initializeLight();
  initializeTexture();
}


void registerFunctions()
{
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
}


static float getArg( char str[], float defValue, int valid )
{
  float t;
  if( !valid )
    return defValue;
  return ( sscanf(str, "%f", &t) != 1 ) ? defValue : t;
}


static void setParameters( char *argv[] )
{
  char *dummy[15];
  int valid = ( argv != NULL );
  if( !valid )
    argv = dummy;
  
  tentai[0].m = getArg( argv[1], 1.0, valid );
  tentai[1].m = getArg( argv[2], 1.0, valid );

  tentai[0].p.x = getArg( argv[3], 1.0, valid );
  tentai[0].p.y = getArg( argv[4], 0.0, valid );
  tentai[0].p.z = getArg( argv[5], 0.0, valid );

  tentai[1].p.x = getArg( argv[6], -1.0, valid );
  tentai[1].p.y = getArg( argv[7], 0.0, valid );
  tentai[1].p.z = getArg( argv[8], 0.0, valid );

  tentai[0].v.x = getArg( argv[9], 0.0, valid );
  tentai[0].v.y = getArg( argv[10], 1.0, valid );
  tentai[0].v.z = getArg( argv[11], 0.0, valid );

  tentai[1].v.x = getArg( argv[12], 0.0, valid );
  tentai[1].v.y = getArg( argv[13], -1.0, valid );
  tentai[1].v.z = getArg( argv[14], 0.2, valid );
}


static void showParameters()
{
  printf( "天体質量1 = %4g                  天体質量2 = %4g\n"
	  "初期位置1 = (%4g, %4g, %4g)    初期位置2 = (%4g, %4g, %4g) \n"
	  "初期速度1 = (%4g, %4g, %4g)    初期速度2 = (%4g, %4g, %4g) \n",
	  tentai[0].m,
	  tentai[1].m,
	  tentai[0].p.x, tentai[0].p.y, tentai[0].p.z,
	  tentai[1].p.x, tentai[1].p.y, tentai[1].p.z,
	  tentai[0].v.x, tentai[0].v.y, tentai[0].v.z,
	  tentai[1].v.x, tentai[1].v.y, tentai[1].v.z );
}


static void parseArguments( int argc, char *argv[] )
{
  if( argc < 15 ) {
    setParameters( NULL );
    printf( "コマンドライン: %s - -  - - -  - - -  - - -  - - -\n", argv[0] );
    showParameters();
//    exit( 1 );
  }
  else {
    setParameters( argv );
    showParameters();
  }
}


int main(int argc,char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(0, -20);
  glutCreateWindow( "Open GL:  M A T R I X  R E L O A D E D" );
  
  parseArguments( argc, argv );
  
  puts( "\n"
	"[xyz][XYZ] 視点の移動    [スペース] 一時停止    [ESC] 終了\n"
	"[f] フルスクリーン切替   [r] 反射表示切替\n"
	"\n"
	"[s] 影表示切替\n"
	"[p] 位置表示切替         [v] ベクトル表示切替\n"
	"[l] プレート表示切替     [n] 円柱表示切替\n"
	"[e] 地球表示切替         [c] 天体2表示切替\n"
	"\n"
	"[0] 注視点を固定         [4] 視点を固定\n"
	"[1] 天体1                [5] 天体1\n"
	"[2] 天体2                [6] 天体2\n"
	"[3] 2天体の中心          [7] 2天体の中心\n"
	"                         [8] 天体1の後ろ\n"
	"                         [9] 天体2の後ろ"
	);
  
  initialize();
  registerFunctions();
  glutMainLoop();
  return 0;
}
