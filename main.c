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
    loading_bmp = load_bitmap("map.bmp",NULL);
    int x = 0;
    int y = 0;

    if(loading_bmp == NULL){
        allegro_message("Error while loading the map.");
        allegro_exit();
        return 1;
    }

    fond = create_bitmap((loading_bmp->x)*2.5,(loading_bmp->y)*2.5);
    stretch_blit(loading_bmp, fond, 0, 0, loading_bmp->w, loading_bmp->h, 0, 0, fond->w, fond->h);
    destroy_bitmap(loading_bmp);


    blit(fond,screen,x,y,0,0,fond->w,fond->h);

    while(!key[KEY_ESC]){
        if(key[KEY_RIGHT] && x<=fond->w-SCREEN_W)
            x++;
        if(key[KEY_LEFT] && x>=0)
            x--;
        if(key[KEY_UP] && y>=0 )
            y--;
        if(key[KEY_DOWN] && y<= fond->h - SCREEN_H)
            y++;
        blit(fond,screen,x,y,0,0,fond->w,fond->h);
    }

    readkey();
    return 0;
}