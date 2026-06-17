#pragma once
#include "../core/models.h"
#include "render.h"
#include "audio.h"
#include "raylib.h"
#include "animacao.h"

/**
 * @brief Botões disponíveis no menu principal
 */
typedef enum BotaoMenuPrincipal
{
    BOTAO_MENU_NENHUM = 0, /**< Nenhum botão selecionado */
    BOTAO_MENU_JOGAR = 1, /**< Botão "Jogar" */
    BOTAO_MENU_PLACAR = 2, /**< Botão "Placar" */
    BOTAO_MENU_SAIR = 3 /**< Botão "Sair" */
} BotaoMenuPrincipal;

/**
 * @brief Campo do ranking a ser extraído para exibição
 */
typedef enum CampoRanking
{
    CAMPO_RANKING_NOME = 'n', /**< Nome do jogador */
    CAMPO_RANKING_TEMPO = 't' /**< Tempo registrado */
} CampoRanking;

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
 * @param indice Botão atualmente selecionado pelo cursor
 */
void determina_posicoes_inputs(BotaoMenuPrincipal *indice);

/**
 * @brief Lê o próximo caractere pressionado pelo jogador para digitação do nome
 */
void atualiza_input_nome(void);

/**
 * @brief Toca o som de digitação ao pressionar backspace ou qualquer caractere
 */
void toca_audio_digitando(void);

/**
 * @brief Verifica interação com botão (mouse ou teclado) e toca sons correspondentes
 * @param retangulo Área do botão
 * @param posicao_teclado Posição do cursor do teclado
 * @param posicao_mouse Posição do mouse
 * @param botao Botão sendo verificado
 * @return true se o botão foi ativado, false caso contrário
 */
bool tocar_sons_botao(Rectangle retangulo, Vector2 posicao_teclado, Vector2 posicao_mouse, BotaoMenuPrincipal botao);

/**
 * @brief Verifica se houve clique do mouse dentro de um retângulo
 * @param[in] retangulo_interagivel Área sensível ao clique
 * @param[in] posicao_mouse Posição atual do cursor do mouse
 * @return true se o mouse estiver dentro do retângulo e o botão esquerdo foi pressionado
 */
bool determina_clique_retangulo(Rectangle retangulo_interagivel, Vector2 posicao_mouse);

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

/**
 * @brief Processa input e transições do menu de pausa
 * @param[in,out] jogo Estado principal que será lido e alterado pelo menu
 */
void atualiza_menu_pausa(Jogo *jogo);

/**
 * @brief Calcula a posição de desenho de um nome ou tempo na tabela de ranking
 * @param[in] placar_atual Entrada do ranking que será exibida
 * @param[in] campo Campo do ranking a ser exibido (nome ou tempo)
 * @param[in] indice Índice da linha na tabela
 * @param[in] fonte_nomes Fonte usada para medir o texto
 * @param[in] retangulo_calculos Retângulo base usado para o cálculo da posição
 * @return Vetor com a posição onde o texto deve ser desenhado
 */
Vector2 determina_posicao_ranking(TipoPlacar placar_atual, CampoRanking campo, int indice, Font fonte_nomes, Rectangle retangulo_calculos);

/**
 * @brief Renderiza os retângulos da tabela de ranking
 * @param[in] retangulo_exterior Retângulo que delimita a área da tabela
 * @param[in] fonte_nomes Fonte usada para os nomes e tempos
 * @param[in] jogo Estado principal com os dados do ranking
 */
void desenha_retangulos_menores_ranking(const Rectangle retangulo_exterior, Font fonte_nomes, Jogo* jogo);

/**
 * @brief Renderiza a tela de ranking com tabela de pontuações
 * @param jogo Ponteiro para a estrutura do jogo
 * @param fonte_jogo Fonte principal do jogo
 * @param fonte_textos Fonte para os textos da tabela
 */
void desenha_tela_ranking(Jogo *jogo, Font fonte_jogo, Font fonte_textos);


