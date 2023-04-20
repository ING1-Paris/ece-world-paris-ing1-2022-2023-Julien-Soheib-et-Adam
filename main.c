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
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,1000,1000,0,0);

    /*
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,1000,1000,0,0)){
        allegro_message("Error while loading screen");
        allegro_exit();
        return 1;
    }*/
    
    
    
    return 0;
}


int init_fond(void){
    BITMAP *fond;
    fond = load_bitmap("map.bmp",NULL);

    if(fond == NULL){
        allegro_message("Error while loading the map.");
        allegro_exit();
        return 1;
    }

    blit(fond,screen,0,0,0,0,fond->w,fond->h);

    readkey();
    return 0;
}