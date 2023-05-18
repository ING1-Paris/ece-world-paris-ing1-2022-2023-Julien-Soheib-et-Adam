#include "header.h"


int main(int argc, char* _argv[]) {
    
    if(init_allegro()!=0)
        return 1;
    if(argc == 1)
        in_game();
    else if(atoi(_argv[1])==0)
        in_game_snake();
    else if(atoi(_argv[1])==1)
        playing_machine();
    
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

    

    int count = 0;
    bool playing = true;

    perso = (BITMAP***) malloc(sizeof(BITMAP**)*4);
    if(perso==NULL)
        return 1;

    init_bitmap(&fond,perso,&fond_interdit);

    int x = fond->w/2;
    int y = fond->h/2;
    
    int last_x = x;
    int last_y = y;

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
        /*
        if(getpixel(fond_interdit,x,y)==magenta || getpixel(fond_interdit,x+48,y+64)==magenta || getpixel(fond_interdit,x,y+64)==magenta || getpixel(fond_interdit,x+48,y)==magenta){
            printf("magenta\n");
            x=last_x;
            y=last_y;
        }
        */
        last_x = x;
        last_y = y;
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

    int taille_du_snake = 0;
    char* taille_char = malloc(sizeof(char)*4);

    pomme pomme_du_terrain;
    nouvelle_coordonees(&pomme_du_terrain);
    

    while(playing){
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
        /*
        if(orientation_donner!=sans_valeur){
            tete->orientation = orientation_donner;
            orientation_donner = sans_valeur;
        }*/

        taille_du_snake+=check_pomme(tete,&pomme_du_terrain);
        blit(buffer,double_buffer,0,0,200,0,fond_snake->w,fond_snake->h);

        textprintf_centre_ex(double_buffer,font,50,50,makecol(0,0,255),-1,"Votre score :");
        textprintf_centre_ex(double_buffer,font,50,70,makecol(0,0,255),-1,"%d",taille_du_snake);

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





void playing_machine(void){
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

        ecran_acceuil_jackpot(icons,&buffer,position_chargement_gauche,position_chargement_droite,type_icon_droite,type_icon_gauche);
       

        if(key[KEY_ENTER]){
            tirage_au_sort(liste_gagnant);
            affichage_jackpot(icons,fond,liste_gagnant);
            readkey();
        }
        rest(50);
    }

    
    if(liste_gagnant[0]==liste_gagnant[1] && liste_gagnant[0]==liste_gagnant[2]){
        return;
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
        return;
    }
}


void tirage_au_sort(int* liste){
    liste[0] = rand()%1;
    liste[1] = rand()%1;
    liste[2] = rand()%1;
}

void ecran_acceuil_jackpot(BITMAP **icons,BITMAP ** buffer,int* position_chargement_gauche, int* position_chargement_droite,int *type_icon_droite,int * type_icon_gauche){
 
    clear_to_color(*buffer,makecol(0,0,0));
    textprintf_centre_ex(*buffer,font,SCREEN_W/2,SCREEN_H/2,makecol(250,250,255),-1,"Appuyez sur la touche entrer afin de commencer a jouer");
    
    for(int i = 0;i<5;i++){
        masked_blit(icons[type_icon_droite[i]],*buffer,0,0,680,position_chargement_droite[i],taille_icons,taille_icons);
        masked_blit(icons[type_icon_gauche[i]],*buffer,0,0,50,position_chargement_gauche[i],taille_icons,taille_icons);
        position_chargement_droite[i] +=5;
        position_chargement_gauche[i]-=5;
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