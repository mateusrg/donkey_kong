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
void desenha_mapa  (const Jogo* jogo);

/**
 * @brief Desenha o tempo na tela se a tela atual for o jogo
 * @param[in, out] Jogo Estado principal consultado para armazenar o tempo de tela.
 */

void desenha_elementos_HUD(Jogo* jogo);

/**
 * @brief Desenha um frame completo da tela atual a partir do estado do jogo
 * @param[in, out] jogo Estado principal consultado para renderização
 */

 void render_desenhar(Jogo* jogo);


/**
 * @brief Desenha o layout do menu de pausa (continuar, voltar para o menu ou sair do jogo)
 */

 void desenha_menu_pausa(void);


/**
 * @brief Desenha tela de game over
 * @param[in] jogo Estado principal consultado
 * @param[in] fonte Fonte usada para desenhar o texto
 */
void desenha_game_over(const Jogo* jogo, Font fonte);

/**
 * @brief Desenha tela de vitoria
 * @param[in] jogo Estado principal consultado
 * @param[in] fonte Fonte usada para desenhar o texto
 */

void desenha_vitoria(const Jogo* jogo, Font fonte);

/**
 * @brief Desenha tela de transição entre fases
 * @details Exibe tela branca com mensagem informando a próxima fase.
 *          O João substituirá esta função por uma versão elaborada depois.
 * @param[in] jogo Estado principal consultado para saber o número da próxima fase
 * @param[in] fonte Fonte usada para desenhar o texto
 */
void desenha_proxima_fase(const Jogo* jogo, Font fonte);

/**
 * @brief Encerra a renderizacao das texturas
 */
void render_encerrar(void);