#include "includes/common.h"
#include <string.h>

#define CACHE_SIZE 5000

void ErrorQuit(const char *error)
{
	puts(error);
	SDL_Quit();
	// system("pause");
	exit(-1);
}

SDL_Texture *loadTexture(app_struct *app, const char *path)
{
	SDL_Surface *tmp = NULL;
	SDL_Texture *texture = NULL;

	tmp = SDL_LoadBMP(path);
	if (tmp == NULL)
		ErrorQuit(SDL_GetError());
	if (!SHOW_ALPHA)
		SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 25, 225, 115));
	texture = SDL_CreateTextureFromSurface(app->renderer, tmp);
	if (texture == NULL)
		ErrorQuit(SDL_GetError());
	SDL_FreeSurface(tmp);
	return texture;
}

void loadMapTileSet(app_struct *app, FILE *F, Map *map)
{
	int numtile, i, j;
	char buf[CACHE_SIZE];
	char buf2[CACHE_SIZE];
	SDL_Rect query;

	fscanf(F, "%s", buf);
	map->tileset = loadTexture(app, buf);
	SDL_QueryTexture(map->tileset, NULL, NULL, &query.w, &query.h);
	fscanf(F, "%d %d", &map->nbtilesX, &map->nbtilesY);
	map->TILE_WIDTH = query.w / map->nbtilesX;
	map->TILE_HEIGHT = query.h / map->nbtilesY;
	map->props = malloc(map->nbtilesX * map->nbtilesY * sizeof(TileProp));

	for (j = 0, numtile = 0 ; j < map->nbtilesY ; j++)
	{
		for ( i = 0 ; i < map->nbtilesX ; i++, numtile++)
		{
			map->props[numtile].rect.w = map->TILE_WIDTH;
			map->props[numtile].rect.h = map->TILE_HEIGHT;
			map->props[numtile].rect.x = i * map->TILE_WIDTH;
			map->props[numtile].rect.y = j * map->TILE_HEIGHT;
			fscanf(F, "%s %s", buf, buf2);
			map->props[numtile].opaque = 1;
			if (strcmp(buf2, "sky") == 0)
				map->props[numtile].opaque = 0;
		}
	}
}

void loadMapLevel(FILE *F, Map *map)
{
	int i,j;
	fscanf(F, "%d %d", &map->nbtiles_width, &map->nbtiles_height);
	map->schema = malloc(map->nbtiles_width  * sizeof(tileindex*));
	for(i = 0; i < map->nbtiles_width; i++)
		map->schema[i] = malloc(map->nbtiles_height * sizeof(tileindex));

	for ( j = 0 ; j < map->nbtiles_height ; j++)
	{
		for ( i = 0 ; i < map->nbtiles_width; i++)
		{
			int tmp;
			fscanf(F, "%d", &tmp);
			if (tmp >= map->nbtilesX * map->nbtilesY)
				ErrorQuit("Level tile out of bound\n");
			map->schema[i][j] = tmp;
		}
	}
}


Map *loadMap(app_struct *app, const char *path)
{
	FILE *F;
	char buf[CACHE_SIZE];
	Map *map;

	F = fopen(path, "r");
	if (!F)
		ErrorQuit("File not found\n");
	map = malloc(sizeof(Map));

	do
	{
		fgets(buf, CACHE_SIZE, F);
		if (strstr(buf,  "#tileset"))
			loadMapTileSet(app, F, map);
		if (strstr(buf, "#level"))
			loadMapLevel(F, map);
	} while (strstr(buf, "#end") == NULL);
	fclose(F);
	map->xscroll = 0;
	map->yscroll = 0;
	map->delta_x = 0.0;
	map->screen_width = WINDOW_WIDTH;
	map->screen_height = WINDOW_HEIGHT;
	return map;
}

int moveMap(app_struct *app, Map *map, Uint32 ticks, bool player_blocked)
{
	static Uint32 last_update_ground = 0;
	Uint32 delta_grounds = ticks - last_update_ground;

	if (delta_grounds >= MAP_SPEED) {
		map->xscroll += MAP_MOVE + map->delta_x;
		if (player_blocked)
			map->delta_x = 0;
		else if (map->delta_x < MAP_MAX_DELTA)
			map->delta_x += MAP_DELTA_X;

	}
	// if (delta_grounds >= GROUND_SPEED) {
	// 	if (terrain->delta_x < 20)
	// 		terrain->delta_x++;
	// 	last_update_ground = ticks;
	// }


	// limitations
    if (map->xscroll<0) 
        map->xscroll=0; 
    if (map->yscroll<0) 
        map->yscroll=0; 
    if (map->xscroll>map->nbtiles_width*map->TILE_WIDTH-map->screen_width-1) 
        map->xscroll=map->nbtiles_width*map->TILE_WIDTH-map->screen_width-1; 
    if (map->yscroll>map->nbtiles_height*map->TILE_HEIGHT-map->screen_height-1) 
        map->yscroll=map->nbtiles_height*map->TILE_HEIGHT-map->screen_height -1;
}

int displayMap(app_struct *app, Map *map)
{
	int i,j;
	SDL_Rect dest;
	int numtile;
	int minx, maxx, miny, maxy;
	minx = map->xscroll / map->TILE_WIDTH - 1;
	miny = map->yscroll / map->TILE_HEIGHT - 1;
	maxx = (map->xscroll + map->screen_width) / map->TILE_WIDTH;
	maxy = (map->yscroll + map->screen_height) / map->TILE_HEIGHT;

	for ( i = minx ; i <= maxx ; i++)
	{
		for ( j = miny ; j <= maxy; j++)
		{
			if (i < 0 || i >= map->nbtiles_width || j < 0 || j >= map->nbtiles_height) // the screen goes out of the global map
				numtile = 0;
			else
				numtile = map->schema[i][j];
			if (map->props[numtile].opaque == 0)
				continue;
			dest.x = i * map->TILE_WIDTH - map->xscroll;
			dest.y = j * map->TILE_HEIGHT - map->yscroll;
			dest.w = map->TILE_WIDTH;
			dest.h = map->TILE_HEIGHT;
			SDL_RenderCopy(app->renderer, map->tileset, &map->props[numtile].rect, &dest);
		}
	}
	return 0;
}

int freeMap(Map *map)
{
	int i;
	SDL_DestroyTexture(map->tileset);
	for (i = 0 ; i < map->nbtiles_width ; i++)
		free(map->schema[i]);
	free(map->schema);
	free(map->props);
	free(map);
	return 0;
}
