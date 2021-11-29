#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define VOISINS_MAX 8

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
typedef struct PointNewton
{
	Vec2 position;
	Vec2 vitesse;
	float masse;
	short voisins[VOISINS_MAX];
} PointNewton;

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
	Vec2 position;
	Vec2 taille;
} ChampsVec;

// corp souple (ce qu'on va simuler)
typedef struct CorpSouple
{
	short nb_points;
	PointNewton *pts;
} CorpSouple;

// informations sur la simulation
typedef struct Simulation
{
	CorpSouple corp;

	short nb_polygones;
	Polygone *polygones;

	short nb_champs;
	ChampsVec *champs;

} Simulation;


// fonctions --------------------------------------------------

// initialise un objet Simulation depuis un fichier chemin
// (const char* est une chaine de charactères)
void init_simulation( Simulation *simulation, const char *chemin );

// libère la mémoire
void supr_simulation( Simulation *simulation );

// simule une frame de la simulation
// dt est le temps depuis la dernière frame en secondes
void simuler_frame( Simulation *simulation, float dt );

