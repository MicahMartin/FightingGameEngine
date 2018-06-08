#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>

#include "graphics.h"
#include "textureManager.h"


int main(int argc, char *argv[]){

  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("we broke\n");
    return -1;
  };

  SDL_Window *window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
  void *glContext = SDL_GL_CreateContext(window);


  Graphics *graphics = new Graphics();
  graphics->init(800, 600);

  graphics->renderBackGround(1.0, 0.0, 0.0, 0.0);
  SDL_GL_SwapWindow(window);
  SDL_Delay(5000);

  SDL_Texture *megaMan;
  SDL_Surface *tmpSurface = IMG_Load("../data/megaMan.jpg");

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  //if (tmpSurface) {
  //  printf("heres the number of bites per pixel? %d\n", tmpSurface->format->BytesPerPixel);
  //} else {
  //  printf("null pointer exception!\n");
  //  return -1;
  //}

  int Mode = GL_RGB;
  if(tmpSurface->format->BytesPerPixel == 4) {
      Mode = GL_RGBA;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, Mode, tmpSurface->w, tmpSurface->h, 0, Mode, GL_UNSIGNED_BYTE, tmpSurface->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int X = 0;
  int Y = 0;
  int Width = 100;
  int Height = 100;
   
  glBegin(GL_QUADS);
      glTexCoord2f(0, 0); glVertex3f(X, Y, 0);
      glTexCoord2f(1, 0); glVertex3f(X + Width, Y, 0);
      glTexCoord2f(1, 1); glVertex3f(X + Width, Y + Height, 0);
      glTexCoord2f(0, 1); glVertex3f(X, Y + Height, 0);
  glEnd();
  SDL_GL_SwapWindow(window);
  SDL_Delay(5000);

  SDL_GL_DeleteContext(glContext);
  SDL_Quit();

  return 0;
}
