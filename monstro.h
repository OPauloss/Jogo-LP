#define largura 1280
#define altura 720


void InicializaMonstro(Monstro monstro[], int tamanho);
void LiberaMonstros(Monstro monstro[], int tamanho, ALLEGRO_BITMAP* inimigo);
void AtualizaMonstros(Monstro monstro[], int tamanho);
void DesenhaMonstros(Monstro monstro[], int tamanho, ALLEGRO_BITMAP* inimigo, int* pontuacao);
void BalaColidida(Tiro tiros[], int tamanho_tiro, Monstro monstro[], int tamanho_monstro, int* pontuacao);
void NaveColidida(Monstro monstro[], int tamanho_monstro, Nave* nave, int* pontuacao);