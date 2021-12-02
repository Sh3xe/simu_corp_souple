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

	for( int i = 0; i < simulation->corps.nb_ressors; ++i)
	{
		// affichage du corps souple

		Ressort *r = &simulation->corps.ressorts[i];

		Point *p1 = &simulation->corps.pts[r->p1];
		Point *p2 = &simulation->corps.pts[r->p2];

		SDL_RenderDrawPoint( contexte->renderer, (int)p1->position.x, (int)p1->position.x );
		SDL_RenderDrawPoint( contexte->renderer, (int)p2->position.x, (int)p2->position.x );

		SDL_RenderDrawLine( contexte->renderer,
			(int)p1->position.x, (int)p1->position.x,
			(int)p2->position.x, (int)p2->position.x
		);

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

	bool fini = false;
	float dt = 0.01f;
	uint64_t t0, t1;

	t0 = SDL_GetPerformanceCounter() - 1;
	t1 = SDL_GetPerformanceCounter();

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
		simuler_frame( &simulation, dt );
		affichage( &contexte, &simulation );


		// calcul dt
		t0 = t1;
		t1 = SDL_GetPerformanceCounter();

		dt = (float)((t1 - t0) * 1000.0f) / (float)SDL_GetPerformanceFrequency();
	}

	supr_simulation( &simulation );
	supr_sdl( &contexte );
	
	return 0;
}