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
