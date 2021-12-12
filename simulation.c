#include "simulation.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

/*
	LISTE DES TRUCS A FAIRE:

	arnaud: finir infos_projection et ajouter le calcul de la normale dans les polygones
	alain: finir la physique
	françois: non
*/

int const g = 9.81;

// structures UTILITAIRES

typedef struct InfoProjections {
	Vec2 plus_proche;
	bool projection_touche;
} InfoProjections;

// FONCTIONS UTILITAIRES ---------------------------------

// renvoie l'inverse de la racine carré de number en O(1)
// avec une précision de +ou- 1%
float Q_rsqrt( float number )
{
	// implémentation de l'algorithme racine carré inverse rapide O(1)
	// c.f. wikipedia "fast inverse squareroot"

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

float distance(Vec2 posA, Vec2 posB)
{
	return sqrt( pow(posA.x - posB.x,2) + pow(posA.y - posB.y,2) );
}

// renvoie la distance au carré entre a et b
float distance2( Vec2 a, Vec2 b )
{
	return pow((b.x - a.x), 2) + pow((b.y - a.y), 2);
}

float longueur( Vec2 p )
{
	return sqrt( p.x*p.x + p.y*p.y );
}

// renvoie la longueur au carré
float longueur2( Vec2 p )
{
	return p.x*p.x + p.y*p.y;
}

// normalise le vecteur vec
// <=> longueur(vec) = 1 & angle(vec) change pas
void normaliser_vec( Vec2 *vec )
{
	float l = longueur( *vec );
	vec->x /= l;
	vec->y /= l;
}

// meme chose que normaliser_vec
// mais avec l'algorithme de l'inverse rapide,
// c.f Q_rsqrt
void normaliser_vec_rapide( Vec2 *vec )
{
	float rc = Q_rsqrt( longueur2(*vec) );
	vec->x *= rc;
	vec->y *= rc;
}

// renvoie true si le point p est
// dans la boite aabb
bool aabb_point( Vec2 p, AABB aabb )
{
	return
		p.x > aabb.position.x && p.x < aabb.position.x + aabb.taille.x &&
		p.y > aabb.position.y && p.y < aabb.position.y + aabb.taille.y;
}

// renvoie les infos sur la projection
// reponse.projection_touche: si la droite d'équation y = p.x touche [ab]
// reponse.plus_proche: le point le plus proche de p sur [ab]
InfoProjections infos_projection( Vec2 a, Vec2 b, Vec2 p )
{

	bool projection_touche = 
		((a.y < p.y && b.y > p.y) || (b.y < p.y && a.y > p.y))
		&& !(b.x > p.x && a.x > p.x);

	float A = distance2(a, p);
	float B = distance2(b, p);
	float L = distance(a, b);

	float d = (B + L*L - A) / (2.0f * L);

	if( d < 0 ) d = 0.01f;
	if( d > L ) d = L;

	Vec2 ba = { a.x - b.x, a.y - b.y };

	normaliser_vec_rapide( &ba );

	InfoProjections rep =
	{
		{ b.x + d * ba.x, b.y + d * ba.y },
		projection_touche
	};

	return rep;
}

// renvoie le point le plus proche de p, qui est à l'exterieur de pg
Vec2 collision( Vec2 p, Polygone *pg )
{

	InfoProjections infos = infos_projection( pg->pts[0], pg->pts[pg->nb_points-1], p );

	// point le plus proche de p sur le polygone
	Vec2 proche = infos.plus_proche;
	// on utilise la distance au carré pour économiser du temps
	float dist2_mini = distance2(proche, p); 

	int total = (int)infos.projection_touche; // c.f. infos_projection
	for( int i = 0; i < pg->nb_points - 1; ++i )
	{
		infos = infos_projection( pg->pts[i], pg->pts[i+1], p );

		// récupère le point le plus proche
		float dist2 = distance2(infos.plus_proche, p );
		if( dist2 < dist2_mini )
		{
			dist2_mini = dist2;
			proche = infos.plus_proche;
		}

		total += (int)infos.projection_touche;
	}

	// si total est pair: pas de collision,
	// on renvoie le vecteur nul
	if( total % 2 == 0 )
	{
		proche.x = p.x;
		proche.y = p.y;
	}

	Vec2 diff = 
	{
		proche.x - p.x,
		proche.y - p.y
	};

	return diff;
}

// FONCTIONS D'INITIALISATION / SUPRESSIONS --------------

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
	corps->nb_points   = (sx+2)*(sy+2);

	corps->pts = (Point*)  malloc( sizeof(Point)   * corps->nb_points   );
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

	// longueur entre chaque points (utilisé pour initialiser l0)
	float tx = w / (float)(sx+1);
	float ty = h / (float)(sy+1);
	float txy = sqrt(tx*tx + ty*ty);

	Ressort r =
	{ 
		0, 0, (sx+1)*(sy+1),	tx
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

			r.l0 = txy;
			r.p2 = i+1 + (j+1)*(sx+2);
			corps->rst[id_r+1] = r;

			r.l0 = ty;
			r.p2 = i + (j+1)*(sx+2);
			corps->rst[id_r+2] = r;

			id_r += 3;

		}
		else if( j == sy + 1)
		{
			r.l0 = txy;
			r.p2 = i+1 + (j-1)*(sx+2);
			corps->rst[id_r+0] = r;

			r.l0 = tx;
			r.p2 = i+1 + (j)*(sx+2);
			corps->rst[id_r+1] = r;

			id_r += 2;
		}
		else
		{
			r.l0 = txy;
			r.p2 = i+1 + (j+1)*(sx+2);
			corps->rst[id_r+0] = r;

			r.l0 = txy;
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
		r.l0 = ty;

		corps->rst[id_r] = r;
		++id_r;
	}
/*
	for( int i = 0; i < corps->nb_points; i++ )
	{
		Point *p = &corps->pts[i];
		printf("%f %f\n", p->position.x, p->position.y);
	}

	for( int i = 0; i < id_r; i++ )
	{
		Ressort *r = &corps->rst[i];
		printf("%d %d\n", r->p1, r->p2);
	}
	*/

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
			if (fscanf(f,"%f %f ", &simulation->polygones[i].pts[j].x , &simulation->polygones[i].pts[j].y ) != 2) return false;
		}
		//crea_AABB
		simulation->polygones[i].collision = crea_aabb( simulation->polygones[i].pts , simulation->polygones[i].nb_points);
	}
	

	//recup les infos depuis le fichier pour la creation des champsVec
	if( fscanf(f,"%hd ", &simulation->nb_champs) != 1) return false;
	
	simulation->champs = (ChampsVec*)malloc( sizeof(ChampsVec) * simulation->nb_champs );

	//recup coordonnées / taille / force des champs vectoriels
	for (int i = 0; i < simulation->nb_champs; i++) 
	{
		ChampsVec *c = &simulation->champs[i];
		
		if( fscanf(f, "%f %f %f %f %f %f",
			&c->aabb.position.x,
			&c->aabb.position.y,
			&c->aabb.taille.x,
			&c->aabb.taille.y,
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

// SIMULATION --------------------------------------------


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

		// si le point est dans un champs de force, on ajoute la force
		// du champs au point.
		for( int j = 0; j < simulation->nb_champs; ++j )
		{
			if( aabb_point( simulation->corps.pts[i].position, simulation->champs[j].aabb) )
			{
				simulation->corps.pts[i].force.x += simulation->champs[j].force.x;
				simulation->corps.pts[i].force.y += simulation->champs[j].force.y;
			}
		}		
	}
	
	// calcul de la force...

	for( int i = 0; i < simulation->corps.nb_ressorts; ++i)
	{
		Ressort *r1 = &simulation->corps.rst[i] ;
		Point *p1 = &simulation->corps.pts[r1->p1];
		Point *p2 = &simulation->corps.pts[r1->p2];
		
		Vec2 ab = 
		{
			p2->position.x - p1->position.x,
			p2->position.y - p1->position.y,
		};

		Vec2 vab = 
		{
			p2->vitesse.x - p1->vitesse.x,
			p2->vitesse.y - p1->vitesse.y,
		};

		normaliser_vec_rapide(&ab);

		float lg = distance(p1->position, p2->position);
		float f = r1->k * (lg - r1->l0) * 20.0f;

		f += 1.0f * (ab.x * vab.x + ab.y * vab.y);
		f /= 2;


		p1->force.x += f * ab.x;
		p1->force.y += f * ab.y;

		p2->force.x -= f * ab.x;
		p2->force.y -= f * ab.y;

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
		
		for( int j = 0; j < simulation->nb_polygones; ++j )
		{
			Polygone *pg = &simulation->polygones[j];

			// première étape: on teste si chaque points est dans
			// l'une des boites du polygones ( très rapide, permet
			// de gagner du temps )
			if( aabb_point(p->position, pg->collision))
			{ // si c'est le cas, alors on va tester les collisions

				// le vecteur qui va de p vers le point le plus proche
				// du polygone si il y a collision, sinon le vecteur nul
				Vec2 diff = collision( p->position, pg );

				p->position.x += diff.x;
				p->position.y += diff.y;

				// TODO: sur le plus long terme, il faudrait connaitre
				// la normale au polygone.
				p->vitesse.x += diff.x;
				p->vitesse.y += diff.y;
				
			}
		}

	}

}