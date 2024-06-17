#include "monstro.h"

void InicializaMonstro(Monstro* monstro, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        monstro[i].velocidade = 5;
        monstro[i].ativo = false;
        monstro[i].borda_x = 50;
        monstro[i].borda_y = 50;
        monstro[i].pontuacao = 100;
    }
}
void LiberaMonstros(Monstro monstro[], int tamanho , ALLEGRO_BITMAP* inimigo) {
    for (int i = 0; i < tamanho; i++) {
        if (!monstro[i].ativo) {
            if (rand() % 500 == 0) {
                monstro[i].x = largura;
                monstro[i].y = rand() % (altura);
                monstro[i].borda_x = 50;
                monstro[i].borda_y = 50;
                monstro[i].ativo = true;
                if (monstro[i].velocidade < 60) {
                    monstro[i].velocidade *= (1.3);
                } else
                    monstro[i].velocidade = 60;         
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
void DesenhaMonstros(Monstro monstro[], int tamanho, ALLEGRO_BITMAP* inimigo, int* pontuacao) {
    for (int i = 0; i < tamanho; i++) {
        if (monstro[i].ativo) {
            al_draw_bitmap(inimigo, monstro[i].x, monstro[i].y, 20);
        }
    }
}
