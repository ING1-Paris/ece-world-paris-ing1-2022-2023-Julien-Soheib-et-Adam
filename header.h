#include <stdio.h>
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

#define taille_snake 20
