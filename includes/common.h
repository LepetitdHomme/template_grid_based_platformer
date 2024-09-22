#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define DEFAULT_Y_MOVE 2

#define BACKGROUND_SPEED 30
#define BACKGROUND_MOVE 4
#define MAP_SPEED 20
#define MAP_MOVE 5
#define MAP_MAX_DELTA 5
#define MAP_DELTA_X 0.1
#define SHOW_ALPHA false

typedef struct app {
	SDL_Window *window;
	SDL_Renderer *renderer;
} app_struct;

typedef struct terrain {
	SDL_Texture *background;
	SDL_Rect original;
	SDL_Rect background1;
	SDL_Rect background2;
	int delta_x;
} terrain_struct;

typedef struct player {
	SDL_Texture *texture;
	SDL_Rect original;
	SDL_Rect src;
	SDL_Rect dst;
	int delta_y;
	int delta_x;
	bool blocked;
} player_struct;

typedef unsigned char tileindex;

typedef struct
{
	SDL_Rect rect;
	int opaque;
} TileProp;

typedef struct
{
	int TILE_WIDTH,TILE_HEIGHT;
	int nbtilesX,nbtilesY;
	int nbtiles_width,nbtiles_height;
	SDL_Texture* tileset;
	TileProp* props;
	tileindex** schema;
	int xscroll,yscroll;
    int screen_width,screen_height;
    double delta_x;
} Map;

app_struct* 		initializeApp(void);

/******** PLAYER ***************************************************************************************/
player_struct* 		initializePlayer(app_struct*);
void 				displayPlayer(SDL_Renderer *, Map *, player_struct *, Uint32 ticks, bool gravity);

/******** TERRAIN **************************************************************************************/
terrain_struct* 	initializeTerrain(app_struct *app);
int 				loadBmp(app_struct *, terrain_struct *, char *, int);
void 				initializeBackground(terrain_struct *);
void 				displayTerrain(SDL_Renderer*, terrain_struct*, Uint32);
int 				getRandomY();

/******** MAP ******************************************************************************************/
Map* 				loadMap(app_struct *app, const char* path);
int 				moveMap(app_struct *app, Map *map, Uint32 ticks, bool player_blocked);
int 				displayMap(app_struct *app, Map* map);
int 				freeMap(Map* map);

/******** EXIT *****************************************************************************************/
void 				exitSdl(app_struct *);
void 				freePlayer(player_struct *);
void 				freeTerrain(terrain_struct *);

/******** COLLISIONS ***********************************************************************************/
int 				collideGrounds(player_struct *player, SDL_Rect *dst, Map *);

#endif