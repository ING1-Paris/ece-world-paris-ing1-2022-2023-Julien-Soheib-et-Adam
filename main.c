#include "header.h"


int main(int argc, char* _argv[]) {
    
    if(init_allegro()!=0)
        return 1;
    
    player* joueurs = malloc(sizeof(joueurs)*2);
    
    if(argc == 1)
        in_game();
    else if(atoi(_argv[1])==0)
        snake_game(joueurs);
    else if(atoi(_argv[1])==1)
        playing_machine(joueurs,0);
    else if(atoi(_argv[1])==2)
        jeu_compteur(joueurs);
    
    
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
    srand(time(NULL));
    // INITIALISATION DU NECESSAIRE POUR LANCER LE JEU
    BITMAP *fond;
    BITMAP ***perso;
    BITMAP *buffer;
    BITMAP *fond_interdit;

    

    int magenta = makecol(255,0,255);

    player* joueurs = (player*) malloc(sizeof(player)*2);
    setup_player(joueurs);
    

    int count = 0;
    bool playing = true;

    perso = (BITMAP***) malloc(sizeof(BITMAP**)*4);
    if(perso==NULL)
        return 1;
    

    init_bitmap(&fond,perso,&fond_interdit);

    buffer = create_bitmap(SCREEN_W,SCREEN_H);
    if(buffer==NULL)
        return 1;

    int player_actuelle = 0;
    
    clear_to_color(buffer,makecol(100,100,100));
    textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(0,0,0),-1,"%s c'est toi qui commence on y va ?",joueurs[player_actuelle].name);
    blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    readkey();
    

    int x = fond->w/2;
    int y = fond->h/2;
    
    int last_x = x;
    int last_y = y;

    

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
        /*
        if(getpixel(fond_interdit,x,y)==magenta || getpixel(fond_interdit,x+48,y+64)==magenta || getpixel(fond_interdit,x,y+64)==magenta || getpixel(fond_interdit,x+48,y)==magenta){
            printf("magenta\n");
            x=last_x;
            y=last_y;
        }
        */
        last_x = x;
        last_y = y;
        
        for(int i = 0; i<2;i++){
            textprintf_centre_ex(buffer,font,SCREEN_W-180,50+(10*i),makecol(0,0,0),-1,"%s",joueurs[i].name);
            textprintf_centre_ex(buffer,font,SCREEN_W-60,50+(10*i),makecol(0,0,0),-1,"%i tickets",joueurs[i].tickets);
        }
        
        blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
        if(key[KEY_ENTER]){
            printf("x : %i  ;  y : %i \n",x,y);
        }
        if(checking_coordonates(&x,&y,joueurs,player_actuelle)==0){
            joueurs[1].tickets--;
            joueurs[0].tickets--;
            for(int i = 0;i<4;i++){
                for(int j =0;j<4;j++){
                    destroy_bitmap(perso[i][j]);
                }
            }
            destroy_bitmap(fond_interdit);
            destroy_bitmap(fond);
            
            init_bitmap(&fond,perso,&fond_interdit);
            rest(1000);
            
            player_actuelle = (player_actuelle+1)%2;
            /*
            clear_to_color(buffer,makecol(100,100,100));
            textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(0,0,0),-1,"%s c'est ton tour !",joueurs[player_actuelle].name);
            blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            */
            if(joueurs[0].tickets==0){
                printf("perdu");
            }
            if(joueurs[1].tickets==0){
                printf("perdu");
            }
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
    BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H);
    int touche;
    char charactere;
    int joueur_saisie = 0;
    int index_saisie_nom = 0;
    char nom_joueur[2][50];
    char* joueur_numero[2] = {"premier","deuxième"};
    while(!key[KEY_ESC] && joueur_saisie!=2){
        clear_to_color(buffer,makecol(100,0,220));
        textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/12,makecol(100,200,20),-1,"BIENVENUE DANS ECE WORLD");
        textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2-50,makecol(100,200,20),-1,"Saisie le nom du %s joueur : ",joueur_numero[joueur_saisie]);
        
        textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(100,200,20),-1,"%s",nom_joueur[joueur_saisie]);
        textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2+100,makecol(100,200,20),-1,"appuie sur entré lorsque tu as fini");
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        if(keypressed()){
            touche = readkey();
            charactere = touche & 0xff;
            if(charactere==8 && index_saisie_nom>0){
                index_saisie_nom--;
                nom_joueur[joueur_saisie][index_saisie_nom] = '\0';
                
            }
            if(charactere==13 && index_saisie_nom>0){
                nom_joueur[joueur_saisie][index_saisie_nom] = '\0';
                joueur_saisie++;
                index_saisie_nom = 0;
            }
            if(charactere>=32 && charactere <=126 && index_saisie_nom<49){
                nom_joueur[joueur_saisie][index_saisie_nom] = charactere;
                index_saisie_nom++;
            }
        }
        
        
    }
    
    for(int i =0;i<2;i++){
        char* nom = malloc(strlen(nom_joueur[i])+1);
        strcpy(nom,nom_joueur[i]);
        joueurs[i].name = nom;
        joueurs[i].tickets = 5;
    }
   


    
}










