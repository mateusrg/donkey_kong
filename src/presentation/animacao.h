#pragma once
#include "raylib.h"
#include "../core/models.h"


/**
 * @brief Função responsável pelo looping da animação
 * @param[in] animacaoPersonagem recebe os dados necessários da animação (frame atual, velocidade, ultimo frame, etc..)
 */

void animation_update(Animacao *animacaoPersonagem);

/**
 * @brief aplica a animation_update em varias entidades
 * @param[in] jogo 
 */

 void atualiza_anim_entidades(Jogo* jogo);

/**
 * @brief Função responsável por selecionar quais serão os frames do looping
 * @param[in] animacaoPersonagem Recebe os dados necessários da animação (frame atual, velocidade, ultimo frame, etc..)
 * @param[in] numero_frames_por_linha Informa para a função quantos frames a imagem possui no total
 */
Rectangle animation_frame(const Animacao *animacaoPersonagem, int numero_frames_por_linha, const DirecaoHorizontal DirecaoHorizontal);

