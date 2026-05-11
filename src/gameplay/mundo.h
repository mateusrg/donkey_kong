#pragma once

#include "../core/models.h"

/**
 * @brief Carrega a fase atual para dentro do estado principal do jogo
 * @param[in,out] jogo Estado principal que receberá o mapa e as entidades da fase
 * @return true se a fase foi carregada com sucesso
 * @return false se houve falha ao preparar a fase atual
 */
bool mundo_carregar_fase(Jogo* jogo);

/**
 * @brief Atualiza mundo, entidades e progresso da fase
 * @param[in,out] jogo Estado principal contendo mapa, jogador e inimigos
 * @param[in] comandos Comandos de entrada do jogador no frame atual
 * @param[in] delta_tempo Tempo decorrido desde o último frame em segundos
 */
void mundo_atualizar(Jogo* jogo, ComandosJogador comandos, float delta_tempo);

/**
 * @brief Informa se o jogador chegou até a porta de saída da fase
 * @param[in] jogo Estado principal consultado para verificar a condição de saída
 * @return true se o jogador alcançou a saída da fase
 * @return false se a fase ainda não foi concluída
 */
bool mundo_jogador_alcancou_saida(const Jogo* jogo);

/**
 * @brief Informa se algum inimigo ativo colidiu com o jogador na fase atual
 * @param[in] jogo Estado principal consultado para verificar colisões de perigo
 * @return true se algum inimigo atingiu o jogador
 * @return false se não houve colisão com inimigos
 */
bool mundo_jogador_colidiu_inimigo(const Jogo* jogo);
