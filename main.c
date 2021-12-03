#include "simulation.h"
#include "graphismes.h"

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

		dt = (float)((t1 - t0)) / (float)SDL_GetPerformanceFrequency();
	}

	return 0;
}