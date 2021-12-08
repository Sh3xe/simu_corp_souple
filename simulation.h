#pragma once

#include <stdbool.h>
#include <stdlib.h>

// structures de données --------------------------------------

// represente un point ou un vecteur mathématique
typedef struct Vec2
{
	float x, y;
} Vec2;

// represente une boite de collisison
typedef struct AABB
{
	Vec2 position;
	Vec2 taille;
} AABB;

// un points de notre simulation
typedef struct Point
{
	Vec2 position;
	Vec2 vitesse;
	Vec2 force;
	float masse;
} Point;

// un Ressort de notre simulation
typedef struct Ressort
{
	short p1, p2;
	float k;
	float l0;
} Ressort;

// polygone statique
typedef struct Polygone
{
	short nb_points;
	Vec2 *pts;
	AABB collision;
} Polygone;

// champ vectoriel
typedef struct ChampsVec
{
	Vec2 force;
	AABB aabb;
} ChampsVec;

// corp souple (ce qu'on va simuler)
typedef struct Corps
{
	short nb_points;
	Point *pts;

	short nb_ressorts;
	Ressort *rst;

} Corps;

// informations sur la simulation
typedef struct Simulation
{
	Corps corps;

	short nb_polygones;
	Polygone *polygones;

	short nb_champs;
	ChampsVec *champs;

} Simulation;


// fonctions --------------------------------------------------

// initialise un objet Simulation depuis un fichier chemin
// (const char* est une chaine de charactères)
bool init_simulation( Simulation *simulation, const char *chemin );

// libère la mémoire
void supr_simulation( Simulation *simulation );

// simule une frame de la simulation
// dt est le temps depuis la dernière frame en secondes
void simuler_frame( Simulation *simulation, float dt );