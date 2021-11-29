#include "simulation.h"
#include "graphismes.h"

// affichage de la simulation
void affichage( Simulation *simulation )
{
	
	for( int i = 0; i < simulation->nb_champs; ++i)
	{
		// affichage des champs vectoriels
		ChampsVec *c = &simulation->champs[i];

		int xmin = (int)c->position.x;
		int ymin = (int)c->position.y;

		int xmax = (int)(c->position.x + c->taille.x);
		int ymax = (int)(c->position.y + c->taille.y);

		afficher_ligne( 1, 'v',
			xmin, ymin, xmin, ymax
		);

		afficher_ligne( 1, 'v',
			xmin, ymax, xmax, ymax
		);

		afficher_ligne( 1, 'v',
			xmax, ymax, xmax, ymin
		);

		afficher_ligne( 1, 'v',
			xmax, ymin, xmin, ymin
		);
	}

	for( int i = 0; i < simulation->nb_polygones; ++i)
	{
		// affichage des polygones
		Polygone *p = &simulation->polygones[i];

		afficher_ligne( 5, 'n',
			(int)p->pts[0].x, (int)p->pts[0].y,
			(int)p->pts[1].x, (int)p->pts[1].y
		);

		for( int i = 1; i < p->nb_points; ++i )
		{
			afficher_ligne( 5, 'n',
				(int)p->pts[i-1].x, (int)p->pts[i-1].y,
				(int)p->pts[i].x, (int)p->pts[i].y
			);
		}

	}

	for( int i = 0; i < simulation->corp.nb_points; ++i)
	{
		// affichage du corp souple

		PointNewton *p = &simulation->corp.pts[i];

		afficher_point( 5, (int)p->position.x, (int)p->position.y );

		for( int j = 0; j < VOISINS_MAX; ++j ) 
		{

			if( p->voisins[j] == -1 ) break;

			PointNewton *v = &simulation->corp.pts[ p->voisins[j] ];

			afficher_ligne( 2, 'r',
				(int)p->position.x, (int)p->position.y,
				(int)v->position.x, (int)v->position.y
			);

		}

	}

}

int main()
{
	// INITIALISATION SDL
	SDL_Window *window;

	if( !init_sdl( window ) ) 
	{
		printf("Erreur: impossible d'initialiser SDL\n");
		return -1;
	}

	// SIMULATION
	Simulation simulation;
	init_simulation( &simulation, "./niveau.txt");

	bool fini = false;
	float dt = 0.01f;

	// BOUCLE DE LA SIMULATION
	while( !fini )
	{
		// GESTION ENTREES SDL
		SDL_Event event;
		while( SDL_PollEvent(&event) ) {
			if(event.type == SDL_QUIT)
				fini = true;
		}

		// LOGIQUE APPLICATION
		//simuler_frame( &simulation, 1.0f / 60.0f );
		//affichage( &simulation );

		// CALCUL DT
		

	}

	supr_simulation( &simulation );
	supr_sdl( window );
	
	return 0;
}