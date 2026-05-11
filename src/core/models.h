#pragma once

#include <stdbool.h>

#include "constants.h"

/**
 * @brief Vetor bidimensional usado para posições, velocidades e acelerações
 */
typedef struct Vetor2D
{
    float x; /**< Componente horizontal do vetor */
    float y; /**< Componente vertical do vetor */
} Vetor2D;

/**
 * @brief Posição discreta do mapa baseada em linha e coluna
 */
typedef struct PosicaoMapa
{
    int linha; /**< Índice da linha no grid do mapa */
    int coluna; /**< Índice da coluna no grid do mapa */
} PosicaoMapa;

/**
 * @brief Entrada de comandos do jogador lida em um frame
 */
typedef struct ComandosJogador
{
    int horizontal; /**< Direção horizontal desejada pelo jogador */
    int vertical; /**< Direção vertical desejada pelo jogador */
    bool acao_principal; /**< Estado da ação principal no frame atual */
} ComandosJogador;

/**
 * @brief Estado completo do jogador durante a partida
 */
typedef struct Jogador
{
    PosicaoMapa tile; /**< Tile atual ocupado pelo jogador */
    PosicaoMapa spawn_inicial; /**< Tile usado para spawn e respawn */
    Vetor2D posicao_pixels; /**< Posição contínua do jogador em pixels */
    Vetor2D velocidade; /**< Velocidade atual do jogador em pixels por segundo */
    Vetor2D aceleracao; /**< Aceleração atual do jogador em pixels por segundo ao quadrado */
    DirecaoHorizontal direcao_horizontal; /**< Direção horizontal atual do jogador */
    int vidas; /**< Quantidade de vidas restantes */
    bool ativo; /**< Indica se o jogador está ativo na simulação */
    bool esta_no_chao; /**< Indica se o jogador está apoiado em plataforma */
    bool esta_em_escada; /**< Indica se o jogador está interagindo com escada */
    bool esta_pulando; /**< Indica se o jogador está em estado de pulo */
} Jogador;

/**
 * @brief Estado completo de um inimigo da fase
 */
typedef struct Inimigo
{
    PosicaoMapa tile; /**< Tile atual ocupado pelo inimigo */
    PosicaoMapa spawn_inicial; /**< Tile usado para o spawn inicial do inimigo */
    Vetor2D posicao_pixels; /**< Posição contínua do inimigo em pixels */
    Vetor2D velocidade; /**< Velocidade atual do inimigo em pixels por segundo */
    Vetor2D aceleracao; /**< Aceleração atual do inimigo em pixels por segundo ao quadrado */
    DirecaoHorizontal direcao_horizontal; /**< Direção horizontal atual do inimigo */
    bool ativo; /**< Indica se o inimigo participa da simulação */
} Inimigo;

/**
 * @brief Dados do layout da fase carregada do arquivo de mapa
 */
typedef struct Mapa
{
    char tiles[MAPA_LINHAS][MAPA_COLUNAS]; /**< Grade de tiles da fase */
    PosicaoMapa spawn_jogador; /**< Posição inicial do jogador na fase */
    PosicaoMapa porta_saida; /**< Posição da porta de saída da fase */
    bool tem_spawn_jogador; /**< Indica se o arquivo de mapa definiu spawn do jogador */
    bool tem_porta_saida; /**< Indica se o arquivo de mapa definiu porta de saída */
} Mapa;

/**
 * @brief Entrada individual do ranking persistido em disco
 */
typedef struct TipoPlacar
{
    char nome[MAX_NOME_JOGADOR]; /**< Nome do jogador associado ao resultado */
    int time; /**< Tempo registrado para essa entrada do ranking */
} TipoPlacar;

/**
 * @brief Estado principal da aplicação e da partida atual
 */
typedef struct Jogo
{
    TelaAtual tela_atual; /**< Tela atualmente exibida pela aplicação */
    bool deve_encerrar; /**< Sinaliza que o loop principal deve ser encerrado */
    bool ranking_atualizado; /**< Indica se o ranking já foi atualizado no fim da partida */
    int fase_atual; /**< Número da fase atualmente carregada */
    float tempo_partida_segundos; /**< Tempo acumulado da partida em segundos */
    char nome_jogador[MAX_NOME_JOGADOR]; /**< Nome digitado para uso no ranking */
    Jogador jogador; /**< Estado atual do jogador */
    Inimigo inimigos[MAX_INIMIGOS]; /**< Vetor fixo com os inimigos ativos da fase */
    int quantidade_inimigos; /**< Quantidade válida de inimigos no vetor */
    Mapa mapa; /**< Dados da fase atualmente carregada */
    TipoPlacar placar[MAX_PLACAR]; /**< Ranking carregado ou montado em memória */
} Jogo;