// FONCTION POUR INITIALISER TOUTES LES BITMAPS NECESSAIRES
void init_bitmap(BITMAP ** fond, BITMAP ***perso,BITMAP **fond_interdit){

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

    *fond = create_bitmap((loading_bmp->w)*1.5,(loading_bmp->h)*1.5);
    stretch_blit(loading_bmp, *fond, 0, 0, loading_bmp->w, loading_bmp->h, 0, 0, (*fond)->w, (*fond)->h);
    

    if(*fond == NULL){
        allegro_message("Error while loading the map.");
        allegro_exit();
        return;
    }


    destroy_bitmap(loading_bmp); 

    // bitmap interdit de penetrer

    BITMAP* last;
    last = load_bitmap("map_interdit.bmp",NULL);

    if(last == NULL){
        allegro_message("Error while loading the map forbiden");
        allegro_exit();
        return;
    }

    *fond_interdit = create_bitmap((last->w)*1.5,(last->h)*1.5);
    stretch_blit(last, *fond_interdit, 0, 0, last->w, last->h, 0, 0, (*fond_interdit)->w, (*fond_interdit)->h);

     if(*fond_interdit == NULL){
        allegro_message("Error while loading the map interdit.");
        allegro_exit();
        return;
    }
}


// check si on arrive dans une zone de mini jeu
int checking_coordonates(int *x, int *y, player * joueur, int player_actuelle){

    // ici test pour l'instant

    if(*x>=30 && *x<=100 && *y<= 1500 && *y>=1420){
        snake_game(joueur);
        *x = 0;
        *y = 0;
        return 0;
    }
    else if(*x>=375 && *x<=420 && *y<= 1980 && *y>=2005){
        jeu_compteur(joueur);
        *x = 0;
        *y = 0;
        return 0;
    }
    else if(*x>=1600 && *x<=1700 && *y<= 1380 && *y>=1350){
        playing_machine(joueur,player_actuelle);
        *x = 0;
        *y = 0;
        return 0;
    }
    else if(*x>=930 && *x<=1010 && *y<= 425 && *y>=400){
        //jeu_soheib(joueur);
        *x = 0;
        *y = 0;
        return 0;
    }
    return 1;

}

void snake_game(player* joueur){
    BITMAP * buffer = create_bitmap(SCREEN_W,SCREEN_H);
    int compteur_joueur = 0;
    int score_joueur[2] = {0,0};
    double temps_joueur[2] = {0.0,0.0};
    while(!key[KEY_ESC]){
        if(key[KEY_ENTER]){
            in_game_snake(&score_joueur[compteur_joueur],&temps_joueur[compteur_joueur]);
            compteur_joueur++;
            if(compteur_joueur==2){
                int winner = score_joueur[0]>score_joueur[1] ? 0 : score_joueur[0]<score_joueur[1]? 1:temps_joueur[0]<temps_joueur[1] ? 0:1; 
                clear_to_color(buffer,makecol(100,100,40));
                textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(86, 232, 47),-1,"%s à gagné bravo !",joueur[winner].name);
                textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2+50,makecol(86, 232, 47),-1,"%s tu gagnes 2 tickets !",joueur[winner].name);
                blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                joueur[winner].tickets+=3;
                rest(5000);
                readkey();
                return;
            }
        }
        clear_to_color(buffer,makecol(60,200,120));
        textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(86, 0, 47),-1,"Pret a jouer ? C'est au tour de %s",joueur[compteur_joueur].name);
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        
    }
}



