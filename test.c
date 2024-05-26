#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include "objetos.h"
#include <stdio.h>

#define NUM_TIRO 5
#define largura 1280
#define altura 720
#define FPS 60

// PROTÓTIPOS
void InicializaNave(Nave* nave);
void InicializaMonstro(Monstro* monstro);
void InitTiros(Tiro tiros[], int tamanho);
void AtirarTiros(Tiro tiros[], Nave nave, int tamanho, ALLEGRO_BITMAP* disparo);
void AtualizaTiros(Tiro tiros[], int tamanho);
void DesenhaTiros(Tiro tiros[], int tamanho, ALLEGRO_BITMAP* disparo);

int main() {
    // VARIÁVEIS DO JOGO
    Nave nave;
    Monstro monstro;
    Tiro tiro[NUM_TIRO];

    InicializaNave(&nave);
    InicializaMonstro(&monstro);
    InitTiros(tiro, NUM_TIRO);

    // INICIALIZAÇÕES
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    //HORIZONTAL/VERTICAL
    ALLEGRO_DISPLAY* display = al_create_display(largura, altura);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "A gente vai conseguir, galera! Foco!");

    ALLEGRO_FONT* font = al_load_font("./font.ttf", 25, 0);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    //DEFINIÇÕES DE BITMAP
    ALLEGRO_BITMAP* sprite = al_load_bitmap("./nave_resized.png");
    ALLEGRO_BITMAP* inimigo = al_load_bitmap("./mon.png");
    ALLEGRO_BITMAP* disparo = al_load_bitmap("./disparo.png");
    ALLEGRO_BITMAP* bg = al_load_bitmap("./R.jpg");

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(timer);

    bool key_up = false;
    bool key_down = false;
    bool key_left = false;
    bool key_right = false;

    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                key_up = true;
                break;
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                key_down = true;
                break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_LEFT:
                key_left = true;
                break;
            case ALLEGRO_KEY_D:
            case ALLEGRO_KEY_RIGHT:
                key_right = true;
                break;
            case ALLEGRO_KEY_SPACE:
                AtirarTiros(tiro, nave, NUM_TIRO, disparo);
                break;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                key_up = false;
                break;
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                key_down = false;
                break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_LEFT:
                key_left = false;
                break;
            case ALLEGRO_KEY_D:
            case ALLEGRO_KEY_RIGHT:
                key_right = false;
                break;
            }
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.button == 1) {
                AtirarTiros(tiro, nave, NUM_TIRO, disparo);
            }
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            if (key_up) {
                nave.y -= nave.velocidade;
                if (nave.y < 0) nave.y = 0;
            }
            if (key_down) {
                nave.y += nave.velocidade;
                if (nave.y > altura - al_get_bitmap_height(sprite)) nave.y = altura - al_get_bitmap_height(sprite);
            }
            if (key_left) {
                nave.x -= nave.velocidade;
                if (nave.x < 0) nave.x = 0;
            }
            if (key_right) {
                nave.x += nave.velocidade;
                if (nave.x > largura - al_get_bitmap_width(sprite)) nave.x = largura - al_get_bitmap_width(sprite);
            }

            AtualizaTiros(tiro, NUM_TIRO);

            monstro.x -= monstro.velocidade;
            if (monstro.x < 0) {
                monstro.x = largura;
            }

            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_bitmap(bg, 0, 0, 0);
            al_draw_text(font, al_map_rgb(0, 0, 0), 7, 7, 0, "Space invaders");
            al_draw_text(font, al_map_rgb(255, 255, 255), 5, 5, 0, "Space Invaders");
            al_draw_bitmap(inimigo, monstro.x, monstro.y, 0);
            al_draw_bitmap(sprite, nave.x, nave.y, 0);
            DesenhaTiros(tiro, NUM_TIRO, disparo);
            al_flip_display();
        }
    }

    al_destroy_bitmap(bg);
    al_destroy_bitmap(sprite);
    al_destroy_bitmap(disparo);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}

void InicializaNave(Nave* nave) {
    nave->x = 0;
    nave->y = 600;
    nave->velocidade = 15;
}

void InicializaMonstro(Monstro* monstro) {
    monstro->x = 1200;
    monstro->y = 0;
    monstro->velocidade = 2;
}

void InitTiros(Tiro tiros[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        tiros[i].velocidade = 10;
        tiros[i].ativo = false;
    }
}

void AtirarTiros(Tiro tiros[], Nave nave, int tamanho, ALLEGRO_BITMAP* disparo) {
    for (int i = 0; i < tamanho; i++) {
        if (!tiros[i].ativo) {
            tiros[i].x = nave.x + (al_get_bitmap_width(disparo) / 2) - (al_get_bitmap_width(disparo) / 2);
            tiros[i].y = nave.y;
            tiros[i].ativo = true;
            break;
        }
    }
}

void AtualizaTiros(Tiro tiros[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (tiros[i].ativo) {
            tiros[i].y -= tiros[i].velocidade;
            if (tiros[i].y < 0) {
                tiros[i].ativo = false;
            }
        }
    }
}

void DesenhaTiros(Tiro tiros[], int tamanho, ALLEGRO_BITMAP* disparo) {
    for (int i = 0; i < tamanho; i++) {
        if (tiros[i].ativo) {
            al_draw_bitmap(disparo, tiros[i].x, tiros[i].y, 0);
        }
    }
}

No* enfilar_comeco(No** fila, int valor) {
    No* novo = malloc(sizeof(No));
    if (novo) {
        novo->dado = valor;
        novo->proximo = *fila;
        *fila = novo;
    }
    else {
        printf("Erro ao alocar memoria");
    }
}

No* remover_final(No** fila) {
    No* aux = *fila;
    No* ant = NULL;
    if (*fila == NULL) {
        printf("A fila está vazia");
        return NULL;
    }
    while (aux->proximo != NULL) {
        ant = aux;
        aux = aux->proximo;
    }
    if (ant == NULL) {
        free(aux);
        *fila = NULL;
    }
    else {
        ant->proximo = NULL;
    }
}