#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "objetos.h"

// PC4r no main
#define MAXHISCORE 999999
#define NUM_HISCORES 10

// PC4r em objetos.h
typedef struct HiScore
{
  long int pontuacao[NUM_HISCORES + 1];
  int posicao[NUM_HISCORES + 1];
  char *nome[NUM_HISCORES + 1][3];
} HiScore;

int
testaHiscore (long int *pontuacao, HiScore * hiscore)
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

HiScore *
atualizaHiscores (HiScore ** hiscore, long int *pontuacao, char *nome,
				  int posicao)
{
  int i;
  HiScore * hiscore_local = *hiscore;

  for (i = NUM_HISCORES; i >= posicao; i--)
	{
	  hiscore_local->pontuacao[i+1] = hiscore_local->pontuacao[i];
	  strcpy(hiscore_local->nome[i+1], hiscore_local->nome[i]);
	}
	
	// Inclui o novo hiscore
	hiscore_local->pontuacao[posicao] = *pontuacao;
    strcpy(hiscore_local->nome[posicao], nome);
	
	return hiscore_local;
}

HiScore *
armazenaHiscores (FILE * arquivo, HiScore ** hiscore)
{
  int i;
  HiScore *hiscore_local = *hiscore;

  for (i = 0; i < NUM_HISCORES; i++)
	{
	  fscanf (arquivo, "%d %s %ld", &hiscore_local->posicao[i],
			  hiscore_local->nome[i], &hiscore_local->pontuacao[i]);
	 printf ("%d %s %ld\n", hiscore_local->posicao[i],
			  hiscore_local->nome[i], hiscore_local->pontuacao[i]);
	}
  return hiscore_local;
}

void
gravaHiscores (HiScore ** hiscore)
{

  FILE *arquivo_hiscores;
  int i;
  HiScore * hiscore_local = * hiscore;

  arquivo_hiscores = fopen ("hiscores_atuais.txt", "w");
  for (i = 0; i < NUM_HISCORES; i++)
	{

	  fprintf (arquivo_hiscores, "%d %s %ld\n", i+1,
			   hiscore_local->nome[i], hiscore_local->pontuacao[i]);
	}

  fclose (arquivo_hiscores);

}

void
exibeHiscores (HiScore * hiscore)
{

  int i;

  for (i = 0; i < NUM_HISCORES; i++)
	{
	  printf ("%d %s %ld\n", hiscore->posicao[i], hiscore->nome[i], hiscore->pontuacao[i]);
	}
}

int
main ()
{
  FILE *arquivo_inicial;

    long int *pontuacao = (long int *) malloc (sizeof (long int));
    *pontuacao = 7001;
    char *nome = "HAI";

  arquivo_inicial = fopen ("hiscore_inicial.txt", "r");
  if (arquivo_inicial == NULL)
	{
	  ferror;
	  printf ("Erro");
	}

  HiScore *hiscore = (HiScore *) malloc (sizeof (HiScore) * NUM_HISCORES + 1);
  hiscore = armazenaHiscores (arquivo_inicial, &hiscore);

  // printf("%d\n", hiscore->posicao[6]);
  //  printf("%s\n", hiscore->nome[6]);

  hiscore = atualizaHiscores (&hiscore, pontuacao, nome, 3);
  //   printf("%ld\n", hiscore->pontuacao[6]);
    puts("APós atualizado");
   exibeHiscores (hiscore);
   gravaHiscores (&hiscore);
   exibeHiscores (hiscore);

  fclose (arquivo_inicial);
}