void in_game_snake(int* score_finale , double * temps_finale){
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

    int taille_du_snake = 0;
    char* taille_char = malloc(sizeof(char)*4);

    pomme pomme_du_terrain;
    nouvelle_coordonees(&pomme_du_terrain);
    
    clock_t actuel, debut;
    debut = clock();
    while(playing){
        actuel = clock();
        clear_to_color(double_buffer,makecol(245,221,221));
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
        

        taille_du_snake+=check_pomme(tete,&pomme_du_terrain);
        blit(buffer,double_buffer,0,0,200,0,fond_snake->w,fond_snake->h);

        textprintf_centre_ex(double_buffer,font,100,50,makecol(0,0,255),-1,"Votre score :");
        textprintf_centre_ex(double_buffer,font,100,70,makecol(0,0,255),-1,"%d",taille_du_snake);


        textprintf_centre_ex(double_buffer,font,100,150,makecol(0,0,255),-1,"%.2f",(double)(actuel-debut)/CLOCKS_PER_SEC);
        textprintf_centre_ex(double_buffer,font,100,130,makecol(0,0,255),-1,"Temps depuis le départ :");

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

    *score_finale = taille_du_snake;
    *temps_finale =  (double)(actuel-debut)/CLOCKS_PER_SEC;
    destroy_bitmap(fond_snake);
    destroy_bitmap(buffer);
    destroy_bitmap(double_buffer);
    destroy_bitmap(tete_serpent);
    destroy_bitmap(queue_serpent);
    destroy_bitmap(corps_serpent);

}


void nouvelle_coordonees(pomme* actuel){
    actuel->x = rand()%10;
    actuel->y = rand()%10;
}


int check_mort_snake(node_snake *tete){
    node_snake *temporaire = tete->next;
    if(tete->x>9 || tete->x<0 || tete->y>9 || tete->y<0){
        rest(300);
        readkey();
        return true;
    }
    while (true)
    {
        if( roundf(tete->x)== roundf(temporaire->x) &&  roundf(tete->y)== roundf(temporaire->y)){
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


int check_pomme(node_snake *tete, pomme* my_pomme){
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
        return 1;
    }
    return 0;
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
            buffer->y-=snake_speed_high;
        else if(buffer->orientation == bas)
            buffer->y+=snake_speed_high;
        else if(buffer->orientation == droite)
            buffer->x+=snake_speed_high;
        else
            buffer->x-=snake_speed_high;
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





void playing_machine(player* joueur, int player_actuelle){
    
    srand(time(NULL));
    
    BITMAP* fond, *icons[10], *buffer,*fond_tempo,*tempons;

    buffer = create_bitmap(SCREEN_W,SCREEN_H);

    fond_tempo = load_bitmap("slot_machine.bmp",NULL);

    if(fond_tempo == NULL) {
        allegro_message("Error fond_tempo bitmaps.");
        allegro_exit();
        return;
    }

    fond = create_bitmap(SCREEN_W,SCREEN_H);

    stretch_blit(fond_tempo,fond,0,0,fond_tempo->w,fond_tempo->h,0,0,fond->w,fond->h);

    destroy_bitmap(fond_tempo);

    tempons = load_bitmap("diamant_jackpot.bmp",NULL);
    icons[0] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[0],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);

    destroy_bitmap(tempons);

    tempons = load_bitmap("trefle_jackpot.bmp",NULL);
    icons[1] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[1],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);
    destroy_bitmap(tempons);

    tempons = load_bitmap("orange_jackpot.bmp",NULL);
    icons[2] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[2],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);
    destroy_bitmap(tempons);


    tempons = load_bitmap("raisain_jackpot.bmp",NULL);
    icons[3] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[3],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);
    destroy_bitmap(tempons);

    tempons = load_bitmap("cloche_jackpot.bmp",NULL);
    icons[4] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[4],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);
    destroy_bitmap(tempons);

    tempons = load_bitmap("fraise_jackpot.bmp",NULL);
    icons[5] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[5],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);
    destroy_bitmap(tempons);

    tempons = load_bitmap("orange_jackpot.bmp",NULL);
    icons[6] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[6],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);
    destroy_bitmap(tempons);

    tempons = load_bitmap("pasteque_jackpot.bmp",NULL);
    icons[7] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[7],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);
    destroy_bitmap(tempons);

    tempons = load_bitmap("pierre_jackpot.bmp",NULL);
    icons[8] = create_bitmap(taille_icons,taille_icons);

    stretch_blit(tempons,icons[8],0,0,tempons->w,tempons->h,0,0,taille_icons,taille_icons);
    destroy_bitmap(tempons);

    tempons = load_bitmap("win.bmp",NULL);
    icons[9] = create_bitmap(400,200);

    stretch_blit(tempons,icons[9],0,0,tempons->w,tempons->h,0,0,icons[9]->w,icons[9]->h);
    destroy_bitmap(tempons);


    int* liste_gagnant = (int*) malloc(sizeof(int)*3);

    int* position_chargement_gauche = malloc(sizeof(int)*5);
    int* position_chargement_droite = malloc(sizeof(int)*5);

    for(int i = 0; i<5 ; i++){
        position_chargement_droite[i] = SCREEN_W/5*i;
        position_chargement_gauche[i] = SCREEN_W/5*i;
    }

    int* type_icon_gauche = malloc(sizeof(int)*5);
    int* type_icon_droite = malloc(sizeof(int)*5);

    for(int i = 0;i<5;i++){
        type_icon_droite[i] = rand()%9;
        type_icon_gauche[i] = rand()%9;
    }



    while(!key[KEY_ESC]){

        ecran_acceuil_jackpot(icons,&buffer,position_chargement_gauche,position_chargement_droite,type_icon_droite,type_icon_gauche,joueur,player_actuelle);
       

        if(key[KEY_ENTER]){
            tirage_au_sort(liste_gagnant);
            affichage_jackpot(icons,fond,liste_gagnant);
            readkey();
            if(liste_gagnant[0]==liste_gagnant[1] && liste_gagnant[0]==liste_gagnant[2]){
                joueur[player_actuelle].tickets+=2;
                clear_to_color(buffer,makecol(120,200,100));
                textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(255,255,0),-1,"%s bravo !! Tu as gagné un ticket", joueur[player_actuelle].name);
                blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            }
            rest(5000);
            readkey();
            return;
        }
        
    }

    
    

    for(int i =0;i<10;i++){
        destroy_bitmap(icons[i]);
    }
    destroy_bitmap(fond);
    destroy_bitmap(buffer);
    
    

}

