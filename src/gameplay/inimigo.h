#pragma once

#include "../core/models.h"

/**
 * @brief Inicializa o inimigo no spawn da fase com estado padrão de patrulha
 * @param[out] inimigo Estrutura do inimigo que será inicializada
 * @param[in] spawn_inicial Posição inicial do inimigo em coordenadas de tile
 * @param[in] tipo Tipo de comportamento do inimigo (patrulheiro ou veloz)
 */
void inimigo_inicializar(Inimigo* inimigo, PosicaoMapa spawn_inicial, TipoInimigo tipo);

/**
 * @brief Atualiza a movimentação do inimigo com base no mapa atual
 * @param[in,out] inimigo Estrutura do inimigo que será atualizada
 * @param[in] mapa Mapa usado para orientar colisões e patrulha
 * @param[in] delta_tempo Tempo decorrido desde o último frame em segundos
 */
void inimigo_atualizar(Inimigo* inimigo, const Mapa* mapa, float delta_tempo);

/**
 * @brief Verifica se a hitbox lógica do inimigo encosta no jogador informado
 * @param[in] inimigo Inimigo consultado na checagem de colisão
 * @param[in] jogador Jogador consultado na checagem de colisão
 * @return true se houver sobreposição entre inimigo e jogador
 * @return false se não existir colisão entre as entidades
 */
bool inimigo_colide_com_jogador(const Inimigo* inimigo, const Jogador* jogador);

/**
 * @brief Reposiciona o inimigo no spawn e restaura o estado de patrulha
 * @param[in,out] inimigo Estrutura do inimigo que será resetada
 * @param[in] spawn_inicial Posição de respawn em coordenadas de tile
 */
void inimigo_resetar(Inimigo* inimigo, PosicaoMapa spawn_inicial);
