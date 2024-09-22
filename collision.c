#include "includes/common.h"

void collideWall(player_struct *player, SDL_Rect *dst, Map *map, int i, int j)
{
	int dst_top, dst_bot, dst_right, wall_top, wall_bot, wall_left, player_top, player_bot;

	dst_top = dst->y;
	dst_bot = dst->y + dst->h;
	dst_right = dst->x + dst->w;
	wall_top = j * map->TILE_HEIGHT;
	wall_bot = j *  map->TILE_HEIGHT + map->TILE_HEIGHT;
	wall_left = i * map->TILE_WIDTH - map->xscroll;
	player_top = player->dst.y;
	player_bot = player->dst.y + player->dst.h;

	// if touches the left of a tie with its right, but tile is not below or above him
	if (dst_right > wall_left) {
		if (wall_top >= player_bot) {
				player->blocked = false;
				return;
		} else if (wall_bot <= player_top) {
			player->blocked = false;
			return;
		} else {
			player->blocked = true;
		}
	}
	else
		player->blocked = false;
}

int collideGrounds(player_struct *player, SDL_Rect *player_dest, Map *map)
{
	int xmin,xmax,ymin,ymax,i,j,indicetile;
	Map *m = map;

	if (player_dest->x < 0 || (player_dest->x + player_dest->w -1) >= (m->nbtiles_width * m->TILE_WIDTH) ||
		player_dest->y < 0 || (player_dest->y + player_dest->h -1) >= (m->nbtiles_height * m->TILE_HEIGHT))
		return 1;

	xmin = (map->xscroll + player_dest->x) / m->TILE_WIDTH;
	ymin = player_dest->y / m->TILE_HEIGHT;
	xmax = (map->xscroll + player_dest->x + player_dest->w -1) / m->TILE_WIDTH;
	ymax = (player_dest->y + player_dest->h -1) / m->TILE_HEIGHT;
	// loops through Xs first to detect wal collision before returning if ground/ceiling colision
	for( j = ymin ; j <= ymax ; j++)
	{
		for ( i = xmin ; i <= xmax ; i++)
		{
			indicetile = m->schema[i][j];
			if (m->props[indicetile].opaque == 1) {
				collideWall(player, player_dest, map, i, j);
				return 1;
			}
		}
	}
	player->blocked = false;
	return 0;
}
