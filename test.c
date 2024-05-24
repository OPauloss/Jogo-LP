#include <allegro5/allegro.h>               
#include <allegro5/allegro_font.h>          
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>

// typedef struct Ent_inimigo {
//     int pos_x;
//     int pos_y;
//     int vida;
//     int dano;
// }Monstro;

// int estaDentroDaTela(Ent_inimigo * entidade) {
//     if (entidade.pos_x > )
// }
#define ALTURA  
#define FPS 60

typedef struct disparo{
    int x;
    int y;
    int velocidade;
    bool ativo;
    int dano;
}disparo;

void preencher_disparos(int verificador[10]){
    for(int i = 0; i<10; )
}

int main (){

    al_init();
    al_init_font_addon(); 
    al_init_ttf_addon();
    al_init_image_addon();    
    al_install_keyboard();

    // (Horizontal,vertical)
    ALLEGRO_DISPLAY * display = al_create_display(1280,720); //resolucao da janela
    al_set_window_position(display, 200, 200); 
    al_set_window_title(display, "Trexton"); //titulo da janela

    ALLEGRO_FONT* font = al_load_font("./font.ttf", 25, 0); //muda a fonte
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); 

    ALLEGRO_BITMAP * sprite = al_load_bitmap("./nave_resized.png");
    ALLEGRO_BITMAP * inimigo = al_load_bitmap("./mon.png");
    ALLEGRO_BITMAP * disparo = al_load_bitmap("./disparo.png");
    ALLEGRO_BITMAP *bg = al_load_bitmap("./R.jpg");
    

    ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue(); 
    al_register_event_source(event_queue, al_get_display_event_source(display)); 
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source() );
    al_start_timer(timer);

     float frame = 0.f;
    int pos_x = 0, pos_y = 600;
    int disparo_pos_x = 0, disparo_pos_y = 600;
    int current_frame_y = 0;

    int inimigo_pos_x = 1200, inimigo_pos_y = 0;
    int inimigo_vel_x = 2; 

    int qtd_disparos = 0;

    bool key_up_pressed = false;

    while(true){ 
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
            break;
        } else if( event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode ==ALLEGRO_KEY_UP ){
            pos_y -= 20;
        } else if( event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode ==ALLEGRO_KEY_RIGHT){
            pos_x += 20;
        } else if( event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode ==ALLEGRO_KEY_LEFT ){
            pos_x -= 20;
        }
        else if( event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode ==ALLEGRO_KEY_DOWN){
            pos_y += 20;
        } else if( event.keyboard.keycode == ALLEGRO_KEY_SPACE ){
            key_up_pressed = true;
            disparo_pos_x = pos_x + (al_get_bitmap_width(sprite) / 2) - (al_get_bitmap_width(disparo) / 2); // Coloca o disparo no centro da nave
            disparo_pos_y = pos_y + al_get_bitmap_height(sprite)/4; // Coloca o disparo abaixo da nave
        }

        if (key_up_pressed ) {

            // Move o disparo para cima até sair para a borda
            disparo_pos_x += 10; //Velocidade do disparo
        } else {
            key_up_pressed = false; // Se o disparo saiu da tela, desativa a tecla pressionada
        }

         inimigo_pos_x -= inimigo_vel_x;
         if(inimigo_pos_x>0 || inimigo_pos_x > (al_get_display_width(display) - al_get_bitmap_width(sprite))){
             inimigo_pos_x -= inimigo_vel_x;
        
         inimigo_pos_x -=inimigo_vel_x;
     }
        al_clear_to_color(al_map_rgb(255,255,255));
        al_draw_bitmap(bg, 0, 0, 0);
        al_draw_text(font, al_map_rgb(0,0,0), 7, 7, 0, "Space invaders");  //titulo da janela
        al_draw_text(font, al_map_rgb(255,255,255), 5, 5, 0, "Space Invaders");
        al_draw_bitmap(inimigo,inimigo_pos_x ,inimigo_pos_y ,0);
        al_draw_bitmap(sprite, pos_x, pos_y,0); // Desenha a nave

        if (key_up_pressed) {
            // Desenha o disparo apenas se a tecla para cima estiver pressionada
            al_draw_bitmap(disparo, disparo_pos_x, disparo_pos_y, 0); // Desenha o disparo
        }

        al_flip_display();
    }

    al_destroy_bitmap(bg);
    al_destroy_bitmap(sprite);
    al_destroy_bitmap(disparo);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
