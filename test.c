#include <allegro5/allegro.h>               
#include <allegro5/allegro_font.h>          
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include "objetos.h"
#include<stdio.h>

#define NUM_TIRO 5
#define largura 1280
#define altura 720
#define FPS 60

// PROTÓTIPOS
void InicializaNave(Nave *nave);
void InicializaMonstro(Monstro *monstro);
void InitTiros(Tiro tiros[], int tamanho);
void AtirarTiros(Tiro tiros[], Nave nave, int tamanho);
void AtualizaTiros(Tiro tiros[], int tamanho);
void DesenhaTiros(Tiro tiros[],int tamanho);

int main (){
    //VARIÁVEIS DO JOGO
    Nave nave;
    Monstro monstro;
    Tiro tiro[NUM_TIRO];

    InicializaNave(&nave);
    InicializaMonstro(&monstro);
    //INICIALIZAÇÕES
    al_init();
    al_init_font_addon(); 
    al_init_ttf_addon();
    al_init_image_addon();    
    al_install_keyboard();

    // (Horizontal,vertical)
    ALLEGRO_DISPLAY * display = al_create_display(largura,altura);
    al_set_window_position(display, 200, 200); 
    al_set_window_title(display, "Trexton");

    ALLEGRO_FONT* font = al_load_font("./font.ttf", 25, 0); 
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS); 


    //DEFINIÇÕES DE BITMAP
    ALLEGRO_BITMAP * sprite = al_load_bitmap("./nave_resized.png");
    ALLEGRO_BITMAP * inimigo = al_load_bitmap("./mon.png");
    ALLEGRO_BITMAP * disparo = al_load_bitmap("./disparo.png");
    ALLEGRO_BITMAP *bg = al_load_bitmap("./R.jpg");


    ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue(); 

    al_register_event_source(event_queue, al_get_display_event_source(display)); 
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source() );
    al_start_timer(timer);

    

    

    int disparo_pos_x = 0, disparo_pos_y = 600;
    bool key_up_pressed = false;

    while(true){ 
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
            break;
        } else if( event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode ==ALLEGRO_KEY_UP ){
            nave.y -= nave.velocidade;
            if(nave.y<0)
                nave.y=0;
        } else if( event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode ==ALLEGRO_KEY_RIGHT){
            nave.x += nave.velocidade;
            if(nave.x>1160)
                nave.x=1160;
        } else if( event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode ==ALLEGRO_KEY_LEFT ){
            nave.x -= nave.velocidade;
            if(nave.x<0)
                nave.x=0;
        }
        else if( event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode ==ALLEGRO_KEY_DOWN){
            nave.y += nave.velocidade;
                    if (nave.y > altura - al_get_bitmap_height(sprite)) nave.y = altura - al_get_bitmap_height(sprite);
        } else if( event.keyboard.keycode == ALLEGRO_KEY_SPACE ){
            key_up_pressed = true;
            AtirarTiros(tiro, nave, NUM_TIRO);
            disparo_pos_x = nave.x + (al_get_bitmap_width(sprite) / 2) - (al_get_bitmap_width(disparo) / 2); 
            disparo_pos_y = nave.y + al_get_bitmap_height(sprite)/4; 
        }

        if (key_up_pressed ) {
            disparo_pos_x += 10; 
        } else {
            key_up_pressed = false; 
        }

         monstro.x -= monstro.velocidade;
         if(monstro.x>0 || monstro.x > (al_get_display_width(display) - al_get_bitmap_width(sprite))){
             monstro.x -= monstro.velocidade;
        
        monstro.x -= monstro.velocidade;
     }
        al_clear_to_color(al_map_rgb(255,255,255));
        al_draw_bitmap(bg, 0, 0, 0);
        al_draw_text(font, al_map_rgb(0,0,0), 7, 7, 0, "Space invaders");  
        al_draw_text(font, al_map_rgb(255,255,255), 5, 5, 0, "Space Invaders");
        al_draw_bitmap(inimigo,monstro.x ,monstro.y ,0);
        al_draw_bitmap(sprite, nave.x, nave.y,0); 

        if (key_up_pressed) {
            al_draw_bitmap(disparo, disparo_pos_x, disparo_pos_y, 0); 
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


void InicializaNave(Nave *nave){
    nave->x = 0;
    nave->y = 600;
    nave->velocidade = 20;
}

void InicializaMonstro(Monstro *monstro){
    monstro ->x = 1200;
    monstro ->y = 0;
    monstro ->velocidade = 2;
}

void InitTiros(Tiro tiros[], int tamanho)
{
    for(int i = 0; i<tamanho; i++){
        tiros[i].velocidade = 10;
        tiros[i].ativo = false;
    }
}
void AtirarTiros(Tiro tiros[], Nave nave, int tamanho)
{
    for(int i =0; i<tamanho; i++){
        if(!tiros[i].ativo){
            tiros[i].x = nave.x+20;
            tiros[i].y = nave.y;
            tiros[i].ativo = true;
        }
    }
}
void AtualizaTiros(Tiro tiros[], int tamanho)
{
    for(int i = 0; i< tamanho; i++){
        if(tiros[i].ativo)
        {
            tiros[i].x += tiros[i].velocidade;

            if(tiros[i].x > largura)
                tiros[i].ativo = false;
        }
    }
}
void DesenhaTiros(Tiro tiros[],int tamanho){
    for(int i =0; i< tamanho; i++){
        if(tiros[i].ativo){
            //al_draw_filled_circle(tiros[i].x, tiros[i].y, 2, al_map_rgb(255, 255, 255));
        }
    }
}

No* enfilar_comeco(No **fila, int valor){
    No *novo = malloc(sizeof(No));
    if(novo){
        novo->dado = valor;
        novo->proximo = *fila;
        *fila = novo;
    }
    else{
        printf("Erro ao alocar memoria");
    }
}

No* remover_final(No **fila){
    No *aux = *fila;
    No *ant = NULL;
    if(*fila == NULL){ 
        printf("A fila está vazia");
        return NULL;
    }
    while(aux->proximo !=NULL){
        ant = aux;
        aux = aux->proximo;
    }
    if(ant==NULL){
        free(aux);
        *fila = NULL;
    }
    else{
        ant->proximo = NULL; 
    }
}