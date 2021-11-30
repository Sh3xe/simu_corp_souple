#include "simulation.h"
#include "graphismes.h"

// affichage de la simulation
void affichage( ContexteSDL *contexte, Simulation *simulation )
{
	SDL_SetRenderDrawColor( contexte->renderer, 0, 0, 0, 255);
	SDL_RenderClear( contexte->renderer );
	
	SDL_SetRenderDrawColor( contexte->renderer, 0, 255, 0, 255);

	for( int i = 0; i < simulation->nb_champs; ++i)
	{
		// affichage des champs vectoriels
		ChampsVec *c = &simulation->champs[i];

		int xmin = (int)c->position.x;
		int ymin = (int)c->position.y;

		int xmax = (int)(c->position.x + c->taille.x);
		int ymax = (int)(c->position.y + c->taille.y);

		SDL_RenderDrawLine( contexte->renderer,
			xmin, ymin, xmin, ymax
		);

		SDL_RenderDrawLine( contexte->renderer,
			xmin, ymax, xmax, ymax
		);

		SDL_RenderDrawLine( contexte->renderer,
			xmax, ymax, xmax, ymin
		);

		SDL_RenderDrawLine( contexte->renderer,
			xmax, ymin, xmin, ymin
		);
	}


	SDL_SetRenderDrawColor( contexte->renderer, 255, 255, 255, 255);

	for( int i = 0; i < simulation->nb_polygones; ++i)
	{
		// affichage des polygones
		Polygone *p = &simulation->polygones[i];

		SDL_RenderDrawLine( contexte->renderer,
			(int)p->pts[0].x, (int)p->pts[0].y,
			(int)p->pts[1].x, (int)p->pts[1].y
		);

		for( int i = 1; i < p->nb_points; ++i )
		{
			SDL_RenderDrawLine( contexte->renderer,
				(int)p->pts[i-1].x, (int)p->pts[i-1].y,
				(int)p->pts[i].x, (int)p->pts[i].y
			);
		}

	}

	SDL_SetRenderDrawColor( contexte->renderer, 255, 0, 0, 255);

	for( int i = 0; i < simulation->corp.nb_points; ++i)
	{
		// affichage du corp souple

		PointNewton *p = &simulation->corp.pts[i];

		SDL_RenderDrawPoint( contexte->renderer, (int)p->position.x, (int)p->position.y );

		for( int j = 0; j < VOISINS_MAX; ++j ) 
		{

			if( p->voisins[j] == -1 ) break;

			PointNewton *v = &simulation->corp.pts[ p->voisins[j] ];

			SDL_RenderDrawLine( contexte->renderer,
				(int)p->position.x, (int)p->position.y,
				(int)v->position.x, (int)v->position.y
			);

		}

	}

	SDL_RenderPresent( contexte->renderer );
	SDL_UpdateWindowSurface( contexte->window );

}

int main()
{
	// INITIALISATION SDL
	ContexteSDL contexte;

	if( !init_sdl( &contexte ) ) 
	{
		printf("Erreur: impossible d'initialiser SDL\n");
		return -1;
	}

	// SIMULATION
	Simulation simulation;
	init_simulation( &simulation, "./niveau.txt");

	CorpSouple corp_tempo;
	corp_tempo.nb_points = 0;

	simulation.nb_champs = 0;
	simulation.nb_polygones = 0;
	simulation.corp = corp_tempo;

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
		affichage( &contexte, &simulation );

		

		// CALCUL DT
		

	}

	supr_simulation( &simulation );
	supr_sdl( &contexte );
	
	return 0;
}