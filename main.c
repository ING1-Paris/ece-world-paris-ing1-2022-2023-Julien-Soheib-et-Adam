#include <stdio.h>
#include <allegro.h>


int init_allegro(void);
int init_fond(void);

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
    BITMAP *perso[9];
    BITMAP *buffer;
    loading_bmp = load_bitmap("map.bmp",NULL);
    int x = 0;
    int y = 0;
    int count = 0;
    
    buffer = load_bitmap("perso.bmp",NULL);
    
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
    /*
    for(int i = 0; i<9;i++){
        perso[i] = create_sub_bitmap(buffer,i*16,(i%3)*16,16,16);
        if(perso[i] == NULL){
        allegro_message("Error while loading character sprites.");
        allegro_exit();
        return 1;
    }
    }
    */

    

    fond = create_bitmap((loading_bmp->w)*2.5,(loading_bmp->h)*2.5);
    stretch_blit(loading_bmp, fond, 0, 0, loading_bmp->w, loading_bmp->h, 0, 0, fond->w, fond->h);
    destroy_bitmap(loading_bmp);    
    //destroy_bitmap(buffer);

    if(fond== NULL){
        allegro_message("Error while loading the map.");
        allegro_exit();
        return 1;
    }


    blit(fond,screen,x,y,0,0,fond->w,fond->h);

    while(!key[KEY_ESC]){
        if(key[KEY_RIGHT] && x<=fond->w-SCREEN_W){
            //draw_sprite(screen,perso[count*3],SCREEN_W/2-16,SCREEN_H/2-16);
            count++;
            count = count%3;
            x++;
        }
        if(key[KEY_LEFT] && x>=0){
            //draw_sprite(screen,perso[count*2],SCREEN_W/2-16,SCREEN_H/2-16);
            count++;
            count = count%3;
            x--;
        }
        if(key[KEY_UP] && y>=0 ){
            //draw_sprite(screen,perso[count*1],SCREEN_W/2-16,SCREEN_H/2-16);
            count++;
            count = count%3;
            y--;
        }
        if(key[KEY_DOWN] && y<= fond->h - SCREEN_H){
            //draw_sprite(screen,perso[count*4],SCREEN_W/2-16,SCREEN_H/2-16);
            count++;
            count = count%3;
            y++;
        }
        blit(fond,screen,x,y,0,0,fond->w,fond->h);
    }

    
    return 0;
}