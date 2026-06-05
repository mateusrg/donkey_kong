#pragma once
#include "../core/models.h"
#include "render.h"
#include "audio.h"
#include "raylib.h"
#include "animacao.h"
/**
 * @brief Processa input e transições da tela de menu principal
 * @param[in] font Define a fonte que o texto traçado será escrito
 * @param[in] text A string do texto em questão
 * @param[in] position Posição que o texto ficará na tela
 * @param[in] fontSize Tamanho desejado do texto
 * @param[in] spacing Espaçamento entre as letras
 * @param[in] textColor Cor do texto
 * @param[in] outlineColor Cor do traçado do texto
 * @param[in] outlineThickness Grossura do traçado
 */
void DrawTextWithOutline(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color textColor, Color outlineColor, float outlineThickness);

/**
 * @brief Cria um vetor de posições que clica a tecla UP ou DOWN do teclado podem gerar 
 * @param[in, out] posicoesTeclado Informa qual o indice do vetor de posições possíveis de um ponto qualquer com pase nos inputs do teclado
 */

 void determina_posicoes_inputs(Vector2 *posicoesTeclado, int *indice);

/**
 * @brief Define qual som será tocado no momento no menu
 * @param[in] Retangulo Retangulo que deseja-se descobrir se deve ou nao tocar o som
 * @param[in] posicaoTeclado Onde está localizado o vetor de posições do teclado na tela
 * @param[in] numBotao para saber qual deles que está sendo pressionado
 * @return True se tocar algum som (Ou seja, tecla foi selecionada)
 * @return False caso contrário
 */
 bool tocar_sons_botao(Rectangle retângulo, Vector2 posicaoTeclado, Vector2 posicaoMouse, int numBotao);

/**
 * @brief Processa input e transições do menu de pausa durante a partida
 * @param[in,out] jogo Estado principal usado para realizar o desenho do menu principal
 * @param[in, out] indice Indice atual dentro do vetor das posições possíveis
 */
void desenha_menu_principal(Jogo *jogo, Font fonteJogo, Font fonteBotoes, Texture2D mario, Texture2D princesa, Texture2D donkey);

void desenha_menu_nome(Jogo *Jogo, Font fonteTextos);

void desenha_tela_ranking(Jogo *jogo, Font fonteJogo, Font fonteTextos);


