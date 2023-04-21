#include "header.h"


int main(void) {
    
    if(init_allegro()!=0)
        return 1;
    
    init_fond();

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


int init_fond(void){
    BITMAP *loading_bmp;
    BITMAP *fond;
    BITMAP *perso[4][4];
    BITMAP *buffer;
    loading_bmp = load_bitmap("map.bmp",NULL);
    int x = 0;
    int y = 0;
    int count = 0;
    
    buffer = load_bitmap("perso_poke.bmp",NULL);
    
    if(buffer == NULL){
        allegro_message("Error while loading character sprite.");
        allegro_exit();
        return 1;
    }
    
    if(loading_bmp == NULL){
        allegro_message("Error while loading the map.");
        allegro_exit();
        return 1;
    }
    for(int i = 0; i<4;i++){
        for(int j = 0;j<4;j++){
            perso[i][j] = create_sub_bitmap(buffer,(j%4)*48,(i%4)*64,48,64);
            if(perso[i][j] == NULL){
            allegro_message("Error while loading character sprites.");
            allegro_exit();
            return 1;
            }
        }
    }
    

    

    fond = create_bitmap((loading_bmp->w)*2.5,(loading_bmp->h)*2.5);
    stretch_blit(loading_bmp, fond, 0, 0, loading_bmp->w, loading_bmp->h, 0, 0, fond->w, fond->h);
    destroy_bitmap(loading_bmp);    
    destroy_bitmap(buffer);

    if(fond == NULL){
        allegro_message("Error while loading the map.");
        allegro_exit();
        return 1;
    }

    buffer = create_bitmap(SCREEN_W,SCREEN_H);
    blit(fond,screen,x,y,0,0,fond->w,fond->h);

    while(!key[KEY_ESC]){
        blit(fond,buffer,x,y,0,0,fond->w,fond->h);
        if(key[KEY_RIGHT] && x<=fond->w-SCREEN_W){
            masked_blit(perso[2][count],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%4;
            x+=3;
        }
        else if(key[KEY_LEFT] && x>=0){
            masked_blit(perso[1][count],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%4;
            x-=3;
        }
        else if(key[KEY_UP] && y>=0 ){
            masked_blit(perso[3][count],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%4;
            y-=3;
        }
        else if(key[KEY_DOWN] && y<= fond->h - SCREEN_H){
            masked_blit(perso[0][count],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%4;
            y+=3;
        }
        else{
            masked_blit(perso[0][0],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
        }
        
        blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
        
    }

    
    return 0;
}