#pragma once

#include "../core/models.h"

/**
 * @brief Inicializa a renderizacao das texturar
 */
void render_inicializar(void);
/**
 * @brief Desenha um frame completo da tela atual a partir do estado do jogo
 * @param[in] jogo Estado principal consultado para renderização
 */
void render_desenhar(const Jogo* jogo);
/**
 * @brief Encerra a renderizacao das texturas
 */
void render_encerrar(void);