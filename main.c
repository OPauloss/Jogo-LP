#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_acodec.h>
#include "objetos.h"
#include <stdio.h>
#include <stdlib.h>
#include "monstro.c"
#include "monstro.h"
#include "disparo_nave.c"
#include "disparo_nave.h"
#include "hiscores.h"
#include <math.h>


#define NUM_TIRO 20
#define NUM_MONSTROS 12
#define largura 1280
#define altura 720
#define FPS 60
#define MAXHISCORE 999999
#define NUM_HISCORES 10
#define MAX_VIDA 3

// PROT�TIPOS
void InicializaNave(Nave* nave);
void telaInicial(bool verificador, ALLEGRO_BITMAP* bgMenu);
void telaGameOver(bool verificador, ALLEGRO_BITMAP* bgGameOver);
//int testaHighScore(long int *pontuacao);

int main(int argc, char * argv[]) {
    // VARI�VEIS DO JOGO
    Nave nave;
    Monstro monstro[NUM_MONSTROS];
    Tiro tiro[NUM_TIRO];
    int i;
    char* criatura = "./mon1.png";
    char* meteoro = "./mon.png";
    char* sprite_inimigo = meteoro;

    FILE* arquivo_inicial;
    FILE* arquivo_hiscores_atualizados;

    arquivo_inicial = fopen("hiscore_inicial.txt", "r");
    if (ferror(arquivo_inicial))
    {
        printf("Erro ao abrir o arquivo de Hiscores iniciais");
    }

    arquivo_hiscores_atualizados = fopen("hiscores_atuais.txt", "r");
    if (ferror(arquivo_hiscores_atualizados))
    {
        printf("Arquivo de Hiscores atuais n�o encontrado.");
    }
    else arquivo_inicial = arquivo_hiscores_atualizados;


    HiScore* hiscore = (HiScore*)malloc(sizeof(HiScore) * NUM_HISCORES + 1);
    hiscore = armazenaHiscores(arquivo_inicial, &hiscore);
    char* player_hiscore = "HAI";
    fclose(arquivo_hiscores_atualizados);
 
    int* pontuacao = (int*)malloc(sizeof(int));
    if (pontuacao != NULL) {
        *pontuacao = 0;
    }

    InicializaNave(&nave);
    InicializaMonstro(monstro, NUM_MONSTROS);
    InitTiros(tiro, NUM_TIRO);
    //Verificador inicializacoes 
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    if (!al_install_audio()) {
        fprintf(stderr, "Falha ao inicializar o �udio.\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar os codecs de �udio.\n");
        return -1;
    }

    if (!al_reserve_samples(5)) {
        fprintf(stderr, "Falha ao reservar samples de �udio.\n");
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

    // CRIA��O DE DISPLAY E OUTROS COMPONENTES
    ALLEGRO_DISPLAY* display = al_create_display(largura, altura);
    if (!display) {
        fprintf(stderr, "Falha ao criar o display.\n");
        return -1;
    }

    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Star Fighter");

    // INICIALIZA��O DAS FONTES
    al_init_font_addon();
    al_init_ttf_addon();


    ALLEGRO_FONT* font = al_load_font("./font.ttf", 25, 0);
    if (!font) {
        fprintf(stderr, "Falha ao carregar a fonte.\n");
        return -1;
    }

    ALLEGRO_FONT* font_arcade = al_load_font("./ARCADE_I.TTF", 25, 0);
    if (!font_arcade) {
        fprintf(stderr, "Falha ao carregar a fonte.\n");
        return -1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Falha ao criar o timer.\n");
        return -1;
    }

    //DEFINI��ES DE BITMAP
    ALLEGRO_BITMAP* sprite = al_load_bitmap("./nave_resized.png");
    if (!sprite) {
        fprintf(stderr, "Falha ao carregar o sprite da nave.\n");
        return -1;
    }

    
    if (argv[1] != NULL && strcmp("monstro", argv[1])) {
        sprite_inimigo = criatura;
    }
    else {
        sprite_inimigo = meteoro;
    } 

     ALLEGRO_BITMAP* inimigo = al_load_bitmap(sprite_inimigo);
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
        fprintf(stderr, "Falha ao carregar o background.\n");                                                //TROQUEI BACKGROUND DO bg, bgGameOver, bgMenu e scoreMenu
        return -1;
    }

    ALLEGRO_BITMAP* bgGameOver = al_load_bitmap("./gameOver.jpg");
    if (!bgGameOver) {
        fprintf(stderr, "Falha ao carregar o background do GameOver.\n");
        return -1;
    }
    ALLEGRO_BITMAP* bgMenu = al_load_bitmap("./bgMenu.png");

    if (!bgMenu) {
        fprintf(stderr, "Falha ao carregar o background do Menu.\n");
        return -1;
    }

    ALLEGRO_BITMAP* scoreMenu = al_load_bitmap("./Score.png");
    if (!scoreMenu) {
        fprintf(stderr, "Falha ao carregar o background do Score.\n");
        return -1;
    }

    ALLEGRO_BITMAP* vida = al_load_bitmap("./vida.png");
    if (!scoreMenu) {
        fprintf(stderr, "Falha ao carregar o bitmap da vida.\n");
        return -1;
    }

    // DEFINI��ES DE SAMPLE
    ALLEGRO_SAMPLE* sample = al_load_sample("./background.wav"); //SOM BACKGROUND                               =============SAMPLES ADICIONADOS===============
    ALLEGRO_SAMPLE* sample_2 = al_load_sample("./disparo-sound.wav"); // SOM DISPARO
    ALLEGRO_SAMPLE* sample_3 = al_load_sample("./colisao.wav"); // SOM COLISAO
    ALLEGRO_SAMPLE* sample_4 = al_load_sample("./menusound.wav");  // SOM MENU
    ALLEGRO_SAMPLE* sample_5 = al_load_sample("./gameOver.wav");    // SOM GAME OVER
    ALLEGRO_SAMPLE* sample_6 = al_load_sample("./211634__qubodup__damage.flac"); // SOM DANO
    /*    Damage http ://www.freesound.org/people/qubodup/sounds/211634 by Iwan �qubodup� Gabovitch
    http://freesound.org/people/qubodup under CC-BY 3.0 License http://creativecommons.org/licenses/by/3.0/legalcode */
    ALLEGRO_SAMPLE_ID sample_id_4;

    if (!sample) {
        fprintf(stderr, "Falha ao carregar o �udio.\n");
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

    bool key_up = false;
    bool key_down = false;
    bool key_left = false;
    bool key_right = false;

    bool telainicial = true;
    bool running = true;
    bool telaScore = false;
    bool telagameOver = false;

    // INICIA A MUSICA DO MENU
    al_play_sample(sample_4, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sample_id_4);

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (telainicial) {          // COMO TELA INICIAL
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    telainicial = false;
                    al_stop_sample(&sample_id_4);
                    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_S) {
                    telainicial = false;
                    telaScore = true;
                }
            }
           telaInicial(telainicial, bgMenu);
        }
        else if (telaScore) { //  TELA DE SCORE
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(scoreMenu, (1280 - largura) / 2, (720 - altura) / 2, 0);

            for (i = 0; i < NUM_HISCORES; i++) {
                al_draw_textf(font_arcade, al_map_rgb(30, 30, 30), largura / 3 + 60, 260 + 40 * i, 0,
                    "%d %ld %s", (int)i + 1, (long int) hiscore->pontuacao[i], (char*) hiscore->nome[i]);
                al_draw_textf(font_arcade, al_map_rgb(255, 180, 254), largura / 3 + 58, 258 +40*i, 0,
                    "%d %ld %s", (int)i + 1, (long int) hiscore->pontuacao[i], (char*) hiscore->nome[i]);
                
            }            

            al_flip_display();
            if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
            {
                telaScore = false;
                telainicial = true;
                telaInicial(telaInicial, bgMenu);
            }

        }
        else if (telagameOver) { // TELA DE GAME OVER
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }

          

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(bgGameOver, (1280 - largura) / 2, (720 - altura) / 2, 0);
            al_flip_display();
            
           // printf("%d %ld\n", testaHiscore(pontuacao, hiscore) , *pontuacao);
            
            
            if (testaHiscore(pontuacao, hiscore) != -1) {
                hiscore = atualizaHiscores(&hiscore, pontuacao, player_hiscore, testaHiscore(pontuacao, hiscore));
                gravaHiscores(&hiscore);
            }     


            if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {       
                telagameOver = false;
                telainicial = true;
                key_up = false;
                key_down = false;
                key_left = false;
                key_right = false;

                *pontuacao = 0;

                InicializaNave(&nave);                     

                telaInicial(telaInicial, bgMenu);
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;


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
                al_play_sample(sample_2, 0.5, 0.0, 3.0, ALLEGRO_PLAYMODE_ONCE, NULL);//0.5 � o som e 3.0 � a velocidade
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
                nave.centro_y -= nave.velocidade;
                if (nave.y < 0) nave.y = 0;
            }
            if (key_down) {
                nave.y += nave.velocidade;
                nave.centro_y += nave.velocidade;
                if (nave.y > altura - al_get_bitmap_height(sprite)) nave.y = altura - al_get_bitmap_height(sprite);
            }
            if (key_left) {
                nave.x -= nave.velocidade;
                nave.centro_x -= nave.velocidade;
                if (nave.x < 0) nave.x = 0;
            }
            if (key_right) {
                nave.x += nave.velocidade;
                nave.centro_y += nave.velocidade;
                if (nave.x > largura - al_get_bitmap_width(sprite)) nave.x = largura - al_get_bitmap_width(sprite);
            }

            AtualizaTiros(tiro, NUM_TIRO);
            LiberaMonstros(monstro, NUM_MONSTROS, inimigo);
            AtualizaMonstros(monstro, NUM_MONSTROS);
            BalaColidida(tiro, NUM_TIRO, monstro, NUM_MONSTROS, pontuacao);
            int naveColidida = NaveColidida(monstro, NUM_MONSTROS, &nave, pontuacao); 
            if (naveColidida == 2) {
                al_play_sample(sample_6, 0.6, 0.0, 2.5, ALLEGRO_PLAYMODE_ONCE, NULL); //0.5 � o som e 3.0 � a velocidade
            }
            else if (naveColidida) {
                telagameOver = true;
                InicializaMonstro(monstro, NUM_MONSTROS);
                al_stop_samples(); // Para qualquer m�sica ou som em andamento
                al_play_sample(sample_5, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Toca o som de Game Over
            }

            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_bitmap(bg, 0, 0, 0);
            if (pontuacao != NULL) {
                al_draw_textf(font, al_map_rgb(255, 255, 255), 5, 5, 0, " SCORE: %d", *pontuacao);
            }

            DesenhaMonstros(monstro, NUM_MONSTROS, inimigo, pontuacao);

            // DESENHA A NAVE
            if (nave.ativo) {
                al_draw_rotated_bitmap(sprite, 10, 10, nave.x, nave.y, ALLEGRO_PI / 2, 0);
                if (nave.ativo) {
                    for (i = 0; i < nave.vida; i++) {
                        al_draw_bitmap(vida, 250+40*i, 5, 200, 50);
                    }                   
                }           
             }
            DesenhaTiros(tiro, NUM_TIRO, disparo);
            al_flip_display();
        }
    }

    // Encerra o jogo, libera a mem�ria
    free(hiscore);
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
    nave->x = 100;
    nave->y = 500;
    nave->borda_x = 50;
    nave->borda_y = 50;
    nave->centro_x = 50 / 2;
    nave->centro_y = 50 / 2;
    nave->vida = MAX_VIDA;
    nave->velocidade = 10;
    nave->ativo = true;
}

