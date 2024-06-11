typedef struct Nave{
    int x;
    int y;
    int velocidade;
    int borda_x;
    int borda_y;
    int pontos;
    int vida;
    bool ativo;
}Nave;

typedef struct no{
    int dado;
    struct no *proximo;
}No;

typedef struct{
    int x;
    int y;
    int velocidade;
    int borda_x;
    int borda_y;
    int pontuacao;
    float rotacao;
    bool ativo;
}Monstro;

typedef struct {
    int x;
    int y;
    int velocidade;
    bool ativo;
    int dano;
}Tiro;

typedef struct HiScore
{
    long int pontuacao[10];
    int posicao[10];
    char* nome[10][3];
} HiScore;
