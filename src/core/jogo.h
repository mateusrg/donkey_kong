#pragma once

#include "models.h"

/**
 * @brief Inicializa o estado base do jogo e abre a aplicação no menu principal
 * @param[out] jogo Estado principal que será preparado para o início da execução
 */
void jogo_inicializar(Jogo* jogo);

/**
 * @brief Atualiza a máquina de estados principal do jogo
 * @param[in,out] jogo Estado principal que será lido e alterado nesta atualização
 * @param[in] delta_tempo Tempo decorrido desde o último frame em segundos
 */
void jogo_atualizar(Jogo* jogo, float delta_tempo);

/**
 * @brief Reinicia a partida inteira a partir do estado inicial
 * @param[in,out] jogo Estado principal que será restaurado para uma nova partida
 */
void jogo_reiniciar_partida(Jogo* jogo);

/**
 * @brief Reinicia apenas a fase atual e reposiciona as entidades dessa fase
 * @param[in,out] jogo Estado principal que contém a fase em andamento
 */
void jogo_reiniciar_fase_atual(Jogo* jogo);

/**
 * @brief Encerra o estado principal e libera os recursos associados ao jogo
 * @param[in,out] jogo Estado principal que será finalizado
 */
void jogo_encerrar(Jogo* jogo);
