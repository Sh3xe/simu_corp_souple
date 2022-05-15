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

	contexte->renderer = SDL_CreateRenderer( contexte->window, -1, SDL_RENDERER_ACCELERATED);
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

void ligne( ContexteSDL *contexte, float x0, float y0, float x1, float y1 )
{
	// on passe les coordonnées de [0, 10] x [0, 6] -> [0, 500] x [0, 300]
	x0 *= 50.0f;
	y0 *= 50.0f;
	x1 *= 50.0f;
	y1 *= 50.0f;

	SDL_RenderDrawLine( contexte->renderer,
		(int)x0, (int)y0, (int)x1, (int)y1
	);

}

//void point( Vec2 p )
//{
//
//}

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

		int xmin = (int)c->aabb.position.x;
		int ymin = (int)c->aabb.position.y;

		int xmax = (int)(c->aabb.position.x + c->aabb.taille.x);
		int ymax = (int)(c->aabb.position.y + c->aabb.taille.y);

		ligne( contexte,
			xmin, ymin, xmin, ymax
		);

		ligne( contexte,
			xmin, ymax, xmax, ymax
		);

		ligne( contexte,
			xmax, ymax, xmax, ymin
		);

		ligne( contexte,
			xmax, ymin, xmin, ymin
		);
	}


	SDL_SetRenderDrawColor( contexte->renderer, 255, 255, 255, 255);

	for( int i = 0; i < simulation->nb_polygones; ++i)
	{
		// affichage des polygones
		Polygone *p = &simulation->polygones[i];

		ligne( contexte,
			p->pts[0].x, p->pts[0].y,
			p->pts[p->nb_points-1].x, p->pts[p->nb_points-1].y
		);

		for( int i = 0; i < p->nb_points-1; ++i )
		{
			ligne( contexte,
				p->pts[i].x, p->pts[i].y,
				p->pts[i+1].x, p->pts[i+1].y
			);
		}

	}

	SDL_SetRenderDrawColor( contexte->renderer, 255, 0, 0, 255);

	// affichage du corps

	for( int i = 0; i < simulation->corps.nb_ressorts; ++i )
	{
		Ressort *r = &simulation->corps.rst[i];
		Point *p1 = &simulation->corps.pts[r->p1];
		Point *p2 = &simulation->corps.pts[r->p2];

		ligne( contexte,
			p1->position.x, p1->position.y,
			p2->position.x, p2->position.y
		);
	}

	SDL_RenderPresent( contexte->renderer );

}