void BalaColidida(Tiro tiros[], int tamanho_tiro, Monstro monstro[], int tamanho_monstro, int* pontuacao) {
    for (int i = 0; i < tamanho_tiro; i++) {
        if (tiros[i].ativo) {
            for (int j = 0; j < tamanho_monstro; j++) {
                if (tiros[i].x < (monstro[j].x + monstro[j].borda_x) &&
                    tiros[i].x + 20 >(monstro[j].x) &&
                    tiros[i].y < (monstro[j].y + monstro[j].borda_y) &&
                    tiros[i].y + 20 >(monstro[j].y) && monstro[j].ativo) {
                    tiros[i].ativo = false;
                    monstro[j].ativo = false;
                    *pontuacao += monstro[i].pontuacao * monstro[i].velocidade/10;
                }
            }
        }
    }
}

int NaveColidida(Monstro monstro[], int tamanho_monstro, Nave* nave, int* pontuacao) { 
    for (int i = 0; i < tamanho_monstro; i++) {
        if (monstro[i].ativo) {
            
            if ((monstro[i].x - monstro[i].borda_x + 100) < (nave->x + nave->borda_x) &&
                (monstro[i].x + monstro[i].borda_x) > (nave->x - nave->borda_x) &&
                (monstro[i].y - monstro[i].borda_y) < (nave->y + nave->borda_y) &&
                (monstro[i].y + monstro[i].borda_y) > (nave->y - nave->borda_y)) {
                //printf("Colidiu! %d %d %d %d", nave->x, nave->y, monstro[i].x, monstro[i].y);
                
            // Colis�o por dis�ncia euclidiana
            //if (sqrt(pow(monstro[i].centro_x - nave->centro_x, 2) + pow(monstro[i].centro_y - nave->centro_y, 2)) < 50) {

                monstro[i].ativo = false;

                nave->vida--;
                if (nave->vida == 0) {
                    nave->ativo = false;
                    return 1;
                }
                else return 2; // Pra indicar que a dano sofreu dano
             }
        }
    }
    return 0;
}

void telaInicial(bool verificador, ALLEGRO_BITMAP* bgMenu) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(bgMenu, (1280 - largura) / 2, (720 - altura) / 2, 0);
    al_flip_display();
}
