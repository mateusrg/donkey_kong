#include "jogador.h"

#include <string.h>

/**
 * @brief Converte um tile do mapa para a origem em pixels usada pelo jogador
 * @param[in] tile Posição em coordenadas de mapa
 * @return Vetor com a posição equivalente em pixels
 */
static Vetor2D jogador_tile_para_pixels(PosicaoMapa tile);

static Vetor2D jogador_tile_para_pixels(PosicaoMapa tile)
{
    Vetor2D posicao_pixels;

    posicao_pixels.x = (float)(tile.coluna * TILE_SIZE);
    posicao_pixels.y = (float)(tile.linha * TILE_SIZE);

    return posicao_pixels;
}

void jogador_inicializar(Jogador* jogador, PosicaoMapa spawn_inicial)
{
    if (jogador)
    {
        memset(jogador, 0, sizeof(Jogador));
        jogador->spawn_inicial = spawn_inicial;
        jogador->tile = spawn_inicial;
        jogador->posicao_pixels = jogador_tile_para_pixels(spawn_inicial);
        jogador->aceleracao.y = GRAVIDADE_PADRAO;
        jogador->direcao_horizontal = DIRECAO_PARADO;
        jogador->vidas = VIDAS_INICIAIS;
        jogador->ativo = true;
        jogador->esta_no_chao = true;
    }
}

void jogador_resetar(Jogador* jogador, PosicaoMapa spawn_inicial)
{
    if (jogador)
    {
        jogador->spawn_inicial = spawn_inicial;
        jogador->tile = spawn_inicial;
        jogador->posicao_pixels = jogador_tile_para_pixels(spawn_inicial);
        jogador->velocidade.x = 0.0f;
        jogador->velocidade.y = 0.0f;
        jogador->aceleracao.x = 0.0f;
        jogador->aceleracao.y = GRAVIDADE_PADRAO;
        jogador->direcao_horizontal = DIRECAO_PARADO;
        jogador->ativo = true;
        jogador->esta_no_chao = true;
        jogador->esta_em_escada = false;
        jogador->esta_pulando = false;
    }
}

void jogador_atualizar(Jogador* jogador, const Mapa* mapa, ComandosJogador comandos, float delta_tempo)
{
    (void)jogador;
    (void)mapa;
    (void)comandos;
    (void)delta_tempo;
}

bool jogador_esta_sobre_plataforma(const Jogador* jogador, const Mapa* mapa)
{
    (void)jogador;
    (void)mapa;
    return false;
}