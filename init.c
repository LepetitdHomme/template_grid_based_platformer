#include "includes/common.h"

app_struct* initializeApp(void){
	static app_struct app;

	if (0 != SDL_Init(SDL_INIT_VIDEO)){
		printf("Error SDL_Init: %s\n", SDL_GetError());
		return NULL;
	}
	app.window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (app.window == NULL) {
		printf("Error SDL_CreateWindow: %s\n", SDL_GetError());
		return NULL;
	}
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if(app.renderer == NULL) {
        printf("Error SDL_CreateRenderer : %s", SDL_GetError());
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return NULL;
    }
    return &app;
}
