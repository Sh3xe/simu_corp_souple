#include "simulation.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int const g = 9.81;

// renvoie true s'il y a collision entre point et polygone
bool collision( Polygone *polygone, Vec2 point )
{

	return false;
}

// renvoie le plus le plus proche de p sur le segment [ab]
Vec2 projection_point( Vec2 a, Vec2 b, Vec2 p)
{
	Vec2 v = {0.0f, 0.0f};

	return v;
}

// renvoie le point le plus proche de p sur le segment [ab]
Vec2 pt_plus_proche( Polygone *polygone, Vec2 point ) {

	for( int i = 0; i < polygone->nb_points; ++i )
	{
		
	}

	return point;

}

// renvoie l'inverse de la racine carré de number en O(1)
float Q_rsqrt( float number )
{
	// implémentation de l'algorithme racine carré inverse rapide O(1)
	// c.f. wikipedia

	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? pas compris
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration

	return y;
}

float distance(Vec2 *posA, Vec2 *posB)
{
	return sqrt( pow(posA->x - posB->x,2) + pow(posA->y - posB->y,2) );
}

// créer une boite assès grande pour contenir tout les points de pts
AABB crea_aabb(Vec2 *pts, int taille)
{
	assert( taille > 1 );

	AABB boite =
	{
		pts[0],
		pts[0]
	};

	for(int i = 0; i < taille; i++)
	{
		if( pts[i].x < boite.position.x )
			boite.position.x = pts[i].x;
			
		if( pts[i].x > boite.position.x + boite.taille.x )
			boite.taille.x = pts[i].x - boite.position.x;
			
		if( pts[i].y < boite.position.y )
			boite.position.y = pts[i].y;
			
		if( pts[i].y > boite.position.y + boite.taille.y )
			boite.taille.y = pts[i].y - boite.position.y;
	}
	
	return boite;
}

// créer un corps à partir des informations
void crea_corps(
	Corps *corps,
	float x, float y,
	float w, float h,
	int sx, int sy )
{
	// initialisation de la mémoire
	corps->nb_ressorts = 4*(sx+1)*(sy+1) + sx + sy + 2;
	corps->nb_points  = (sx+2)*(sy+2);

	corps->pts = (Point*)malloc( sizeof(Point) * corps->nb_points );
	corps->rst = (Ressort*)malloc( sizeof(Ressort) * corps->nb_ressorts );

	// initialisation des points
	for( int i = 0; i <= sx + 1; ++i )
	for( int j = 0; j <= sy + 1; ++j )
	{

		float px = x + ((float)(i) / (float)(sx+1)) * w ;
		float py = y + ((float)(j) / (float)(sy+1)) * h ;		

		Point p =
		{
			{px  , py  },
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			0.02f
		};

		corps->pts[i + j * (sy+2)] = p;
	}

	// initialisation des ressorts
	int id_r = 0;

	// longueure entre chaque points (utilisé pour initialiser l0)
	float tx = w / (float)sx;
	float ty = h / (float)sy;

	Ressort r =
	{ // TODO: tatonner pour trouver une valeure qui va (pour l0)
		0, 0, 1.0f,	tx
	};

	for( int i = 0; i <= sx; ++i     )
	for( int j = 0; j <= sy + 1; ++j )
	{
		r.p1 = i + (j)*(sx+2);

		if( j == 0)
		{
			r.l0 = tx;
			r.p2 = i+1 + (j)*(sx+2);
			corps->rst[id_r+0] = r;

			r.l0 = sqrt(tx*tx + ty*ty);
			r.p2 = i+1 + (j+1)*(sx+2);
			corps->rst[id_r+1] = r;

			r.l0 = ty;
			r.p2 = i + (j+1)*(sx+2);
			corps->rst[id_r+2] = r;

			id_r += 3;

		}
		else if( j == sy + 1)
		{
			r.l0 = tx;
			r.p2 = i+1 + (j-1)*(sx+2);
			corps->rst[id_r+0] = r;

			r.l0 = tx;
			r.p2 = i+1 + (j)*(sx+2);
			corps->rst[id_r+1] = r;

			id_r += 2;
		}
		else
		{
			r.l0 = tx;
			r.p2 = i+1 + (j+1)*(sx+2);
			corps->rst[id_r+0] = r;

			r.l0 = tx;
			r.p2 = i+1 + (j-1)*(sx+2);
			corps->rst[id_r+1] = r;

			r.l0 = tx;
			r.p2 = i+1 + (j)*(sx+2);
			corps->rst[id_r+2] = r;

			r.l0 = tx;
			r.p2 = i + (j+1)*(sx+2);
			corps->rst[id_r+3] = r;

			id_r += 4;
		}
	}

	for( int j = 0; j <= sy; ++j )
	{
		r.p1 = sx+1 + (j)*(sx+2);
		r.p2 = sx+1 + (j+1)*(sx+2);
		r.l0 = tx;

		corps->rst[id_r] = r;
		++id_r;
	}

}

