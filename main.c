#include "header.h"


int main(int argc, char* _argv[]) {
    
    if(init_allegro()!=0)
        return 1;
    if(!argc)
        in_game();
    else if(atoi(_argv[1])==0)
        in_game_snake();
    
    allegro_exit();
    return 0;
}END_OF_MAIN();


int init_allegro(void){
    allegro_init();
    set_color_depth(desktop_color_depth());
    
    install_mouse();
    install_keyboard();
    
    
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0){
        allegro_message("Error while loading screen");
        allegro_exit();
        return 1;
    }
    
    show_mouse(screen);
    
    
    
    return 0;
}


int in_game(void){

    // INITIALISATION DU NECESSAIRE POUR LANCER LE JEU
    BITMAP *fond;
    BITMAP ***perso;
    BITMAP *buffer;

    int x = 0;
    int y = 0;
    int count = 0;
    bool playing = true;

    perso = (BITMAP***) malloc(sizeof(BITMAP**)*4);
    if(perso==NULL)
        return 1;

    init_bitmap(&fond,perso);

    
    

    buffer = create_bitmap(SCREEN_W,SCREEN_H);
    if(buffer==NULL)
        return 1;

    while(playing){

        blit(fond,buffer,x,y,0,0,fond->w,fond->h);

        if(key[KEY_RIGHT] && x<=fond->w-SCREEN_W){
            masked_blit(perso[de_profil_droite][count],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%nombre_sprite_perso;
            x+=velocity;
        }
        else if(key[KEY_LEFT] && x>=0){
            masked_blit(perso[de_profil_gauche][count],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%nombre_sprite_perso;
            x-=velocity;
        }
        else if(key[KEY_UP] && y>=0 ){
            masked_blit(perso[de_dos][count],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%nombre_sprite_perso;
            y-=velocity;
        }
        else if(key[KEY_DOWN] && y<= fond->h - SCREEN_H){
            masked_blit(perso[de_face][count],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%nombre_sprite_perso;
            y+=velocity;
        }
        else if(key[KEY_ESC])
            playing = false;
        else{
            masked_blit(perso[de_face][de_face],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
        }

        blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
        if(checking_coordonates(&x,&y)==0){
            //playing = false;
            in_game_snake();
        }
        
        
        
    }

    for(int i = 0;i<4;i++){
        for(int j =0;j<4;j++){
            destroy_bitmap(perso[i][j]);
        }
        free(perso[i]);
    }
    free(perso);
    destroy_bitmap(buffer);
    destroy_bitmap(fond);
    return 0;
}



// FONCTION POUR INITIALISER TOUTES LES BITMAPS NECESSAIRES
void init_bitmap(BITMAP ** fond, BITMAP ***perso){

    // INIT SPRITES PERSO

    BITMAP *buffer;

    for(int i = 0;i<nombre_sprite_perso;i++){
        *(perso+i) = (BITMAP**) malloc(sizeof(int*)*nombre_sprite_perso);
        if(*(perso+i)==NULL)
            return;
    }

    buffer = load_bitmap("perso_poke.bmp",NULL);
    
    if(buffer == NULL){
        allegro_message("Error while loading character sprite.");
        allegro_exit();
        return;
    }

    // PETIT BUG ICI A REGLER ??
    //blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);

    textout_centre_ex(screen,font,"APPUYEZ SUR UN BOUTON POUR COMMENCER A JOUER...",screen->w/2,screen->h/2,makecol(120,80,80),-1);
    readkey();
    //rest(2000);
    //readkey();

    for(int i = 0; i<4;i++){
        for(int j = 0;j<nombre_sprite_perso;j++){
            perso[i][j] = create_sub_bitmap(buffer,(j%nombre_sprite_perso)*48,(i%4)*64,48,64);
            if(perso[i][j] == NULL){
            allegro_message("Error while loading character sprites.");
            allegro_exit();
            return;
            }
        }
    }
       
    destroy_bitmap(buffer);

    // INIT FOND DE LA MAP

    BITMAP *loading_bmp;

    loading_bmp = load_bitmap("map.bmp",NULL);
    if(loading_bmp == NULL){
        allegro_message("Error while loading the map.");
        allegro_exit();
        return;
    }

    *fond = create_bitmap((loading_bmp->w)*2.5,(loading_bmp->h)*2.5);
    stretch_blit(loading_bmp, *fond, 0, 0, loading_bmp->w, loading_bmp->h, 0, 0, (*fond)->w, (*fond)->h);
    

    if(*fond == NULL){
        allegro_message("Error while loading the map.");
        allegro_exit();
        return;
    }


    destroy_bitmap(loading_bmp); 

}


// check si on arrive dans une zone de mini jeu
int checking_coordonates(int *x, int *y){

    // ici test pour l'instant

    if(*x>=140 && *x<=180 && *y<= 1185 && *y>=1150){
        //in_game_snake();
        //allegro_message("ZONE DE JEUX");
        *x = 0;
        *y = 0;
        return 0;
    }
    return 1;

}



void in_game_snake(void){
    // dans le jeu de snake

    BITMAP *fond_snake;
    BITMAP *tete_serpent;
    BITMAP *queue_serpent;
    BITMAP *corps_serpent;
    BITMAP *tempons;

    fond_snake = create_bitmap(SCREEN_W-200,SCREEN_H);


    // initialisation de la queue
    tempons = load_bitmap("queue_serpent.bmp",NULL);
    queue_serpent = create_sub_bitmap(tempons,0,0,(fond_snake->w/taille_snake),(fond_snake->h/taille_snake));
    destroy_bitmap(tempons);

    // initialisation de la tete
    tempons = load_bitmap("tete_serpent.bmp",NULL);
    tete_serpent = create_sub_bitmap(tempons,0,0,(fond_snake->w/taille_snake),(fond_snake->h/taille_snake));
    destroy_bitmap(tempons);

    // initialisation du corp
    tempons = load_bitmap("corps_serpent.bmp",NULL);
    tete_serpent = create_sub_bitmap(tempons,0,0,(fond_snake->w/taille_snake),(fond_snake->h/taille_snake));

    int color_vert_clair = makecol(86, 232, 47);
    int color_vert_fonce = makecol(0, 192, 180);

    

    for(int i = 0; i < taille_snake; i++){
        for(int j = 0; j < taille_snake ; j++){
            if((i+j)%2==0)
                rectfill(fond_snake,(fond_snake->w/taille_snake)*i,(fond_snake->h/taille_snake)*j,(fond_snake->w/taille_snake)*(i+1),(fond_snake->h/taille_snake)*(j+1),color_vert_clair);
            else
                rectfill(fond_snake,(fond_snake->w/taille_snake)*i,(fond_snake->h/taille_snake)*j,(fond_snake->w/taille_snake)*(i+1),(fond_snake->h/taille_snake)*(j+1),color_vert_fonce);
        }
    }


    blit(fond_snake,screen,0,0,100,0,fond_snake->w,fond_snake->h);


    // playing part
    BITMAP *buffer;

    bool playing = true;

    while(playing){
        if(key[KEY_ESC])
            playing = false;
    }


    destroy_bitmap(fond_snake);
}