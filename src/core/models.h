#pragma once
#include <stdbool.h>
#include "raylib.h"
#include "constants.h"

/**
 * @brief Vetor bidimensional usado para posições, velocidades e acelerações
 */
typedef struct Vetor2D
{
    float x; /**< Componente horizontal do vetor */
    float y; /**< Componente vertical do vetor */
} Vetor2D;

typedef struct Animacao {
    int first;
    int last;
    int cur; // current
    float speed;
    float duration_left;
    AnimationType type;
    int step; // how far you are stepping to the next frame
} Animacao;

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
    bool acao_pulo; /**< Estado da ação principal no frame atual */
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
    Animacao animacao;
    ComandosJogador comandos;
    Vetor2D aceleracao; /**< Aceleração atual do jogador em pixels por segundo ao quadrado */
    DirecaoHorizontal direcao_horizontal; /**< Direção horizontal atual do jogador */
    int vidas; /**< Quantidade de vidas restantes */
    bool ativo; /**< Indica se o jogador está ativo na simulação */
    bool esta_no_chao; /**< Indica se o jogador está apoiado em plataforma */
    bool esta_em_escada; /**< Indica se o jogador está interagindo com escada */
    bool esta_pulando; /**< Indica se o jogador está em estado de pulo */
    bool alcancou_porta; /**< Indica se o jogador alcançou a porta de saída */
    bool invencivel; /**< Indica se o jogador está invencível temporariamente */
    float tempo_invencibilidade; /**< Segundos restantes de invencibilidade */
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
    Animacao animacao;
    Vetor2D aceleracao; /**< Aceleração atual do inimigo em pixels por segundo ao quadrado */
    DirecaoHorizontal direcao_horizontal; /**< Direção horizontal atual do inimigo */
    TipoInimigo tipo; /**< Comportamento de patrulha do inimigo */
    bool ativo; /**< Indica se o inimigo participa da simulação */
} Inimigo;

/**
 * @brief Estado completo do donkey na fase
 */
typedef struct Donkey
{
    PosicaoMapa tile;
    PosicaoMapa spawn_inicial;
    Vetor2D posicao_pixels;
    Vetor2D velocidade;
    Animacao animacao;
    Vetor2D aceleracao;
    DirecaoHorizontal direcao_horizontal;
    bool ativo;
} Donkey;

/**
 * @brief Estado completo da princesa na fase
 */

 typedef struct Princesa
 {
    PosicaoMapa tile;
    PosicaoMapa spawn_inicial;
    Vetor2D posicao_pixels;
    Animacao animacao;
    DirecaoHorizontal direcao_horizontal;
    bool ativo; // fim, pois a princesa não se moverá
 } Princesa;
/**
 * @brief Power-up coletável espalhado na fase
 */
typedef struct PowerUp
{
    Vetor2D posicao_pixels; /**< Posição em pixels do power-up na tela */
    TipoPowerUp tipo; /**< Efeito aplicado ao ser coletado */
    bool ativo; /**< Indica se o power-up ainda pode ser coletado */
} PowerUp;

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
 * @brief Controle de tempos das diferentes telas do jogo
 */
typedef struct TemposTelas
{
    float segundos_ate_jogar; /**< Tempo acumulado até o jogador começar */
    float segundos_ate_fim_partida; /**< Tempo acumulado até o fim da partida */
    float segundos_ate_pausar; /**< Momento em que o jogo foi pausado, usado para não contar o tempo parado */
} TemposTelas;

/**
 * @brief Estado principal da aplicação e da partida atual
 */
typedef struct Jogo
{
    TelaAtual tela_atual; /**< Tela atualmente exibida pela aplicação */
    bool deve_encerrar; /**< Sinaliza que o loop principal deve ser encerrado */
    bool ranking_atualizado; /**< Indica se o ranking já foi atualizado no fim da partida */
    bool enter_processado_neste_frame; /**< Indica se a tecla ENTER foi processada neste frame */
    int fase_atual; /**< Número da fase atualmente carregada */
    int tempo_partida_segundos; /**< Tempo acumulado da partida em segundos */
    char nome_jogador[MAX_NOME_JOGADOR]; /**< Nome digitado para uso no ranking */
    Jogador jogador; /**< Estado atual do jogador */
    Donkey donkey;/**<Estado atual do donkey */
    Princesa princesa; /** <Estado atual da princesa */
    Inimigo inimigos[MAX_INIMIGOS]; /**< Vetor fixo com os inimigos ativos da fase */
    int quantidade_inimigos; /**< Quantidade válida de inimigos no vetor */
    Mapa mapa; /**< Dados da fase atualmente carregada */
    TipoPlacar placar[MAX_PLACAR]; /**< Ranking carregado ou montado em memória */
    TemposTelas tempos_telas; /**< Controle de tempos das diferentes telas */
    PowerUp powerups[MAX_POWERUPS]; /**< Power-ups ativos na fase atual */
    int quantidade_powerups; /**< Quantidade válida de power-ups no vetor */
    int bonus_tempo_segundos; /**< Segundos acumulados de redução de tempo por power-ups */
    int opcao_pausa; /**< Opção selecionada no menu de pausa */
} Jogo;

/**
 * @brief Define a estrutura das imagens utilizadas
 */
typedef struct {
    Texture2D mario;
    Texture2D fantasma;
    Texture2D princesa;
    Texture2D donkey;
    Texture2D mapa;
    Texture2D chao;
    Texture2D escada;
    Texture2D porta;
    Texture2D estrela;
    Texture2D coracao;
    Texture2D relogio;
    
} TexturasJogo;