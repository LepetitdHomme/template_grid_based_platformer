#include "includes/common.h"
#include <time.h>

// SDL_QueryTexture(terrain.texture, NULL, NULL, &terrain.original.w, &terrain.original.h);

terrain_struct* initializeTerrain(app_struct *app)
{
	static terrain_struct terrain;
	if (loadBmp(app, &terrain, "assets/city.bmp", 0) < 0)
		return NULL;
	
	terrain.delta_x = 0;
	initializeBackground(&terrain);
	return &terrain;
}

int loadBmp(app_struct *app, terrain_struct *terrain, char *path, int x)
{
	SDL_Surface *tmp = SDL_LoadBMP(path);
	bool error = false;

	if (tmp == NULL) {
		printf("Error SDL_LoadBMP: %s\n", SDL_GetError());
		return -1;
	}
	SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 25, 225, 115));
	switch(x) {
		case 0:
			terrain->background = SDL_CreateTextureFromSurface(app->renderer, tmp);
			if (terrain->background == NULL)
				error = true;
			break;
	}
	SDL_FreeSurface(tmp);
	if (error) {
		printf("Error SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
		return -1;
	}
	return 0;
}

void initializeBackground(terrain_struct *terrain)
{
	// first background
	terrain->background1.x = 0;
	terrain->background1.y = 0;
	terrain->background1.w = WINDOW_WIDTH;
	terrain->background1.h = WINDOW_HEIGHT;
	// second background
	terrain->background2.x = WINDOW_WIDTH;
	terrain->background2.y = 0;
	terrain->background2.w = WINDOW_WIDTH;
	terrain->background2.h = WINDOW_HEIGHT;
}

int getRandomY()
{
	srand( time( NULL ) );
	int random = 0;
	random = rand() % 50;
	return random;
}

// static int get_x_diff(ground_struct *current)
// {
// 	int x;
// 	if(current == NULL)
// 		return 0;
// 	x = -current->dst_top.x - (WINDOW_WIDTH / 2);
// 	//printf("x: %d\n", x);
// 	return x;
// }

void displayTerrain(SDL_Renderer *renderer, terrain_struct *terrain, Uint32 ticks)
{
	static Uint32 last_update_background = 0;
	Uint32 delta_backgrounds = ticks - last_update_background;


	/************************** BACKGROUNDS ******************************/
	if (delta_backgrounds >= BACKGROUND_SPEED) {
		if (terrain->background2.x <= BACKGROUND_MOVE) {
			terrain->background1.x = 0;
			terrain->background2.x = WINDOW_WIDTH;
		} else {
			terrain->background1.x -= BACKGROUND_MOVE;
			terrain->background2.x -= BACKGROUND_MOVE;
		}
		last_update_background = ticks;
	}
	SDL_RenderCopy(renderer, terrain->background, NULL, &terrain->background1);
	SDL_RenderCopy(renderer, terrain->background, NULL, &terrain->background2);

	/************************** GROUNDS **********************************/
	// while (current != NULL) {
	// 	if (delta_grounds >= GROUND_SPEED) {
	// 		if (current->dst_top.x + current->dst_top.w < 0) {
	// 			x_diff = get_x_diff(current);
	// 			current->dst_top.x = WINDOW_WIDTH + (WINDOW_WIDTH / 2) - x_diff;
	// 			current->dst_top.y = 75 + getRandomY();
	// 			current->dst_down.y = WINDOW_HEIGHT - 190 + getRandomY();
	// 			current->dst_down.x = WINDOW_WIDTH + (WINDOW_WIDTH / 2) - x_diff;
	// 		}
	// 		current->dst_top.x = current->dst_top.x - 1 - terrain->delta_x;
	// 		current->dst_down.x = current->dst_down.x - 1 - terrain->delta_x;
	// 	}
	// 	SDL_RenderCopy(renderer, terrain->ground, NULL, &current->dst_down);
	// 	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
	// 	SDL_RenderCopyEx(renderer, terrain->ground, NULL, &current->dst_top, 180, NULL, flip);
	// 	current = current->next;
	// }
	// if (delta_grounds >= GROUND_SPEED) {
	// 	if (terrain->delta_x < 20)
	// 		terrain->delta_x++;
	// 	last_update_ground = ticks;
	// }
}
