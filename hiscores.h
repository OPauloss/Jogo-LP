int testaHiscore(long int *pontuacao, HiScore * hiscore);
HiScore * atualizaHiscores (HiScore ** hiscore, long int *pontuacao, char *nome, int posicao);
HiScore * armazenaHiscores (FILE * arquivo, HiScore ** hiscore);
void gravaHiscores(HiScore ** hiscore);
void exibeHiscores (HiScore * hiscore);