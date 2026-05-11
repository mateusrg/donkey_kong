#include "placar_io.h"

bool placar_carregar(const char* caminho_arquivo, TipoPlacar* placar, int capacidade)
{
    (void)caminho_arquivo;
    (void)placar;
    (void)capacidade;
    return false;
}

bool placar_salvar(const char* caminho_arquivo, const TipoPlacar* placar, int capacidade)
{
    (void)caminho_arquivo;
    (void)placar;
    (void)capacidade;
    return false;
}

bool placar_elegivel(const TipoPlacar* placar, int capacidade, int novo_tempo)
{
    (void)placar;
    (void)capacidade;
    (void)novo_tempo;
    return false;
}

void placar_inserir(TipoPlacar* placar, int capacidade, const char* nome, int novo_tempo)
{
    (void)placar;
    (void)capacidade;
    (void)nome;
    (void)novo_tempo;
}