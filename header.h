#include <stdio.h>
#include <allegro.h>


int init_allegro(void);
int in_game(void);
void init_bitmap(BITMAP ** fond, BITMAP ***perso);


#define velocity 1

#define de_face 0
#define de_dos 3
#define de_profil_droite 2
#define de_profil_gauche 1

#define nombre_sprite_perso 4
