/* UobiOne.cpp - v0.01 - A tentative to develop a game/game engine using SDL 2.2.0.5
   List of files:
   - UobiOne.cpp    - main file
   - loadassets.cpp - load images using stb_image.h (https://github.com/nothings/stb)
   - base.h         - vector structures mostly
   - entities.h     - contains the main structure for the game. Renderer window and entities information
   ...
 */

#include "base.h"
#include "entities.h"

#define PLAYER_X 50
#define PLAYER_Y 50
#define PLAYER_SPEED 60
#define MAX_ENTITIES 1

// Keyboard
#define UP    SDL_SCANCODE_UP
#define DOWN  SDL_SCANCODE_DOWN 
#define LEFT  SDL_SCANCODE_LEFT
#define RIGHT SDL_SCANCODE_RIGHT
#define AXIS 4

// Coordinates
#define NORTH 90 
#define SOUTH 270
#define EAST  0
#define OVEST 180
#define EPSILON 0.29951539475553562
#define RTD M_PI / 180

Uint8 mov_inputs[AXIS] = {RIGHT, UP, LEFT, DOWN};
SDL_Event keyboard_event;


// Delta time and  FPS
u64 NOW = SDL_GetPerformanceCounter();
u32 FPS_NOW =  SDL_GetTicks();
double delta_time = 0;
u32 currentFPS = 0;

//////////////////////////////////////////////////////////////////////////////
//
// Angle of 2 vectors (in points)
//
double core_pointdir(double x1, double y1, double x2, double y2){
  // TODO(ste) this doesn't work at all
  double angle = 0;
  
  y1 = y1 * -1;
  y2 = y2 * -1;
  
  if ( x1 == 0 && y1 == 0 && y2 < 0 && x2 == 0 )
    return angle = 270;
  
  if ( x1 == 0 && y1 == 0 && y2== 0 && x2 < 0 )
    return angle = 180;

  if (x1 == 0 && y1 == 0 && y2 > 0 && x2 == 0)
	  return angle = 90;

  if (x1 == 0 && y1 == 0 && y2 == 0 && x2 > 0)
	  return angle = 0;

  double m = ( x2 - x1 ) / ( y2 - y1 );
  angle = ( atan(m) * 180 ) / M_PI;

  return angle;
}


//////////////////////////////////////////////////////////////////////////////
//
// Lenght X
//
s32 core_xlenght(s32 lenght, double angle){
	if (angle == NORTH || angle == SOUTH)
		return 0;

  double s = cos(angle*RTD) ;
    
  s32 result = (s < 0) ? -1 : 1;
  
  s32 x = (lenght * result) ;
  TRACE(("x: %d\n", x));
  return x;
}



//////////////////////////////////////////////////////////////////////////////
//
// Lenght Y
//
s32 core_ylenght(s32 lenght, double angle){

	if (angle == EAST || angle == OVEST)
		return 0;

  double s = sin(angle);
  s32 result = (s < 0) ? -1 : 1;
  
  s32 y = (lenght * result) * -1 ;
  TRACE(("y: %d\n", y));
  return y;
}

//////////////////////////////////////////////////////////////////////////////
//
// Calc delta_time - Calc FPS
//
void dtime(double * delta_time, u32 * current_FPS){
  u32 delta_clock =  SDL_GetTicks() - FPS_NOW;
  FPS_NOW = SDL_GetTicks();
  
  if (delta_clock != 0)
    *current_FPS = 1000 / delta_clock;
 
  u64 last  = NOW;
  NOW = SDL_GetPerformanceCounter();
  *delta_time = ( ( NOW - last) * 1000 / (double)SDL_GetPerformanceFrequency() ) * 0.001;
}