void affichage_jackpot(BITMAP **icons, BITMAP* fond,int* winner){
    BITMAP *buffer = create_bitmap(SCREEN_W,SCREEN_H);
    for(int i = 0;i<30;i++){
        blit(fond,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        masked_blit(icons[rand()%9],buffer,0,0,taille_icons,SCREEN_H/2-taille_icons/2,taille_icons,taille_icons);
        masked_blit(icons[rand()%9],buffer,0,0,SCREEN_W/3+taille_icons,SCREEN_H/2-taille_icons/2,taille_icons,taille_icons);
        masked_blit(icons[rand()%9],buffer,0,0,SCREEN_W/3*2+taille_icons,SCREEN_H/2-taille_icons/2,taille_icons,taille_icons);
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(6*i);
    }
    blit(fond,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
    masked_blit(icons[winner[0]],buffer,0,0,taille_icons,SCREEN_H/2-taille_icons/2,taille_icons,taille_icons);
    masked_blit(icons[winner[1]],buffer,0,0,SCREEN_W/3+taille_icons,SCREEN_H/2-taille_icons/2,taille_icons,taille_icons);
    masked_blit(icons[winner[2]],buffer,0,0,SCREEN_W/3*2+taille_icons,SCREEN_H/2-taille_icons/2,taille_icons,taille_icons);
    blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    rest(900);
    if(winner[0]==winner[1] && winner[0]==winner[2]){
        clear_to_color(buffer,makecol(0,0,0));
        masked_blit(icons[9],buffer,0,0,SCREEN_W/2-icons[9]->w/2,SCREEN_H/2-icons[9]->h/2,icons[9]->w,icons[9]->h);
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        rest(3000);
        return;
    }
}


void tirage_au_sort(int* liste){
    liste[0] = rand()%1;
    liste[1] = rand()%1;
    liste[2] = rand()%1;
}

void ecran_acceuil_jackpot(BITMAP **icons,BITMAP ** buffer,int* position_chargement_gauche, int* position_chargement_droite,int *type_icon_droite,int * type_icon_gauche,player *joueur,int player_playing){
 
    clear_to_color(*buffer,makecol(120,180,100));
    textprintf_centre_ex(*buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(250,250,255),-1,"Appuie sur la touche entrer afin de commencer à jouer");
    textprintf_centre_ex(*buffer,font,SCREEN_W/2,SCREEN_H/2+50,makecol(250,250,255),-1,"%s c'est toi qui joue ",joueur[player_playing].name);
    
    for(int i = 0;i<5;i++){
        masked_blit(icons[type_icon_droite[i]],*buffer,0,0,680,position_chargement_droite[i],taille_icons,taille_icons);
        masked_blit(icons[type_icon_gauche[i]],*buffer,0,0,50,position_chargement_gauche[i],taille_icons,taille_icons);
        position_chargement_droite[i] ++;
        position_chargement_gauche[i]--;
        if(position_chargement_droite[i]>SCREEN_H+taille_icons){
            type_icon_droite[i] = rand()%9;
            position_chargement_droite[i] = -taille_icons;
        }
        if(position_chargement_gauche[i]<-taille_icons){
            type_icon_gauche[i] = rand()%9;
            position_chargement_gauche[i] = SCREEN_H+taille_icons;
        }
        
    }
    blit(*buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    
}       


void jeu_compteur(player* joueur){
    clock_t debut,actuel;
    int click=0;
    int playing =0;
    BITMAP *buffer = create_bitmap(SCREEN_W,SCREEN_H);
    int unclick = 1;
    int tour_joueur = 0;
    int scorePlayer[2] = {-1,-1};
    
    while(!key[KEY_ESC] ){
        if(playing==1){

            actuel = clock();
            clear_to_color(buffer,makecol(120,200,100));
            textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(255,255,0),-1,"Votre score actuel est de %i",click);
            textprintf_centre_ex(buffer,font,SCREEN_W-100,30,makecol(255,255,0),-1,"Temps : %.2f seconde restante",(double) 10-((double)(actuel-debut)/CLOCKS_PER_SEC));
            blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            if(mouse_b & 1 && unclick==1){
                click++;
                unclick = 0;
            }
            else if(!(mouse_b & 1) && unclick==0)
                unclick = 1;

            if((double)(actuel-debut)/CLOCKS_PER_SEC>10){
                clear_to_color(buffer,makecol(120,200,100));
                textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(255,255,0),-1,"Votre score final est de %i, Bravo %s!",click, joueur[tour_joueur].name);
                blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                rest(2000);
                readkey();
                scorePlayer[tour_joueur] = click;
                click = 0;
                playing = 0;
                tour_joueur++;
            }
            
        }
        else if(tour_joueur==2){
            if(scorePlayer[0]>scorePlayer[1]){
                joueur[0].tickets+=2;
                clear_to_color(buffer,makecol(120,200,100));
                textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(255,255,0),-1,"%s à gagné, bravo tu recois un ticket suplémentaire !", joueur[0].name);
                blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            }
            else if(scorePlayer[1]>scorePlayer[0]){
                joueur[1].tickets+=2;
                clear_to_color(buffer,makecol(120,200,100));
                textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(255,255,0),-1,"%s à gagné, bravo tu recois un ticket suplémentaire !", joueur[1].name);
                blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            }
            else{
                clear_to_color(buffer,makecol(120,200,100));
                textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(255,255,0),-1,"Il y à égalité bravo a vous deux", joueur[0].name);
                blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            }
            rest(8000);

            return;
        }
        else{
            clear_to_color(buffer,makecol(120,200,100));
            textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(255,255,0),-1,"Appuyer sur la touche entrer pour commencer a joueur");
            textprintf_centre_ex(buffer,font,SCREEN_W/2,SCREEN_H/2+50,makecol(255,255,0),-1,"%s c'est ton tour",joueur[tour_joueur].name);
            blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            if(key[KEY_ENTER]){
                playing = 1;
                debut = clock();
            }
        }
    }
}






