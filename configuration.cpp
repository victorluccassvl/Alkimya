#ifndef CONFIGURATION
#define CONFIGURATION

#include <SDL2/SDL_image.h>
#include <cglm/cglm.h>
#include "glad.c"

// SDLGlWindow class
const int _GL_MAJOR_VERSION = 3;
const int _GL_MINOR_VERSION = 3;

const int _SDL_IMG_FLAGS = IMG_INIT_JPG;

const char _SDL_WINDOW_DEFAULT_TITLE[] = { " Titulo " };
const int _SDL_WINDOW_MAX_TITLE_NAME = 50;
const int _SDL_WINDOW_DEFAULT_X_POS = 0;
const int _SDL_WINDOW_DEFAULT_Y_POS = 0;
const int _SDL_WINDOW_DEFAULT_WIDTH = 800;
const int _SDL_WINDOW_DEFAULT_HEIGHT = 600;
const Uint32 _SDL_WINDOW_FLAGS = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

// OpenGl class
const int _GL_MAX_SHADER_COMPILATION_LOG_SIZE = 512;
const int _GL_MAX_SHADER_PROGRAM_COMPILATION_LOG_SIZE = 512;


#endif