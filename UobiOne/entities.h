// entities.h
#ifndef _ENTITIES_H_
#define _ENTITIES_H_

void game_init(void);
void game_quit(void);


//Screen dimension constants
#define SCREEN_NAME "Prototype"
#define SCREEN_W 640
#define SCREEN_H 480
#define SCREEN_SCALE 1


SDL_Surface* loadSurface(std::string path);



struct {
  // define "attributes"
  SDL_bool running;
  struct {
    unsigned int w;
    unsigned int h;
    const char* name;
    SDL_Window* window;
    SDL_Renderer* renderer;
  } screen;
  
  struct {
    u32 n;
    SDL_Surface** spritesheet;
  } gfx;
  
  // define "methods"
  void(*init)(void);
  void(*quit)(void);
} Game = {
  SDL_FALSE,
  {
    SCREEN_SCALE*SCREEN_W,
    SCREEN_SCALE*SCREEN_H,
    SCREEN_NAME,
    NULL,
    NULL
  },
  { 0, NULL },
  game_init,
  game_quit
};

#endif