#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define TROU_WIDTH 180
#define TROU_HEIGHT 120
#define SPACING (SCREEN_WIDTH / 4)

#define OFFSET (SCREEN_WIDTH / 8)

void draw_trous(BITMAP *buffer, BITMAP *trou) {
    int descente = SCREEN_HEIGHT / 8;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            blit(trou, buffer, 0, 0, j * SPACING + OFFSET, i * SPACING + descente, TROU_WIDTH, TROU_HEIGHT);
        }
    }
}

int taupe_positions[9][2] = {{150, 25}, {350, 25}, {550, 25},
                             {150, 225}, {350, 225}, {550, 225},
                             {150, 425}, {350, 425}, {550, 425}};

void draw_random_taupe(BITMAP *buffer, BITMAP *taupe) {
    int rand_index = rand() % 9;
    stretch_sprite(buffer, taupe, taupe_positions[rand_index][0], taupe_positions[rand_index][1], 100, 110); // changé ici
}

int jeu_soheib() {
    allegro_init();
    install_timer();
    srand(time(NULL));


    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) != 0) {
        allegro_message("Erreur lors du changement de mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    install_mouse();
    show_mouse(screen);
    BITMAP *buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    clear_to_color(buffer, makecol(255, 255, 255));

    BITMAP *background_image = load_bitmap("image.bmp", NULL);
    if (background_image == NULL) {
        allegro_message("Erreur lors du chargement de l'image de fond");
        destroy_bitmap(buffer);
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    BITMAP *trou_image = load_bitmap("TrouImg.bmp", NULL);
    if (trou_image == NULL) {
        allegro_message("Erreur lors du chargement de l'image des trous");
        destroy_bitmap(buffer);
        destroy_bitmap(background_image);
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    BITMAP *taupe_image = load_bitmap("TaupeImg.bmp", NULL);
    if (taupe_image == NULL) {
        allegro_message("Erreur lors du chargement de l'image de la taupe");
        destroy_bitmap(trou_image);
        destroy_bitmap(background_image);
        destroy_bitmap(buffer);
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    draw_trous(buffer, trou_image);

    int not_clicked=1;
    int score=0;
    while(!key[KEY_ESC]){
        blit(background_image, buffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        draw_trous(buffer, trou_image);
        draw_random_taupe(buffer, taupe_image);
        if(mouse_b & 1 && not_clicked==1)
        {
            score++;
            not_clicked=0;
        }
        if(!(mouse_b & 1)&& not_clicked==0)
        {
            not_clicked=1;
        }
        textprintf_centre_ex(buffer,font,SCREEN_W-200,SCREEN_H-50, makecol(255,255,255),-1,"Score %i",score);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        rest(1500);
    }

    destroy_bitmap(trou_image);
    destroy_bitmap(taupe_image);
    destroy_bitmap(background_image);
    destroy_bitmap(buffer);
    allegro_exit();

    return 0;
}

   



