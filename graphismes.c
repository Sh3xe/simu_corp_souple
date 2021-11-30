#include "graphismes.h"

bool init_sdl( ContexteSDL *contexte )
{
	if( SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0 )
		return false;

	contexte->window = SDL_CreateWindow(
		"simulation corp souple",
		0, 0, 1280, 720,
		SDL_WINDOW_RESIZABLE
	);

	if( contexte->window == NULL )
		return false;

	contexte->renderer = SDL_CreateRenderer( contexte->window, -1, 0);
	contexte->video_mode = SDL_GetWindowSurface( contexte->window );

	if( !contexte->video_mode ) return false;

	return true;
}

void supr_sdl( ContexteSDL *contexte )
{
	SDL_DestroyRenderer( contexte->renderer );
	SDL_DestroyWindow( contexte->window );
	SDL_Quit();
}