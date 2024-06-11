
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "hiscore.h"


#define MAXHISCORE 999999
#define NUM_HISCORES 10


bool testaHiscore(int* pontuacao, HiScore* hiscore) {
    int posicao = NUM_HISCORES + 1;

    printf("�ltima pontua��o: %d\n", hiscore->pontuacao[NUM_HISCORES - 1]);
    //  printf("%ld\n", *pontuacao);

    if (*pontuacao > hiscore->pontuacao[NUM_HISCORES - 1]) {
        // trocar para true
        return true;
    }
    else return false;
}

HiScore* armazenaHiscores(FILE* arquivo) {
    int i;
    HiScore* hiscore = (HiScore*)malloc(sizeof(HiScore) * NUM_HISCORES);

    for (i = 0; i < NUM_HISCORES; i++) {
        fscanf_s(arquivo, "%d %s %d", &hiscore->posicao[i], hiscore->nome[i], &hiscore->pontuacao[i]);
        //  printf("%d %s %ld\n", hiscore->posicao[i], hiscore->nome[i], hiscore->pontuacao[i]);
    }
    return hiscore;
}

void gravaHiscores(HiScore* hiscore) {

    FILE* arquivo_hiscores;
    int i;

    arquivo_hiscores = fopen_s(&arquivo_hiscores, "hiscores_atuais.txt", "w, ccs=UTF-8");
    for (i = 0; i < NUM_HISCORES; i++) {
        fprintf_s(arquivo_hiscores, "%d %s %d\n", hiscore->posicao[i], hiscore->nome[i], hiscore->pontuacao[i]);
    }

    fclose(arquivo_hiscores);
}

HiScore * inicializaHiScores()
{
    FILE* ptr;
   // int * pontuacao = (int*) malloc (sizeof(int));

    fopen_s(&ptr, "hiscores_iniciais.txt", "r, ccs=UTF-8");
    if (ptr == NULL) {
        perror;
    }
    else {
        HiScore* hiscore;

        hiscore = armazenaHiscores(ptr);
        fclose(ptr);

        return hiscore;
        //testaHiscore(pontuacao, hiscore);
       // gravaHiscores(hiscore);

    }   
}

