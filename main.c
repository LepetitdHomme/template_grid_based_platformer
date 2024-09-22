#include "includes/common.h"

int main(int argc, char **argv)
{
	SDL_bool play = SDL_TRUE;
	app_struct *app = NULL;
	Map *map = NULL;
	player_struct *player = NULL;
	terrain_struct *terrain = NULL;

	if ((app = initializeApp()) == NULL)
		return -1;
	if ((player = initializePlayer(app)) == NULL) {
		exitSdl(app);
		return -1;
	}
	if ((terrain = initializeTerrain(app)) == NULL) {
		exitSdl(app);
		freePlayer(player);
		return -1;
	}
	map = loadMap(app, "assets/level2.txt");
	if (map == NULL) {
		printf("ERROR MAP\n");
		return -1;
	}
	bool gravity = true;
	SDL_Event event;
	Uint32 ticks;

	SDL_SetRenderDrawColor(app->renderer, 0, 153, 255, 255);
	while (play) {
		ticks = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					play = SDL_FALSE;
				else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (player->delta_y == 0) {
						gravity = (gravity) ? false : true;
					}
				}
			}
		}
		SDL_RenderClear(app->renderer);
		displayTerrain(app->renderer, terrain, ticks);
		moveMap(app, map, ticks, player->blocked);
		displayMap(app, map);
		displayPlayer(app->renderer, map, player, ticks, gravity);
		SDL_RenderPresent(app->renderer);
		SDL_Delay(20);
	}
	freeMap(map);
	freePlayer(player);
	freeTerrain(terrain);
	exitSdl(app);
	return 0;
}
