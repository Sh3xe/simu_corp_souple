#pragma once

#include "simulation.h"
#include "SDL.h"
#include <stdbool.h>

typedef struct ContexteSDL {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *video_mode;
} ContexteSDL;

// initialie SDL
bool init_sdl( ContexteSDL *contexte );

// détruit SDL
void supr_sdl( ContexteSDL *contexte );

// affiche sur une fenetre la simulation
void affichage( ContexteSDL *contexte, Simulation *simulation );