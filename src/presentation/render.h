#pragma once
#include "raylib.h"
#include "../core/models.h"

/**
 * @brief redimensiona as imagens estáticas que estão muito grandes
 * @param[in] Recebe o nome do arquivo desejado
 */
Texture2D redimensiona_imagem_estatica(const char* nome_arquivo);
/**
 * @brief Inicializa a renderizacao das texturas
 */
void render_inicializar(Jogo* jogo);

/**
 * @brief Desenha o fundo da tela
 * @param[in] Jogo Estado principal consultado para renderização
 */

void desenha_entidades (const Jogo* Jogo);

/**
 * @brief Desenha o fundo da tela
 * @param[in] Jogo Estado principal consultado para renderização
 */
void desenha_mapa  (const Jogo* Jogo);

/**
 * @brief Desenha um frame completo da tela atual a partir do estado do jogo
 * @param[in] jogo Estado principal consultado para renderização
 */

 void render_desenhar(Jogo* jogo);


/**
 * @brief Desenha o layout do menu de pausa (continuar, voltar para o menu ou sair do jogo)
 */

 void desenha_menu_pausa(void);


/**
 * @brief Encerra a renderizacao das texturas
 */
void render_encerrar(void);