typedef struct{
    int x;
    int y;
    int velocidade;
    int borda_x;
    int borda_y;
    int pontos;
    int vida;
}Nave;

typedef struct no{
    int dado;
    struct no *proximo;
}No;

typedef struct{
    int x;
    int y;
    int velocidade;
}Monstro;

typedef struct{
    int x;
    int y;
    int velocidade;
    bool ativo;
    int dano;
}Tiro;
