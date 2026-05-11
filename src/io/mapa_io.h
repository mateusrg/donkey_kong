#pragma once

#include <stdbool.h>

#include "../core/models.h"

/**
 * @brief Monta o caminho do arquivo correspondente à fase atual
 * @param[in] fase_atual Número da fase desejada
 * @param[out] caminho_saida Buffer que receberá o caminho montado
 * @param[in] tamanho_caminho_saida Capacidade total do buffer de saída
 * @return true se o caminho couber no buffer informado
 * @return false se não houver espaço suficiente para montar o caminho
 */
bool mapa_montar_caminho_fase(int fase_atual, char* caminho_saida, int tamanho_caminho_saida);

/**
 * @brief Lê o mapa de disco e preenche layout, spawn e inimigos iniciais
 * @param[in] caminho_arquivo Caminho do arquivo de mapa que será aberto
 * @param[out] mapa Estrutura de mapa que receberá os tiles carregados
 * @param[out] inimigos Vetor que receberá os inimigos encontrados no arquivo
 * @param[out] quantidade_inimigos Quantidade de inimigos preenchidos no vetor
 * @return true se o mapa foi lido e interpretado com sucesso
 * @return false se houve falha de leitura ou formato inválido
 */
bool mapa_carregar(const char* caminho_arquivo, Mapa* mapa, Inimigo* inimigos, int* quantidade_inimigos);
