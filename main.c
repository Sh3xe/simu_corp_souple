#include "simulation.h"

#include "SDL.h"

void affichage( Simulation *simulation )
{
	// TODO: affichage
}

int main()
{
	Simulation simulation;
	init_simulation( &simulation, "./niveau.txt");

	bool fini = false;
	float dt = 0.01f;

	while( !fini ) {


		// GESTION ENTREES SDL
		fini = true;

		// LOGIQUE APPLICATION
		simuler_frame( &simulation, 1.0f / 60.0f );
		affichage( &simulation );

		// CALCUL DT

	}

	supr_simulation( &simulation );
	
	return 0;
}