#include "includes/common.h"

#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))
#define ABS(X) ((((X)<0)?(-(X)):(X)))

player_struct* initializePlayer(app_struct *app)
{
	static player_struct player;
	SDL_Surface *tmp = SDL_LoadBMP("assets/megaman.bmp");
	if (tmp == NULL) {
		printf("Error SDL_LoadBMP: %s\n", SDL_GetError());
		return NULL;
	}
	if (!SHOW_ALPHA)
		SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 25, 225, 115));
	player.texture = SDL_CreateTextureFromSurface(app->renderer, tmp);
	SDL_FreeSurface(tmp);
	if (player.texture == NULL) {
		printf("Error SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
		return NULL;
	}
	SDL_QueryTexture(player.texture, NULL, NULL, &player.original.w, &player.original.h);
	
	player.delta_y = 0;
	player.delta_x = 0;
	player.blocked = false;
	player.original.x = WINDOW_WIDTH / 10;
	player.original.y = WINDOW_HEIGHT / 10;
	player.original.w = 50;
	// player.original.h does not change
	
	player.src.x = 0;
	player.src.y = 0;
	player.src.w = 50;
	player.src.h = 40;
	
	player.dst.x = 100;
	player.dst.y = WINDOW_HEIGHT / 2 - player.original.w;
	player.dst.w = 60;
	player.dst.h = 60;

	return &player;
}

// static void move(player_struct *player, bool falls_down)
// {
// 	if (falls_down) {
// 		player->dst.y += player->delta_y;
// 	} else {
// 		player->dst.y = player->dst.y - player->delta_y;
// 	}
// 	if (player->delta_y < 20)
// 		player->delta_y++;
// }

int movePlayer(player_struct *player, Map *map, bool falls_down, int dx, int dy)
{
	SDL_Rect test;
	test = player->dst;
	if (falls_down)
		test.y += DEFAULT_Y_MOVE + dy;
	else
		test.y -= (DEFAULT_Y_MOVE + dy);
	if (collideGrounds(player, &test, map) == 0)
	{
		player->dst = test;
		return 1;
	}
	return 0;
}

int refineMove(player_struct *player, Map *map, bool falls_down, int dx, int dy)
{
	int i;
	int moved = 0;
	// for ( i = 0 ; i < ABS(dx) ; i++)
	// {
	// 	if (movePlayer(player, map, falls_down, SGN(dx), 0) == 0)
	// 		break;
	// }
	// if (ABS(dy) <= 1)
	// 	return moved;
	for ( i = 0 ; i < ABS(dy) ; i++)
	{
		if (movePlayer(player, map, falls_down, 0, SGN(dy)) == 0)
			break;
		else
			moved = 1;
	}
	return moved;
}

void animatePlayer(player_struct *player, int anim, int animY, int moved)
{
	if (player->blocked) {
		player->src.y = 0;
		player->src.x = 0;
	} else {
		if (moved) {
			//second row of sprite
			player->src.y = 53;
			player->src.h = 54;
			player->src.x = animY * player->original.w;
		}
		else {
			player->src.y = 0;
			player->src.h = 40;
			player->src.x = anim * player->original.w;
		}

	}
}

void displayPlayer(SDL_Renderer *renderer, Map *map, player_struct *player, Uint32 ticks, bool gravity)
{
	static Uint32 last_update_player = 0;
	int ground_collision;
	int animY = (ticks / 100) % 7;
	int anim = (ticks / 100) % 8;
	double angle;
	SDL_RendererFlip flip;
	int moved = 0;
	Uint32 delta_player = ticks - last_update_player;

	if (delta_player >= MAP_SPEED) {
		if (movePlayer(player, map, gravity, 0, player->delta_y) == 0) // if initial move attempt failed
		{
			if (refineMove(player, map, gravity, 0, player->delta_y) == 0) {
				// printf("didn't move");
				// if no smaller move was found/processed
				player->delta_y = 0;
			}
			else {
				// printf("moved slow");
				moved = 1;
			}
		} else {
			// printf("moved big");
			moved = 1;
		}
		if (moved == 1 && player->delta_y < 20)
			player->delta_y += 1;
		last_update_player = ticks;
		if (player->blocked) {
			player->dst.x = player->dst.x - (MAP_MOVE + map->delta_x);
		} else {
			if (player->dst.x < 100)
				player->dst.x++;
		}
	}
	animatePlayer(player,anim,animY, moved);

	if (gravity) {
		angle = 0;
		flip = SDL_FLIP_NONE;
	} else {
		angle = 180;
		flip = SDL_FLIP_HORIZONTAL;
	}
	SDL_RenderCopyEx(renderer, player->texture, &player->src, &player->dst, angle, NULL, flip);
}