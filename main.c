#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_acodec.h>
#include "objetos.h"
#include <stdio.h>
#include <stdlib.h>


#define NUM_TIRO 20
#define NUM_MONSTROS 10
#define largura 1280
#define altura 720
#define FPS 60

// PROTÓTIPOS
void InicializaNave(Nave* nave);
void InicializaMonstro(Monstro monstro[], int tamanho);
void InitTiros(Tiro tiros[], int tamanho);
void AtirarTiros(Tiro tiros[], Nave nave, int tamanho, ALLEGRO_BITMAP* disparo);
void AtualizaTiros(Tiro tiros[], int tamanho);
void DesenhaTiros(Tiro tiros[], int tamanho, ALLEGRO_BITMAP* disparo);
void LiberaMonstros(Monstro monstro[], int tamanho, ALLEGRO_BITMAP* inimigo);
void AtualizaMonstros(Monstro monstro[], int tamanho);
void DesenhaMonstros(Monstro monstro[], int tamanho, ALLEGRO_BITMAP* inimigo, int* pontuacao);
void BalaColidida(Tiro tiros[], int tamanho_tiro, Monstro monstro[], int tamanho_monstro, int* pontuacao);

int main() {
    // VARIÁVEIS DO JOGO
    Nave nave;
    Monstro monstro[NUM_MONSTROS];
    Tiro tiro[NUM_TIRO];

    int* pontuacao = (int*)malloc(sizeof(int));
    *pontuacao = 0;

    InicializaNave(&nave);
    InicializaMonstro(monstro, NUM_MONSTROS);
    InitTiros(tiro, NUM_TIRO);
    //Verificador inicializacoes 
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    if (!al_install_audio()) {
        fprintf(stderr, "Falha ao inicializar o áudio.\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar os codecs de áudio.\n");
        return -1;
    }

    if (!al_reserve_samples(2)) {
        fprintf(stderr, "Falha ao reservar samples de áudio.\n");
        return -1;
    }

    if (!al_init_font_addon()) {
        fprintf(stderr, "Falha ao iniciar a fonte\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao iniciar a imagem\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado");
        return -1;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "Falha ao inicializar o mouse.");
        return -1;
    }

    // CRIAÇÃO DE DISPLAY E OUTROS COMPONENTES
    ALLEGRO_DISPLAY* display = al_create_display(largura, altura);
    if (!display) {
        fprintf(stderr, "Falha ao criar o display.\n");
        return -1;
    }

    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "A gente vai conseguir, galera! Foco!");

    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_FONT* font = al_load_font("./font.ttf", 25, 0);
    if (!font) {
        fprintf(stderr, "Falha ao carregar a fonte.\n");
        return -1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Falha ao criar o timer.\n");
        return -1;
    }

    // DEFINIÇÕES DE BITMAP
    ALLEGRO_BITMAP* sprite = al_load_bitmap("./nave_resized.png");
    if (!sprite) {
        fprintf(stderr, "Falha ao carregar o sprite da nave.\n");
        return -1;
    }

    ALLEGRO_BITMAP* inimigo = al_load_bitmap("./mon.png");
    if (!inimigo) {
        fprintf(stderr, "Falha ao carregar o sprite do inimigo.\n");
        return -1;
    }

    ALLEGRO_BITMAP* disparo = al_load_bitmap("./disparo.png");
    if (!disparo) {
        fprintf(stderr, "Falha ao carregar o sprite do disparo.\n");
        return -1;
    }

    ALLEGRO_BITMAP* bg = al_load_bitmap("./R.jpg");
    if (!bg) {
        fprintf(stderr, "Falha ao carregar o background.\n");
        return -1;
    }

    // DEFINIÇÕES DE SAMPLE
    ALLEGRO_SAMPLE* sample = al_load_sample("./background.wav");
    ALLEGRO_SAMPLE* sample_2 = al_load_sample("./disparo-sound.wav");
    ALLEGRO_SAMPLE* sample_3 = al_load_sample("./colisao.wav");
    if (!sample) {
        fprintf(stderr, "Falha ao carregar o áudio.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(timer);


    // INICIA A REPRODUÇÃO DA MÚSICA DE FUNDO
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

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
                al_play_sample(sample_2, 0.5, 0.0, 3.0, ALLEGRO_PLAYMODE_ONCE, NULL);//0.5 é o som e 3.0 é a velocidade
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
            LiberaMonstros(monstro, NUM_MONSTROS, inimigo);
            AtualizaMonstros(monstro, NUM_MONSTROS);
            BalaColidida(tiro, NUM_TIRO, monstro, NUM_MONSTROS, pontuacao);

            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_bitmap(bg, 0, 0, 0);
//al_draw_text(font, al_map_rgb(0, 0, 0), 7, 7, 0, "Space invaders");
            al_draw_textf(font, al_map_rgb(255, 255, 255), 5, 5, 0, "Pontuacao: %d", *pontuacao);
           // char* ponto = *pontuacao;
            
//al_draw_textf(font, al_map_rgb(255, 255, 255), 250, 5, 0, %);
            DesenhaMonstros(monstro, NUM_MONSTROS, inimigo, pontuacao);
            al_draw_rotated_bitmap(sprite, 10, 10, nave.x, nave.y, ALLEGRO_PI / 2, 0);
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
    al_destroy_sample(sample);
    al_destroy_sample(sample_2);
    al_destroy_sample(sample_3);

    return 0;
}

void InicializaNave(Nave* nave) {
    nave->x = 0;
    nave->y = 600;
    nave->velocidade = 10;
}


void InitTiros(Tiro tiros[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        tiros[i].velocidade = 15;
        tiros[i].ativo = false;
    }
}

void AtirarTiros(Tiro tiros[], Nave nave, int tamanho, ALLEGRO_BITMAP* disparo) {
    for (int i = 0; i < tamanho; i++) {
        if (!tiros[i].ativo) {
            tiros[i].x = nave.x + (al_get_bitmap_width(disparo) / 2) - (al_get_bitmap_width(disparo) / 2);
            tiros[i].y = nave.y + 40;
            tiros[i].ativo = true;
            break;
        }
    }
}

void AtualizaTiros(Tiro tiros[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (tiros[i].ativo) {
            tiros[i].x += tiros[i].velocidade;
            if (tiros[i].x > largura) {
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

void InicializaMonstro(Monstro* monstro, int tamanho){
    for (int i = 0; i < tamanho; i++) {
        monstro[i].velocidade = 5;
        monstro[i].ativo = false;
        monstro[i].borda_x = 50;
        monstro[i].borda_y = 50;
        monstro[i].pontuacao = 100;
    }
}
void LiberaMonstros(Monstro monstro[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (!monstro[i].ativo){
            if (rand() % 500 == 0) {
                monstro[i].x = largura;
                monstro[i].y = rand() % (altura - 60);
                monstro[i].ativo = true;
                break;
            }
        }
    }
}
void AtualizaMonstros(Monstro monstro[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (monstro[i].ativo) {
            monstro[i].x -= monstro[i].velocidade;
            if (monstro[i].x < 0) {
                monstro[i].ativo = false;
            }
        }
    }
}
void DesenhaMonstros(Monstro monstro[], int tamanho, ALLEGRO_BITMAP* inimigo) {
    for (int i = 0; i < tamanho; i++) {
        if (monstro[i].ativo){
            al_draw_bitmap(inimigo, monstro[i].x, monstro[i].y, 20);
        }
    }
}
void BalaColidida(Tiro tiros[], int tamanho_tiro, Monstro monstro[], int tamanho_monstro, int * pontuacao) {
    for (int i = 0; i < tamanho_tiro; i++) {
        if (tiros[i].ativo) {
            for (int j = 0; j < tamanho_monstro; j++) {
                if (tiros[i].x < (monstro[j].x + monstro[j].borda_x) &&
                    tiros[i].x + 20 >(monstro[j].x) &&
                    tiros[i].y < (monstro[j].y + monstro[j].borda_y) &&
                    tiros[i].y + 20 > (monstro[j].y)) {
                    tiros[i].ativo = false;
                    monstro[j].ativo = false;
                    *pontuacao += monstro[i].pontuacao;
                    break;
                }
            }
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