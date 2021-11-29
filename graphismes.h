#pragma once

#include "SDL.h"
#include <stdbool.h>

// initialie SDL
bool init_sdl( SDL_Window *win );

// détruit SDL
void supr_sdl( SDL_Window *win );

// affiche une point noir
void afficher_point( int epaisseur, int x, int y );

// affiche une ligne entre (x0, y0) et (x1, y1) avec la couleur "couleur"
// n -> noir, r -> rouge, v -> vert
void afficher_ligne( int epaisseur, char couleur, int x0, int y0, int x1, int y1 );