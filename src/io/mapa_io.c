#include <stdio.h>
#include <stdbool.h>
#include "mapa_io.h"
#include "../core/constants.h"
#include "../core/models.h"

// Monta o nome do arquivo (Ex: mapa1.txt)
bool mapa_montar_caminho_fase(int fase_atual, char caminho_saida[]) {
    // snprintf escreve o texto "mapas/mapaX.txt" dentro da variável caminho_saida
    int resultado = snprintf(caminho_saida, TAMANHO_CAMINHO, "dados/mapas/mapa%d.txt", fase_atual);

    // Se o resultado for negativo ou maior que o tamanho do buffer, deu erro
    return (resultado > 0 && resultado < TAMANHO_CAMINHO);
}

bool mapa_carregar(const char caminho_arquivo[], Jogo* jogo) {
    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) return false;

    jogo->quantidade_inimigos = 0;
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
                    // I/O: Apenas traduz o que o arquivo diz sobre o Jogador (posições estáticas)
                    jogo -> jogador.tile.linha = linha;
                    jogo -> jogador.tile.coluna = coluna;
                    jogo-> jogador.spawn_inicial.linha = linha;
                    jogo -> jogador.spawn_inicial.coluna = coluna;
                    
                    // Posição que será atualizada do jogador
                    jogo -> jogador.posicao_pixels.x = (float)(coluna * TILE_SIZE);
                    jogo-> jogador.posicao_pixels.y = (float)(linha * TILE_SIZE);

                    jogo -> mapa.tiles[linha][coluna] = ' '; 
                    jogo -> jogador.ativo = true;
                    break;

                case TILE_INIMIGO:
                    // I/O: Apenas preenche os dados de localização do inimigo
                    if (jogo -> quantidade_inimigos < MAX_INIMIGOS) {
                        int i = jogo -> quantidade_inimigos;
                        jogo -> inimigos[i].tile.linha = linha;
                        jogo -> inimigos[i].tile.coluna = coluna;
                        jogo -> inimigos[i].spawn_inicial.linha = linha;
                        jogo -> inimigos[i].spawn_inicial.coluna = coluna;
                        jogo -> inimigos[i].posicao_pixels.x = (float)(coluna * TILE_SIZE);
                        jogo -> inimigos[i].posicao_pixels.y = (float)(linha * TILE_SIZE);
                        jogo -> inimigos[i].ativo = true;
                        
                        (jogo -> quantidade_inimigos)++;
                    }
                    jogo -> mapa.tiles[linha][coluna] = ' ';
                    break;
                case TILE_DONKEY:
                    jogo -> donkey.tile.linha = linha;
                    jogo -> donkey.tile.coluna = coluna;
                    jogo -> donkey.spawn_inicial.linha = linha;
                    jogo -> donkey.spawn_inicial.coluna = coluna;
                    jogo -> donkey.posicao_pixels.x = (float) (coluna * TILE_SIZE);
                    jogo -> donkey.posicao_pixels.y = (float) (linha * TILE_SIZE);
                    jogo -> mapa.tiles[linha][coluna] = ' ';
                    jogo -> donkey.ativo = true;
                    break;
                
                case TILE_PRINCESA:
                    jogo -> princesa.tile.linha = linha;
                    jogo -> princesa.tile.coluna = coluna;
                    jogo -> princesa.spawn_inicial.linha = linha;
                    jogo -> princesa.spawn_inicial.coluna = coluna;
                    jogo -> princesa.posicao_pixels.x = (float) (coluna * TILE_SIZE);
                    jogo -> princesa.posicao_pixels.y = (float) (linha * TILE_SIZE);
                    jogo -> mapa.tiles[linha][coluna] = ' ';
                    jogo -> princesa.ativo = true;
                    break;

                case TILE_CHAO: 
                case TILE_ESCADA_SUBIDA: 
                case TILE_ESCADA_DESCIDA: 
                case TILE_ESCADA_PADRAO: 
                case TILE_PORTA:
                    jogo -> mapa.tiles[linha][coluna] = caractere;
                    break;

                default:
                    jogo -> mapa.tiles[linha][coluna] = ' ';
                    break;
            }
            coluna++;
        }
    }
    fclose(arquivo);
    return true;
}