// libère la mémoire pour l'objet Corps
void supr_corps( Corps *corps )
{
	free(corps->rst);
	free(corps->pts);
}

bool init_simulation( Simulation *simulation, const char *chemin ) {

	FILE *f;
	f = fopen(chemin,"r") ;
	
	

	if( f == NULL ) return false;

	//recup les infos depuis le fichier pour la creation du corp 
	float x, y, w, h;
	if( fscanf(f,"%f %f %f %f", &x, &y, &w, &h) != 4) return false;

	int sx, sy;
	if( fscanf(f,"%d %d", &sx, &sy) != 2 ) return false;
	
	//creation du corp
	crea_corps(&simulation->corps, x, y, w, h, sx, sy);

	//recup les infos depuis le fichier pour la creation des polygones
	if( fscanf(f,"%hd", &simulation->nb_polygones) != 1) return false;

	// allocation de la mémoire pour les polygones
	simulation->polygones = (Polygone*)malloc( sizeof(Polygone) * simulation->nb_polygones );

	//recup nb point par polygone
	for (int i = 0; i < simulation->nb_polygones; i++) 
	{
		short nb_pts = 0;
		if( fscanf(f,"%hd", &nb_pts) != 1) return false;		
		simulation->polygones[i].nb_points = nb_pts;
		simulation->polygones[i].pts = (Vec2*)malloc( sizeof(Vec2) * nb_pts );
	}

	//recup coordonnées des points des polygones
	for (int i = 0; i < simulation->nb_polygones; i++) 
	{
		for (int j = 0; j < simulation->polygones[i].nb_points; j++)
		{
			fscanf(f,"%f %f ", &simulation->polygones[i].pts[j].x , &simulation->polygones[i].pts[j].y );
		}
		//crea_AABB
		simulation->polygones[i].collision = crea_aabb( simulation->polygones[i].pts , simulation->polygones[i].nb_points);
	}
	

	//recup les infos depuis le fichier pour la creation des champsVec
	fscanf(f,"%hd ", &simulation->nb_champs);
	
	simulation->champs = (ChampsVec*)malloc( sizeof(ChampsVec) * simulation->nb_champs );

	//recup coordonnées / taille / force des champs vectoriels
	for (int i = 0; i < simulation->nb_champs; i++) 
	{
		ChampsVec *c = &simulation->champs[i];
		
		if( fscanf(f, "%f %f %f %f %f %f",
			&c->position.x,
			&c->position.y,
			&c->taille.x,
			&c->taille.y,
			&c->force.x,
			&c->force.y) != 6) return false;
	}

  fclose(f);
	return true;
}	

void supr_simulation( Simulation *simulation )
{
	supr_corps( &simulation->corps );
	free( simulation->champs );

	for( int i = 0; i < simulation->nb_polygones; ++i )
		free( simulation->polygones[i].pts );
		
	free( simulation->polygones );

}

void simuler_frame( Simulation *simulation, float dt )
{
	//printf("%f %f\n", simulation->corps.pts[0].position.x, simulation->corps.pts[0].position.y);

	// pour chaques points du corp souple.
	for( int i = 0; i < simulation->corps.nb_points; ++i)
	{
		// on initialise la force à 0
		simulation->corps.pts[i].force.x = 0 ;
		// on ajoute la force interne de pesanteur
		simulation->corps.pts[i].force.y = simulation->corps.pts[i].masse * g;		
	}
	
	// calcul de la force...

	for( int i = 0; i < simulation->corps.nb_ressorts; ++i)
	{
		Ressort *r1 = &simulation->corps.rst[i] ;
		Point *p1 = &simulation->corps.pts[r1->p1];
		Point *p2 = &simulation->corps.pts[r1->p2];
		float force = - r1->k * ( distance( &p1->position , &p2->position ) - r1->l0 ) ;
		float angle1 = atan2( p1->position.x - p2->position.x , p1->position.y - p2->position.y );
		float angle2 = atan2( p2->position.x - p1->position.x , p2->position.y - p1->position.y );
	}
	
	// calcul de l'accéleration, de la vitesse, gestion des collisions
	for( int i = 0; i < simulation->corps.nb_points; ++i )
	{
		Point *p = &simulation->corps.pts[i];
		
		// méthode d'Euler pour le calcul en temps réel
		// de la vitesste / position / collision
		p->vitesse.x += (p->force.x / p->masse) * dt;
		p->vitesse.y += (p->force.y / p->masse) * dt;
		p->position.x += p->vitesse.x * dt;
		p->position.y += p->vitesse.y * dt;

		// TODO:arnaud -- gérer les collisions
		
	}

}