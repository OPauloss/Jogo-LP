#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "objetos.h"

// Pôr no main
#define MAXHISCORE 999999
#define NUM_HISCORES 10

typedef struct HiScore
{
	long int pontuacao[NUM_HISCORES + 1];
	int posicao[NUM_HISCORES + 1];
	char* nome[NUM_HISCORES + 1][3];
} HiScore;

int
testaHiscore(long int* pontuacao, HiScore* hiscore)
{
	int i, posicao = -1;

	for (i = 0; i < NUM_HISCORES; i++)
	{
		if (*pontuacao > hiscore->pontuacao[i])
		{
			posicao = i;
			break;
		}
	}
	return posicao;
}

HiScore*
atualizaHiscores(HiScore** hiscore, long int* pontuacao, char* nome,
	int posicao)
{
	int i;
	HiScore* hiscore_local = *hiscore;
	
	for (i = NUM_HISCORES; i >= posicao; i--)
	{
		hiscore_local->pontuacao[i + 1] = hiscore_local->pontuacao[i];
		strcpy((char*)hiscore_local->nome[i + 1], (char*)hiscore_local->nome[i]);
		//	 strcpy (char2, char1);
	}

	// Inclui o novo hiscore
	hiscore_local->pontuacao[posicao] = *pontuacao;
	strcpy((char*) hiscore_local->nome[posicao], (char*)nome);

	return hiscore_local;
}

HiScore*
armazenaHiscores(FILE* arquivo, HiScore** hiscore)
{
	int i;
	HiScore* hiscore_local = *hiscore;

	for (i = 0; i < NUM_HISCORES; i++)
	{
		fscanf(arquivo, "%d %s %ld\n", &hiscore_local->posicao[i],
			(char*) hiscore_local->nome[i], &hiscore_local->pontuacao[i]);
		//printf ("%d %s %ld\n", hiscore_local->posicao[i],
		  //	  hiscore_local->nome[i], hiscore_local->pontuacao[i]);
	}
	return hiscore_local;
}

void
gravaHiscores(HiScore** hiscore)
{

	FILE* arquivo_hiscores;
	int i;
	HiScore* hiscore_local = *hiscore;

	arquivo_hiscores = fopen_s(&arquivo_hiscores, "hiscores_atuais.txt", "w");
	for (i = 0; i < NUM_HISCORES; i++)
	{
		fprintf(arquivo_hiscores, "%d %s %ld\n", i,
			(char*) hiscore_local->nome[i], hiscore_local->pontuacao[i]);
	}
	fclose(arquivo_hiscores);
}

char * exibeHiscores(HiScore* hiscore)
{
	int i;

	for (i = 0; i < NUM_HISCORES; i++)
	{
	printf("%d %s %ld\n", hiscore->posicao[i], (char*) hiscore->nome[i], hiscore->pontuacao[i]);
	}	
}