//////////////////////////////////////////////////////////////////////////////
//
// Read events and create/change the movements vectors
//
void check_mov(SDL_Rect* rect){  
  const Uint8* current_key_state = SDL_GetKeyboardState(NULL);
  s32 move_xinput = 0;
  s32 move_yinput = 0;
  
  for (int i = 0; i < AXIS; i++){
    const Uint8 current = mov_inputs[i];
    if (current_key_state[current]) {
      double angle = i * 90;
      move_xinput = move_xinput + (s32)core_xlenght(1, angle);
      move_yinput = move_yinput + (s32)core_ylenght(1, angle);
    }
  }


  if (vcomp(vmake(0, 0), vmake(move_xinput, move_yinput))){
    double move_speed_this_frame = PLAYER_SPEED * delta_time;
    s32 move_dir = (s32)core_pointdir(0, 0, move_xinput, move_yinput);

   
    
    rect->x += (s32)core_xlenght(move_speed_this_frame, move_dir);
    rect->y += (s32)core_ylenght(move_speed_this_frame, move_dir);  
  }
}


//////////////////////////////////////////////////////////////////////////////
//
// Init the game
//
void game_init(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL error -> %sn", SDL_GetError());
    exit(1);
  }
  
  unsigned int w = Game.screen.w;
  unsigned int h = Game.screen.h;
  const char* name = Game.screen.name;
  
  Game.screen.window = SDL_CreateWindow(
                                        name,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        w, h, 0
                                        );
  Game.screen.renderer = SDL_CreateRenderer(
                                            Game.screen.window, -1,
                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                                            );
  
  SDL_Surface* surface = loadSurface("Assets/son_goku.png");
  //int n = ((surface->w / 8)*(surface->h / 8) + 1);
  int n = (PLAYER_X * PLAYER_Y);
  
  Game.gfx.n = n;
  Game.gfx.spritesheet = (SDL_Surface**)malloc(sizeof(SDL_Surface*)*n);
  
 
  SDL_Rect rect = { 0, 0, PLAYER_X, PLAYER_Y };

  for (int i = 0; i < MAX_ENTITIES; ++i) {
    
    Game.gfx.spritesheet[i] = SDL_CreateRGBSurface(0, PLAYER_X, PLAYER_Y, 24, 0x00, 0x00, 0x00, 0x00);
    SDL_SetColorKey(Game.gfx.spritesheet[i], 1, 0xFF00FF);
    SDL_FillRect(Game.gfx.spritesheet[i], 0, 0xFF00FF);

    rect.x = PLAYER_X;
    rect.y = PLAYER_Y;
    SDL_BlitSurface(surface, &rect, Game.gfx.spritesheet[i], NULL);
    
  }
  
  SDL_FreeSurface(surface);
  
  Game.running = SDL_TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
// Destroy all the entities loaded
//
void game_quit(void) {
  int i;
  for (i = 0; i<MAX_ENTITIES; ++i)
    SDL_FreeSurface(Game.gfx.spritesheet[i]);
  free(Game.gfx.spritesheet);
  Game.gfx.spritesheet = NULL;
  
  SDL_DestroyRenderer(Game.screen.renderer);
  Game.screen.renderer = NULL;
  
  SDL_DestroyWindow(Game.screen.window);
  Game.screen.window = NULL;
  
  SDL_Quit();
}

//////////////////////////////////////////////////////////////////////////////
//
// Main loop
//
int main(int argc, char* argv[]) {

  game_init();

  SDL_Rect player_rect = {50,50,50,50};
  SDL_Texture* t_player = SDL_CreateTextureFromSurface(Game.screen.renderer, Game.gfx.spritesheet[0]);
  
  SDL_Event event;
  while (Game.running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        Game.running = SDL_FALSE;
      } break;
      }
    }
    
    dtime(&delta_time, &currentFPS);
    
    SDL_RenderClear(Game.screen.renderer);
    check_mov(&player_rect);
    // TRACE(("FPS: %d\n", currentFPS));
    SDL_RenderCopy(Game.screen.renderer, t_player, NULL, &player_rect);
    SDL_RenderPresent(Game.screen.renderer);
  }
   
  SDL_DestroyTexture(t_player);
  
  Game.quit();  
  return 0;
}
