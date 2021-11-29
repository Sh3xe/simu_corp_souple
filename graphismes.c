#include "graphismes.h"

bool init_sdl( SDL_Window *win )
{
	if( SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0 )
		return false;

	win = SDL_CreateWindow(
		"simulation corp souple",
		0, 0, 1280, 720,
		SDL_WINDOW_RESIZABLE
	);

	if( win == NULL )
		return false;


	return true;
}

void supr_sdl( SDL_Window *win )
{
	SDL_DestroyWindow( win );
	SDL_Quit();
}

void afficher_point( int epaisseur, int x, int y )
{

}

void afficher_ligne( int epaisseur, char couleur, int x0, int y0, int x1, int y1 )
{

}