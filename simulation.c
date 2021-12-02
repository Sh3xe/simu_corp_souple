#include "simulation.h"

#include <stdio.h>


// x, y: position, w, h: taille, sx: subdivision en x, sy: subdivision en y
void crea_corps( Corps *c, float x, float y, float w, float h, int sx, int sy )
{

	c->nb_points = sx * sy;
	c->pts = malloc( sizeof(Point) * c->nb_points );
	
	for( int x = 0; x < sx; ++x)
	for( int y = 0; y < sy; ++y)
	{
		// TODO:
	}

}

void init_simulation( Simulation *simulation, const char *chemin )
{

	FILE *f = fopen( chemin, "r" );

	// récupération des informations depuis un fichier
	// et calcul des boites de collisions

	fclose( f );
}

void supr_simulation( Simulation *simulation )
{
	// gestion de la memoire avec free() sur tout les pointeurs allouées dynamiquement
}

void simuler_frame( Simulation *simulation, float dt )
{
	//TODO: alain
}