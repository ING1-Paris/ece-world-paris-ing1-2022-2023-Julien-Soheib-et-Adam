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
    BITMAP *perso[16];
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
    int v =0;
    for(int i = 0; i<4;i++){
        for(int j = 0;j<4;j++){
            perso[v] = create_sub_bitmap(buffer,(j%4)*48,(i%4)*64,48,64);
            if(perso[v] == NULL){
            allegro_message("Error while loading character sprites.");
            allegro_exit();
            return 1;
            }
            v++;
        }
    }
    
    /*
    int sub_width = 48;
    int sub_height = 64;
    int k =0;
    for(int i = 0;i<4;i++){
        for(int j = 0;j<4;j++){
            int sub_x = j * sub_width;
            int sub_y = i * sub_height;
            perso[k] = create_sub_bitmap(buffer, sub_x, sub_y, sub_width, sub_height);
            k++;
        }
    }
    */
    

    

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
            masked_blit(perso[count*3-1],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            //count++;
            count = count++==5 ? count=1:count++;    // BUG ICI
            x++;
        }
        if(key[KEY_LEFT] && x>=0){
            masked_blit(perso[count*2],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%4;
            x--;
        }
        if(key[KEY_UP] && y>=0 ){
            masked_blit(perso[count*4],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%4;
            y--;
        }
        if(key[KEY_DOWN] && y<= fond->h - SCREEN_H){
            masked_blit(perso[count*1],buffer,0,0,SCREEN_W/2-16,SCREEN_H/2-16,48,64);
            count++;
            count = count%4;
            y++;
        }
        
        blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
        
    }

    
    return 0;
}