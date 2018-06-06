#include "graphics.h"
#include <SDL2/SDL.h>
#include <cmath>

Graphics::Graphics(){
}

Graphics::~Graphics(){
}

/*  This sets up the opengl stuff */
void Graphics::init(int w, int h){
  this->width  = w;
  this->height = h;

  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  resizeWindow(w, h);
}

void Graphics::renderBackGround(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha){
  /* Set the background black */
  glClearColor( red, green, blue, alpha);
  /* Clear The back buffer And The Depth Buffer */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Graphics::resizeWindow(int w, int h){
  GLfloat ratio;

  //dont divide by zero
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
  return this->width;
}

int Graphics::getScreenHeight(){
  return this->height;
}
