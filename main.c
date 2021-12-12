#include "simulation.h"
#include "graphismes.h"

const float ips = 60.0f;

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
	if ( !init_simulation( &simulation, "./donnees.txt"))
	{
		printf("Impossible d'initialier la simulation");
		return -1;
	}

	bool fini = false;
	float dt = 0.0001;
	uint64_t t0, t1;
	uint64_t frames = 0;
	float timer = 0.0f, temps_total = 0.0f; // utilisé pour afficher des informations toutes les secondes

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
		simuler_frame( &simulation, 0.005 );
		affichage( &contexte, &simulation );

		// calcul dt
		t0 = t1;
		t1 = SDL_GetPerformanceCounter();

		dt = (float)((t1 - t0)) / (float)SDL_GetPerformanceFrequency();

		// limitations des ips
		if( dt < (1.0f / ips) )
		{
			int ms = (1.0f / ips) * 1000 - dt * 1000;
			SDL_Delay( ms );
		}

		if( timer > 1.5f )
		{
			timer = 0.0f;
			//printf("%f\n", frames / temps_total);
		}

		timer += dt;
		temps_total += dt;
		++frames;
	}

	supr_sdl( &contexte );
	supr_simulation( &simulation );
	
	return 0;
}