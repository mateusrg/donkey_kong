#include <stdio.h>
#include <stdbool.h>
#include "mapa_io.h"
#include "constants.h"

// Monta o nome do arquivo (Ex: mapa1.txt)
bool mapa_montar_caminho_fase(int fase_atual, char caminho_saida[]) {
    // snprintf escreve o texto "mapas/mapaX.txt" dentro da variável caminho_saida
    int resultado = snprintf(caminho_saida, TAMANHO_CAMINHO, "mapas/mapa+%d.txt", fase_atual);

    // Se o resultado for negativo ou maior que o tamanho do buffer, deu erro
    return (resultado > 0 && resultado < TAMANHO_CAMINHO);
}

bool mapa_carregar(const char caminho_arquivo[], Mapa* mapa, Jogador* jogador, Inimigo* inimigos, int* quantidade_inimigos) {
    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) return false;

    *quantidade_inimigos = 0;
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
                case 'P':
                    // I/O: Apenas traduz o que o arquivo diz sobre o Jogador (posições estáticas)
                    jogador->tile.linha = linha;
                    jogador->tile.coluna = coluna;
                    jogador->spawn_inicial.linha = linha;
                    jogador->spawn_inicial.coluna = coluna;
                    
                    // Posição que será atualizada do jogador
                    jogador->posicao_pixels.x = (float)(coluna * TILE_SIZE);
                    jogador->posicao_pixels.y = (float)(linha * TILE_SIZE);

                    mapa->tiles[linha][coluna] = ' '; 
                    break;

                case 'E':
                    // I/O: Apenas preenche os dados de localização do inimigo
                    if (*quantidade_inimigos < MAX_INIMIGOS) {
                        int i = *quantidade_inimigos;
                        inimigos[i].tile.linha = linha;
                        inimigos[i].tile.coluna = coluna;
                        inimigos[i].spawn_inicial.linha = linha;
                        inimigos[i].spawn_inicial.coluna = coluna;
                        inimigos[i].posicao_pixels.x = (float)(coluna * TILE_SIZE);
                        inimigos[i].posicao_pixels.y = (float)(linha * TILE_SIZE);
                        
                        (*quantidade_inimigos)++;
                    }
                    mapa->tiles[linha][coluna] = ' ';
                    break;

                case 'Z': case 'S': case 'D': case 'H': case 'F':
                    mapa->tiles[linha][coluna] = caractere;
                    break;

                default:
                    mapa->tiles[linha][coluna] = ' ';
                    break;
            }
            coluna++;
        }
    }
    fclose(arquivo);
    return true;
}