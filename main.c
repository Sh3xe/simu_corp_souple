#include "simulation.h"

#include "SDL.h"


// toute la logique 
void logique( Simulation *simulation )
{
	simuler_frame( simulation, 1.0f / 60.0f );
}

void affichage( Simulation *simulation )
{
	// TODO: affichage
}

int main()
{
	Simulation simulation;
	init_simulation( &simulation, "./niveau.txt");

	bool fini = false;

	while( !fini ) {

		logique( &simulation );
		affichage( &simulation );

		// temporaire
		fini = true;

	}

	supr_simulation( &simulation );
	
	return 0;
}