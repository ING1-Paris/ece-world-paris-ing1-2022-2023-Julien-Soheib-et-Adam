#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>


int init_allegro(void);
int in_game(void);
void init_bitmap(BITMAP ** fond, BITMAP ***perso);
int checking_coordonates(int *x, int *y);
void in_game_snake(void);

#define velocity 3

#define de_face 0
#define de_dos 3
#define de_profil_droite 2
#define de_profil_gauche 1

#define nombre_sprite_perso 4

#define taille_snake 10



#define true 1
#define false 0

typedef int bool;



#define haut 0
#define bas 2
#define droite 1
#define gauche 3

typedef struct node_snake{
    int x;
    int y;
    int orientation;

    struct node_snake *next;
}node_snake;