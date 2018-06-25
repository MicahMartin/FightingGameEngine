#include "imageSurface.h"

ImageSurface::ImageSurface(){}

ImageSurface::ImageSurface(const char* imgPath){
  imgBmp = SDL_LoadBMP(imgPath);

  if( imgBmp == NULL ){
    throw( SDL_GetError() );
  }
}

ImageSurface::~ImageSurface(){

  if( imgBmp == NULL ){
    throw( "This objects surface pointer does not currently point to anything" );
  }

  // free surface that imgBmp points to from memory 
  SDL_FreeSurface(imgBmp);
  imgBmp = NULL;
}

void ImageSurface::blitImg(SDL_Surface* surface) {

 SDL_BlitSurface(imgBmp, NULL, surface, NULL); 
}
