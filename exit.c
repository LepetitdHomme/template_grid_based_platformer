#include "includes/common.h"

void exitSdl(app_struct *app)
{
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	SDL_Quit();
}

void freePlayer(player_struct *player)
{
	SDL_DestroyTexture(player->texture);
}

void freeTerrain(terrain_struct *terrain)
{
	SDL_DestroyTexture(terrain->background);
}
