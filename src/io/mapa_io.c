#include "mapa_io.h"

bool mapa_montar_caminho_fase(int fase_atual, char* caminho_saida, int tamanho_caminho_saida)
{
    (void)fase_atual;
    (void)caminho_saida;
    (void)tamanho_caminho_saida;
    return true;
}

bool mapa_carregar(const char* caminho_arquivo, Mapa* mapa, Inimigo* inimigos, int* quantidade_inimigos)
{
    (void)caminho_arquivo;
    (void)mapa;
    (void)inimigos;
    (void)quantidade_inimigos;
    return TILE_VAZIO;
}