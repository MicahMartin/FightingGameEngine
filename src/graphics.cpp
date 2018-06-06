#include "graphics.h"
#include <cmath>

Graphics::Graphics(){
}

Graphics::~Graphics(){
}

void Graphics::init(int w, int h){
   width  = w;
   height = h;

   glClearColor( 0.0f, 0.0f, 0.0f, 0.5f);
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_TEXTURE_2D);

   resizeWindow(w, h);
}

void Graphics::resizeWindow(int w, int h){
   GLfloat ratio;

   if(h == 0)
      h = 1;

   ratio = (GLfloat)w/(GLfloat)h;
   glViewport(0, 0, (GLint)w, (GLint)h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   perspectiveGl( 45.0f, ratio, 0.1f, 200.0f);

   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();
}

void Graphics::perspectiveGl(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar){
  const GLdouble pi = 3.1415926535897932384626433832795;
  GLdouble fW, fH;

  fH = tan( fovY / 360 * pi ) * zNear;
  fW = fH * aspect;

  glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

int Graphics::getScreenWidth(){
   return width;
}

int Graphics::getScreenHeight(){
   return height;
}
