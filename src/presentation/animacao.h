#pragma once
#include "raylib.h"
#include "../core/models.h"

/**
 * @brief Faz o looping da animação entre first e last
 * @param[in] animacaoPersonagem Dados da animação (frame atual, velocidade, último frame, etc.)
 */
void atualiza_animacao(Animacao *animacaoPersonagem);

/**
 * @brief Reinicia os valores padrão de uma animação para o estado inicial
 * @param[in,out] animacaoPersonagem Animação que será reinicializada
 */
void animacao_inicializar(Animacao *animacaoPersonagem);

/**
 * @brief Aplica atualiza_animacao em várias entidades do jogo
 * @param[in] jogo Estado principal com as entidades
 */
void atualiza_anim_entidades(Jogo* jogo);

/**
 * @brief Seleciona o retângulo de corte do frame atual na imagem
 * @param[in] animacaoPersonagem Dados da animação (frame atual, velocidade, último frame, etc.)
 * @param[in] numero_frames_por_linha Quantidade de frames por linha na imagem
 * @param[in] DirecaoHorizontal Direção para espelhar o frame
 * @return Retângulo de corte no sprite sheet
 */
Rectangle tamanho_frames(const Animacao *animacaoPersonagem, int numero_frames_por_linha, const DirecaoHorizontal DirecaoHorizontal);
