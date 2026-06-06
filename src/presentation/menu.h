#pragma once
#include "../core/models.h"
#include "render.h"
#include "audio.h"
#include "raylib.h"
#include "animacao.h"
/**
 * @brief Desenha texto com contorno (outline) para melhor legibilidade
 * @param font Fonte a ser utilizada
 * @param text Texto a ser desenhado
 * @param position Posição do texto na tela
 * @param fontSize Tamanho da fonte
 * @param spacing Espaçamento entre caracteres
 * @param textColor Cor principal do texto
 * @param outlineColor Cor do contorno
 * @param outlineThickness Espessura do contorno em pixels
 */
void DrawTextWithOutline(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color textColor, Color outlineColor, float outlineThickness);

/**
 * @brief Atualiza a posição do cursor do teclado com base em inputs de setas
 * @param posicoes_teclado Vetor de posições possíveis
 * @param indice Índice atual do cursor
 */
void determina_posicoes_inputs(Vector2 *posicoes_teclado, int *indice);
void atualiza_input_nome(void);
void toca_audio_digitando(void);

/**
 * @brief Verifica interação com botão (mouse ou teclado) e toca sons correspondentes
 * @param retangulo Área do botão
 * @param posicao_teclado Posição do cursor do teclado
 * @param posicao_mouse Posição do mouse
 * @param num_botao Número do botão (1=Jogar, 2=Placar, 3=Sair)
 * @return true se o botão foi ativado, false caso contrário
 */
bool tocar_sons_botao(Rectangle retangulo, Vector2 posicao_teclado, Vector2 posicao_mouse, int num_botao);

/**
 * @brief Processa input e transições da tela de menu principal
 * @param jogo Ponteiro para a estrutura do jogo
 * @param posicoes_teclado Vetor de posições possíveis do cursor
 * @param posicao_mouse Posição atual do mouse
 * @param retangulo_jogar Área do botão "Jogar"
 * @param retangulo_placar Área do botão "Placar"
 * @param retangulo_sair Área do botão "Sair"
 */
void atualiza_menu_principal(Jogo *jogo, Vector2 *posicoes_teclado, Vector2 posicao_mouse, Rectangle retangulo_jogar, Rectangle retangulo_placar, Rectangle retangulo_sair);

/**
 * @brief Renderiza o menu principal e processa inputs
 * @param jogo Ponteiro para a estrutura do jogo
 * @param fonte_jogo Fonte principal do jogo
 * @param fonte_botoes Fonte dos botões
 * @param mario Textura do Mario
 * @param princesa Textura da Princesa
 * @param donkey Textura do Donkey Kong
 */
void desenha_menu_principal(Jogo *jogo, Font fonte_jogo, Font fonte_botoes, Texture2D mario, Texture2D princesa, Texture2D donkey);

/**
 * @brief Renderiza e processa a tela de digitação de nome do jogador
 * @param jogo Ponteiro para a estrutura do jogo
 * @param fonte_textos Fonte para os textos
 */
void desenha_menu_nome(Jogo *jogo, Font fonte_textos);

Vector2 determina_posicao_ranking(TipoPlacar placar_atual, char opcao, int indice, Font fonte_nomes, Rectangle retangulo_calculos);
/**
 * @brief Renderiza os retângulos da tabela de ranking
 * @param retangulo_exterior Retângulo que delimita a área da tabela
 */
void desenha_retangulos_menores_ranking(const Rectangle retangulo_exterior, Font fonte_nomes, Jogo* jogo);

bool atualiza_tela_ranking(Rectangle retangulo_interagivel, Vector2  posicao_mouse);

/**
 * @brief Renderiza a tela de ranking com tabela de pontuações
 * @param jogo Ponteiro para a estrutura do jogo
 * @param fonte_jogo Fonte principal do jogo
 * @param fonte_textos Fonte para os textos da tabela
 */
void desenha_tela_ranking(Jogo *jogo, Font fonte_jogo, Font fonte_textos);


