#include "header.h"


int main(int argc, char* _argv[]) {
    
    if(init_allegro()!=0)
        return 1;
    if(argc == 1)
        in_game();
    else if(atoi(_argv[1])==0)
        in_game_snake();
    else if(atoi(_argv[1])==1)
        menu();
    
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

    player* joueurs = (player*) malloc(sizeof(player)*2);
    setup_player(joueurs);
    

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

        //affichage_ath(&buffer,joueurs);
        blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
        if(checking_coordonates(&x,&y)==0){
            //playing = false;
            //in_game_snake();
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


// FONCTION POUR PARAMETRER LES JOUEURS

void setup_player(player* joueurs){
    char *nom = malloc(8);
    strcpy(nom,"ADAM");
    (joueurs)->name = nom;
    (joueurs)->tickets = 5;

    nom = malloc(8);
    strcpy(nom, "SAM");
    (joueurs+1)->name = nom;
    (joueurs+1)->tickets = 5;
}


// FONCTION QUI AFFICHE L'ATH
void affichage_ath(BITMAP** buffer, player* joueurs){
    textout_ex(*buffer,font,(joueurs)->name,SCREEN_W-90,50,makecol(255,255,255),-1);


    BITMAP *buffer_image = load_bitmap("ticket.bmp",NULL);
    BITMAP *ticket = create_bitmap(15,10);

    stretch_blit(buffer_image,ticket,0,0,buffer_image->w,buffer_image->h,0,0,ticket->w,ticket->h);

    for(int i = 0;i<(joueurs)->tickets;i++){
        masked_blit(ticket,*buffer,0,0,SCREEN_W-90+ticket->w*i,60,ticket->w,ticket->h);
    }

    textout_ex(*buffer,font,(joueurs+1)->name,SCREEN_W-90,90,makecol(255,255,255),-1);
    for(int i = 0;i<(joueurs+1)->tickets;i++){
        masked_blit(ticket,*buffer,0,0,SCREEN_W-90+ticket->w*i,100,ticket->w,ticket->h);
    }

    destroy_bitmap(ticket);

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

    //textout_centre_ex(screen,font,"APPUYEZ SUR UN BOUTON POUR COMMENCER A JOUER...",screen->w/2,screen->h/2,makecol(120,80,80),-1);
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

    readkey();

    // initialisation de la queue
    tempons = load_bitmap("queue_serpent.bmp",NULL);

    queue_serpent = create_bitmap(fond_snake->w/taille_snake,fond_snake->h/taille_snake);

    if(tempons==NULL)
        allegro_message("ERROR loading la queue");

    stretch_blit(tempons,queue_serpent,0,0,tempons->w,tempons->h,0,0,queue_serpent->w,queue_serpent->h);

    destroy_bitmap(tempons);

    // initialisation de la tete
    tempons = load_bitmap("tete_serpent.bmp",NULL);

    tete_serpent = create_bitmap(fond_snake->w/taille_snake,fond_snake->h/taille_snake);

    if(tempons==NULL)
        allegro_message("ERROR loading la tete");
    stretch_blit(tempons,tete_serpent,0,0,tempons->w,tempons->h,0,0,tete_serpent->w,tete_serpent->h);

    destroy_bitmap(tempons);

    // initialisation du corp
    tempons = load_bitmap("corps_serpent.bmp",NULL);

    corps_serpent = create_bitmap(fond_snake->w/taille_snake,fond_snake->h/taille_snake);

    if(tempons==NULL)
        allegro_message("ERROR loading le corps");

    stretch_blit(tempons,corps_serpent,0,0,tempons->w,tempons->h,0,0,tete_serpent->w,tete_serpent->h);

    destroy_bitmap(tempons);

    BITMAP* sprites_serpent[3] = {tete_serpent,corps_serpent,queue_serpent};

    

    int color_vert_clair = makecol(86, 232, 47);
    int color_vert_fonce = makecol(0, 192, 180);

    
    // impression de toute les tuiles sur le fond
    for(int i = 0; i < taille_snake; i++){
        for(int j = 0; j < taille_snake ; j++){
            if((i+j)%2==0)
                rectfill(fond_snake,(fond_snake->w/taille_snake)*i,(fond_snake->h/taille_snake)*j,(fond_snake->w/taille_snake)*(i+1),(fond_snake->h/taille_snake)*(j+1),color_vert_clair);
            else
                rectfill(fond_snake,(fond_snake->w/taille_snake)*i,(fond_snake->h/taille_snake)*j,(fond_snake->w/taille_snake)*(i+1),(fond_snake->h/taille_snake)*(j+1),color_vert_fonce);
        }
    }


   

    // playing part
    BITMAP *buffer = create_bitmap(SCREEN_W-200,SCREEN_H);
    BITMAP *double_buffer = create_bitmap(SCREEN_W,SCREEN_H);


    BITMAP *pomme_sprites = create_bitmap(fond_snake->w/taille_snake,fond_snake->h/taille_snake);
    clear_to_color(pomme_sprites,makecol(255,0,255));
    tempons = load_bitmap("pomme.bmp",NULL);

    stretch_blit(tempons,pomme_sprites,0,0,tempons->w,tempons->h,0,0,pomme_sprites->w,pomme_sprites->h);

     blit(fond_snake,buffer,0,0,100,0,fond_snake->w,fond_snake->h);

     masked_blit(queue_serpent,buffer,0,0,(buffer->w/taille_snake)*3.5,(buffer->h/taille_snake)*3,queue_serpent->w,queue_serpent->h);
     
    blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
    bool playing = true;

    node_snake* tete = malloc(sizeof(node_snake));

    tete->orientation = droite;
    tete->x = 2;
    tete->y = 1;
   { 
        node_snake *temporaire = malloc(sizeof(node_snake));
        temporaire->orientation = droite;
        temporaire->x=1;
        temporaire->y=1;
        tete->next = temporaire;

        temporaire = malloc(sizeof(node_snake));
        temporaire->orientation = droite;
        temporaire->x=0;
        temporaire->y=1;
        tete->next->next = temporaire;
        temporaire->next = NULL;
    }

    int orientation_donner = sans_valeur;

    pomme pomme_du_terrain;
    nouvelle_coordonees(&pomme_du_terrain);
    

    while(playing){
        blit(fond_snake,buffer,0,0,0,0,fond_snake->w,fond_snake->h);
        affichage_snake(&buffer,tete,sprites_serpent);
        masked_blit(pomme_sprites,buffer,0,0,(SCREEN_W-200)/taille_snake*pomme_du_terrain.x,SCREEN_H/taille_snake*pomme_du_terrain.y,pomme_sprites->w,pomme_sprites->h);
        
        if(key[KEY_ESC])
            playing = false;
        if(key[KEY_UP] && tete->orientation!=bas && tete->orientation!=haut)
            orientation_donner = haut;
        if(key[KEY_DOWN] && tete->orientation!=bas && tete->orientation!=haut)
            orientation_donner = bas;
        if(key[KEY_LEFT] && tete->orientation!=gauche && tete->orientation!=droite)
            orientation_donner = gauche;
        if(key[KEY_RIGHT] && tete->orientation!=gauche && tete->orientation!=droite)
            orientation_donner = droite;
        /*
        if(orientation_donner!=sans_valeur){
            tete->orientation = orientation_donner;
            orientation_donner = sans_valeur;
        }*/

        check_pomme(tete,&pomme_du_terrain);
        blit(buffer,double_buffer,0,0,100,0,fond_snake->w,fond_snake->h);
        blit(double_buffer,screen,0,0,0,0,double_buffer->w,double_buffer->h);
        
        update_coordonate_snake(tete,&orientation_donner);

        //floating point exception du coup on arrondit

        tete->x=roundf(tete->x*1000)/1000;
        tete->y=roundf(tete->y*1000)/1000;

        if(check_mort_snake(tete)){
            playing = false;
        }

        rest(30);
    }


    destroy_bitmap(fond_snake);
}


void nouvelle_coordonees(pomme* actuel){
    actuel->x = rand()%10;
    actuel->y = rand()%10;
}


int check_mort_snake(node_snake *tete){
    node_snake *temporaire = tete->next;
    if(tete->x>9 || tete->x<0 || tete->y>9 || tete->y<0){
        printf("PERDU");
        rest(300);
        readkey();
        return true;
    }
    while (true)
    {
        if( roundf(tete->x)== roundf(temporaire->x) &&  roundf(tete->y)== roundf(temporaire->y)){
            printf("PERDU");
            rest(300);
            readkey();
            return true;
        }
        else if(temporaire->next!=NULL)
            temporaire = temporaire->next;
        else 
            break;

    }
    
    return false;

}


void check_pomme(node_snake *tete, pomme* my_pomme){
    if(tete->x==my_pomme->x && tete->y==my_pomme->y){
        node_snake *temporaire = tete;
        while(temporaire->next->next!=NULL)
            temporaire = temporaire->next;
        node_snake *new_node = malloc(sizeof(node_snake));
        new_node->next = temporaire->next;
        temporaire->next = new_node;
        if(temporaire->orientation==haut){
            new_node->x = temporaire->x;
            new_node->y = temporaire->y+1;
            new_node->next->y++;
        }
        else if(temporaire->orientation==bas){
            new_node->x = temporaire->x;
            new_node->y = temporaire->y-1;
            new_node->next->y--;
        }
        else if(temporaire->orientation==droite){
            new_node->x = temporaire->x-1;
            new_node->y = temporaire->y;
            new_node->next->x--;
        }
        else if(temporaire->orientation==gauche){
            new_node->x = temporaire->x+1;
            new_node->y = temporaire->y;
            new_node->next->x++;
        }
        new_node->orientation=temporaire->orientation;
        
        nouvelle_coordonees(my_pomme);
    }
}




void update_coordonate_snake(node_snake *actuel, int *orientation){
    
    if(is_integer(actuel->x) && is_integer(actuel->y)){
        update_orienation_snake(actuel);
        
        if(*orientation!=sans_valeur){
            actuel->orientation = *orientation;
            *orientation = sans_valeur;
        }
        
    }
    
    
    node_snake *buffer = actuel;

    while(true){
        if(buffer->orientation == haut)
            buffer->y-=snake_speed;
        else if(buffer->orientation == bas)
            buffer->y+=snake_speed;
        else if(buffer->orientation == droite)
            buffer->x+=snake_speed;
        else
            buffer->x-=snake_speed;
        if(buffer->next==NULL)
            break;
        buffer = buffer->next;
        }
    
}

void update_orienation_snake(node_snake *actuel){
    if(actuel->next!=NULL){
        update_orienation_snake(actuel->next);
        actuel->next->orientation = actuel->orientation;
    }
    
}


void affichage_snake(BITMAP** buffer, node_snake* tete,BITMAP** sprites_serpent){

    //impression de la tete

    BITMAP *rotate = create_bitmap(sprites_serpent[0]->w,sprites_serpent[0]->h);
    clear_to_color(rotate,makecol(255,0,255));

    rotate_sprite(rotate,sprites_serpent[0],0,0,itofix((tete->orientation*90* 256) / 360));
    masked_blit(rotate,*buffer,0,0,((*buffer)->w)/taille_snake*(tete->x),(*buffer)->w/taille_snake*(tete->y),sprites_serpent[0]->w,sprites_serpent[0]->h);


    node_snake *actuel = tete->next;
    while(actuel->next!=NULL){
        clear_to_color(rotate,makecol(255,0,255));
        rotate_sprite(rotate,sprites_serpent[1],0,0,itofix((actuel->orientation*90* 256) / 360));
        masked_blit(rotate,*buffer,0,0,(*buffer)->w/taille_snake*(actuel->x),(*buffer)->w/taille_snake*(actuel->y),sprites_serpent[1]->w,sprites_serpent[1]->h);
        actuel = actuel->next;
    }

    clear_to_color(rotate,makecol(255,0,255));
    rotate_sprite(rotate,sprites_serpent[2],0,0,itofix((actuel->orientation*90* 256) / 360));
    masked_blit(rotate,*buffer,0,0,(*buffer)->w/taille_snake*(actuel->x),(*buffer)->w/taille_snake*(actuel->y),sprites_serpent[2]->w,sprites_serpent[2]->h);
    
}


