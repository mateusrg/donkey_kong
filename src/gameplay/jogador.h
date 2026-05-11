#pragma once

#include "../core/models.h"

/**
 * @brief Prepara o jogador para a fase usando o tile de spawn informado
 * @param[out] jogador Estrutura do jogador que será inicializada
 * @param[in] spawn_inicial Posição inicial do jogador em coordenadas de tile
 */
void jogador_inicializar(Jogador* jogador, PosicaoMapa spawn_inicial);

/**
 * @brief Recoloca o jogador no spawn e limpa o estado temporário de movimento
 * @param[in,out] jogador Estrutura do jogador que será resetada
 * @param[in] spawn_inicial Posição de respawn em coordenadas de tile
 */
void jogador_resetar(Jogador* jogador, PosicaoMapa spawn_inicial);

/**
 * @brief Atualiza movimento e estados do jogador
 * @param[in,out] jogador Estrutura do jogador que será atualizada
 * @param[in] mapa Mapa atual usado para colisões e validações de movimento
 * @param[in] comandos Comandos de entrada do frame atual
 * @param[in] delta_tempo Tempo decorrido desde o último frame em segundos
 */
void jogador_atualizar(Jogador* jogador, const Mapa* mapa, ComandosJogador comandos, float delta_tempo);

/**
 * @brief Informa se existe uma plataforma sustentando o jogador no mapa atual
 * @param[in] jogador Estado atual do jogador
 * @param[in] mapa Mapa consultado para verificar suporte abaixo do jogador
 * @return true se houver uma plataforma sustentando o jogador
 * @return false se o jogador estiver sem apoio no tile abaixo
 */
bool jogador_esta_sobre_plataforma(const Jogador* jogador, const Mapa* mapa);
