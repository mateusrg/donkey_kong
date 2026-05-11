#include "inimigo.h"

#include <string.h>

/**
 * @brief Converte um tile do mapa para a origem em pixels usada pelo inimigo
 * @param[in] tile Posição em coordenadas de mapa
 * @return Vetor com a posição equivalente em pixels
 */
static Vetor2D inimigo_tile_para_pixels(PosicaoMapa tile);

static Vetor2D inimigo_tile_para_pixels(PosicaoMapa tile)
{
    Vetor2D posicao_pixels;

    posicao_pixels.x = (float)(tile.coluna * TILE_SIZE);
    posicao_pixels.y = (float)(tile.linha * TILE_SIZE);

    return posicao_pixels;
}

void inimigo_inicializar(Inimigo* inimigo, PosicaoMapa spawn_inicial)
{
    if (inimigo)
    {
        memset(inimigo, 0, sizeof(Inimigo));
        inimigo->spawn_inicial = spawn_inicial;
        inimigo->tile = spawn_inicial;
        inimigo->posicao_pixels = inimigo_tile_para_pixels(spawn_inicial);
        inimigo->velocidade.x = VELOCIDADE_INIMIGO_PADRAO;
        inimigo->direcao_horizontal = DIRECAO_DIREITA;
        inimigo->ativo = true;
    }
}

void inimigo_atualizar(Inimigo* inimigo, const Mapa* mapa, float delta_tempo)
{
    (void)inimigo;
    (void)mapa;
    (void)delta_tempo;
}

bool inimigo_colide_com_jogador(const Inimigo* inimigo, const Jogador* jogador)
{
    (void)inimigo;
    (void)jogador;
    return false;
}