#pragma once
#include "raylib.h"
#include "../core/models.h"

/**
 * @brief Redimensiona as imagens estáticas que estão muito grandes
 * @param[in] nome_arquivo Nome do arquivo desejado
 * @return Textura carregada e redimensionada para o tamanho do tile
 */
Texture2D redimensiona_imagem_estatica(const char* nome_arquivo);

/**
 * @brief Inicializa a renderização das texturas
 */
void render_inicializar(Jogo* jogo);

/**
 * @brief Reinicia as structs de animação dos inimigos carregados na fase atual
 * @param[in,out] jogo Estado principal com os inimigos cujas animações serão resetadas
 */
void render_reinicializar_animacoes_inimigos(Jogo* jogo);

/**
 * @brief Desenha as entidades móveis da fase (jogador, inimigos, donkey, princesa)
 * @param[in] jogo Estado principal consultado para renderização
 */
void desenha_entidades(const Jogo* jogo);

/**
 * @brief Desenha o mapa estático da fase (chão, escadas, porta)
 * @param[in] jogo Estado principal consultado para renderização
 */
void desenha_mapa(const Jogo* jogo);

/**
 * @brief Desenha o tempo, vidas e fase atual na HUD
 * @param[in,out] jogo Estado principal consultado para renderização
 */
void desenha_elementos_HUD(Jogo* jogo);

/**
 * @brief Desenha um frame completo da tela atual a partir do estado do jogo
 * @param[in,out] jogo Estado principal consultado para renderização
 */
void render_desenhar(Jogo* jogo);

/**
 * @brief Desenha todos os power-ups ativos da fase
 * @param[in] jogo Estado principal consultado para a lista de power-ups
 */
void desenha_powerups(const Jogo* jogo);

/**
 * @brief Desenha o layout do menu de pausa (continuar, voltar para o menu ou sair do jogo)
 * @param[in] opcao_selecionada Opção atualmente destacada no menu
 */
void desenha_menu_pausa(OpcaoPausa opcao_selecionada);

/**
 * @brief Desenha tela de game over
 * @param[in] jogo Estado principal consultado
 * @param[in] fonte Fonte usada para desenhar o texto
 */
void desenha_game_over(const Jogo* jogo, Font fonte);

/**
 * @brief Desenha tela de vitória
 * @param[in] jogo Estado principal consultado
 * @param[in] fonte Fonte usada para desenhar o texto
 */
void desenha_vitoria(const Jogo* jogo, Font fonte);

/**
 * @brief Encerra a renderização das texturas
 */
void render_encerrar(void);
