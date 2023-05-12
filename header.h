#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include <math.h>


#include "menu.h"



#define velocity 1

#define de_face 0
#define de_dos 3
#define de_profil_droite 2
#define de_profil_gauche 1

#define nombre_sprite_perso 4

#define taille_snake 10



#define true 1
#define false 0

typedef int bool;



#define haut 3
#define bas 1
#define droite 0
#define gauche 2
#define sans_valeur 10

#define snake_speed_base 0.2
#define snake_speed_mid 0.4
#define snake_speed_high 0.2

typedef struct node_snake{
    float x;
    float y;
    int orientation;

    struct node_snake *next;
}node_snake;

typedef struct pomme
{
    int x;
    int y;
}pomme;

typedef struct player
{
    char* name;
    int tickets;    
}player;



int init_allegro(void);
int in_game(void);
void init_bitmap(BITMAP ** fond, BITMAP ***perso);
int checking_coordonates(int *x, int *y);
void in_game_snake(void);
void affichage_snake(BITMAP** buffer, node_snake* tete,BITMAP** sprites_serpent);
void update_orienation_snake(node_snake *actuel);
void update_coordonate_snake(node_snake *actuel, int *orientation);
int check_mort_snake(node_snake *tete);
void nouvelle_coordonees(pomme* actuel);
int check_pomme(node_snake *tete, pomme* my_pomme);
void affichage_ath(BITMAP** buffer, player* joueurs);
void setup_player(player* joueurs);


int is_integer(double number) {
    return floor(number) == number && ceil(number) == number;
}