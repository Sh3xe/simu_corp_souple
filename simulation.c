#include "simulation.h"

#include <stdio.h>

void init_simulation( Simulation *simulation, const char *chemin )
{
	// récupération des informations depuis un fichier
	// et calcul des boites de collisions
}

void supr_simulation( Simulation *simulation )
{
	// gestion de la memoire avec free() sur tout les pointeurs allouées dynamiquement
}

void simuler_frame( Simulation *simulation, float dt )
{

	// pour chaques points du corp souple.
	for( int i = 0; i < simulation->corp.nb_points; ++i)
	{

		// on initialise la force à 0
		float force = 0.0f;
		// on récupère le point
		PointNewton *p = &simulation->corp.pts[i];

		for( int j = 0; j < VOISINS_MAX; ++j )
		{
			// calcul de la force...


		}

		// calcul de l'accéleration, de la vitesse, gestion des collisions

	}
}