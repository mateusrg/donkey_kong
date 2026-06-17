#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mapa_io.h"
#include "../core/constants.h"
#include "../core/models.h"

bool mapa_montar_caminho_fase(int fase_atual, char caminho_saida[]) {
    int resultado = snprintf(caminho_saida, TAMANHO_CAMINHO, "dados/mapas/mapa%d.txt", fase_atual);
    return (resultado > 0 && resultado < TAMANHO_CAMINHO);
}

bool mapa_carregar(const char caminho_arquivo[], Jogo* jogo) {
    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) return false;

    jogo->quantidade_inimigos = 0;
    jogo->quantidade_powerups = 0;

    // reseta donkey e princesa: se o novo mapa não tiver os tiles M/R,
    // eles devem ficar inativos, não herdar o estado da fase anterior
    jogo->donkey.ativo = false;
    jogo->princesa.ativo = false;

    // limpa todos os tiles antes de ler o novo arquivo
    // sem isso, tiles de fases anteriores ficam em posições não sobrescritas
    // quando a nova fase tem linhas mais curtas que a anterior
    memset(jogo->mapa.tiles, ' ', sizeof(jogo->mapa.tiles));

    int linha = 0, coluna = 0;
    char caractere;

    while ((caractere = fgetc(arquivo)) != EOF) {
        if (caractere == '\n') {
            linha++;
            coluna = 0;
            continue;
        }

        if (linha < MAPA_LINHAS && coluna < MAPA_COLUNAS) {
            switch (caractere) {
                case TILE_JOGADOR:
                    jogo->jogador.tile.linha = linha;
                    jogo->jogador.tile.coluna = coluna;
                    jogo->jogador.spawn_inicial.linha = linha;
                    jogo->jogador.spawn_inicial.coluna = coluna;
                    jogo->jogador.posicao_pixels.x = (float)(coluna * TILE_SIZE);
                    jogo->jogador.posicao_pixels.y = (float)(linha * TILE_SIZE);
                    jogo->mapa.tiles[linha][coluna] = ' ';
                    jogo->jogador.ativo = true;
                    break;

                case TILE_INIMIGO:
                    if (jogo->quantidade_inimigos < MAX_INIMIGOS) {
                        int i = jogo->quantidade_inimigos;
                        jogo->inimigos[i].tile.linha = linha;
                        jogo->inimigos[i].tile.coluna = coluna;
                        jogo->inimigos[i].spawn_inicial.linha = linha;
                        jogo->inimigos[i].spawn_inicial.coluna = coluna;
                        jogo->inimigos[i].posicao_pixels.x = (float)(coluna * TILE_SIZE);
                        jogo->inimigos[i].posicao_pixels.y = (float)(linha * TILE_SIZE);
                        jogo->inimigos[i].tipo = INIMIGO_PATRULHEIRO;
                        jogo->inimigos[i].ativo = true;
                        (jogo->quantidade_inimigos)++;
                    }
                    jogo->mapa.tiles[linha][coluna] = ' ';
                    break;

                case TILE_INIMIGO_VELOZ:
                    if (jogo->quantidade_inimigos < MAX_INIMIGOS) {
                        int iv = jogo->quantidade_inimigos;
                        jogo->inimigos[iv].tile.linha = linha;
                        jogo->inimigos[iv].tile.coluna = coluna;
                        jogo->inimigos[iv].spawn_inicial.linha = linha;
                        jogo->inimigos[iv].spawn_inicial.coluna = coluna;
                        jogo->inimigos[iv].posicao_pixels.x = (float)(coluna * TILE_SIZE);
                        jogo->inimigos[iv].posicao_pixels.y = (float)(linha * TILE_SIZE);
                        jogo->inimigos[iv].tipo = INIMIGO_VELOZ;
                        jogo->inimigos[iv].ativo = true;
                        (jogo->quantidade_inimigos)++;
                    }
                    jogo->mapa.tiles[linha][coluna] = ' ';
                    break;

                case TILE_POWERUP_TEMPO:
                case TILE_POWERUP_VIDA:
                case TILE_POWERUP_INVENCIVEL:
                    if (jogo->quantidade_powerups < MAX_POWERUPS) {
                        int p = jogo->quantidade_powerups;
                        jogo->powerups[p].posicao_pixels.x = (float)(coluna * TILE_SIZE);
                        jogo->powerups[p].posicao_pixels.y = (float)(linha * TILE_SIZE);
                        if (caractere == TILE_POWERUP_VIDA)
                            jogo->powerups[p].tipo = POWERUP_VIDA;
                        else if (caractere == TILE_POWERUP_INVENCIVEL)
                            jogo->powerups[p].tipo = POWERUP_INVENCIVEL;
                        else
                            jogo->powerups[p].tipo = POWERUP_TEMPO;
                        jogo->powerups[p].ativo = true;
                        (jogo->quantidade_powerups)++;
                    }
                    jogo->mapa.tiles[linha][coluna] = ' ';
                    break;

                case TILE_DONKEY:
                    jogo->donkey.tile.linha = linha;
                    jogo->donkey.tile.coluna = coluna;
                    jogo->donkey.spawn_inicial.linha = linha;
                    jogo->donkey.spawn_inicial.coluna = coluna;
                    jogo->donkey.posicao_pixels.x = (float)(coluna * TILE_SIZE);
                    jogo->donkey.posicao_pixels.y = (float)(linha * TILE_SIZE);
                    jogo->mapa.tiles[linha][coluna] = ' ';
                    jogo->donkey.ativo = true;
                    break;

                case TILE_PRINCESA:
                    jogo->princesa.tile.linha = linha;
                    jogo->princesa.tile.coluna = coluna;
                    jogo->princesa.spawn_inicial.linha = linha;
                    jogo->princesa.spawn_inicial.coluna = coluna;
                    jogo->princesa.posicao_pixels.x = (float)(coluna * TILE_SIZE);
                    jogo->princesa.posicao_pixels.y = (float)(linha * TILE_SIZE);
                    jogo->mapa.tiles[linha][coluna] = ' ';
                    jogo->princesa.ativo = true;
                    break;

                case TILE_CHAO:
                case TILE_ESCADA_SUBIDA:
                case TILE_ESCADA_DESCIDA:
                case TILE_ESCADA_PADRAO:
                case TILE_PORTA:
                    jogo->mapa.tiles[linha][coluna] = caractere;
                    break;

                default:
                    jogo->mapa.tiles[linha][coluna] = ' ';
                    break;
            }
            coluna++;
        }
    }
    fclose(arquivo);
    return true;
}
