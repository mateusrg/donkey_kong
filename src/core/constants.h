#pragma once

/** Quantidade de linhas disponíveis no mapa */
#define MAPA_LINHAS 30

/** Quantidade de colunas disponíveis no mapa */
#define MAPA_COLUNAS 30

/** Tamanho de cada tile em pixels */
#define TILE_SIZE 30

/** Altura reservada para a HUD em pixels */
#define HUD_ALTURA 80

/** Largura total da janela em pixels */
#define JANELA_LARGURA (MAPA_COLUNAS * TILE_SIZE)

/** Altura total da janela em pixels incluindo a HUD */
#define JANELA_ALTURA (MAPA_LINHAS * TILE_SIZE + HUD_ALTURA)

// ===== TAMANHOS DE FONTE (RESPONSIVOS AO TILE_SIZE) =====

/** Tamanho da fonte principal ao carregar (em pontos) */
#define TAMANHO_FONTE_PRINCIPAL ((TILE_SIZE * 10 + 6))

/** Tamanho da fonte para exibição de tempo durante gameplay */
#define TAMANHO_FONTE_TEMPO ((float)TILE_SIZE)

/** Tamanho da fonte do título no menu principal */
#define TAMANHO_FONTE_TITULO_MENU ((TILE_SIZE * 4 + 4) / 3.0f)

/** Tamanho da fonte dos botões do menu principal */
#define TAMANHO_FONTE_BOTAO_MENU ((TILE_SIZE * 4) / 5.0f)

/** Tamanho da fonte do título da tela de ranking */
#define TAMANHO_FONTE_TITULO_RANKING ((TILE_SIZE * 7) / 3.0f)

/** Tamanho da fonte dos nomes na tabela de ranking */
#define TAMANHO_FONTE_NOMES_RANKING ((TILE_SIZE * 2) / 3.0f)

/** Tamanho da fonte dos botões de UI (como botão X de voltar) */
#define TAMANHO_FONTE_BOTAO_UI ((float)TILE_SIZE)

/** Tamanho da fonte ao digitar o nome do jogador */
#define TAMANHO_FONTE_DIGITACAO ((float)TILE_SIZE)

/** Quantidade maxima de power-ups simultaneos na fase */
#define MAX_POWERUPS 16

/** Duracao em segundos da invencibilidade concedida pelo power-up */
#define DURACAO_INVENCIBILIDADE 8.0f

/** Reducao em segundos aplicada ao tempo ao coletar um power-up */
#define REDUCAO_TEMPO_POWERUP 5

/** Taxa de quadros alvo da aplicação */
#define FPS_PADRAO 60

/** Quantidade máxima de inimigos simultâneos na fase */
#define MAX_INIMIGOS 32

/** Quantidade máxima de entradas persistidas no ranking */
#define MAX_PLACAR 10
/** Quantidade que aparece voz ver na tela do ranking */
#define MAX_APARECE_RANKING 5

/** Tamanho máximo do nome do jogador incluindo o terminador */
#define MAX_NOME_JOGADOR 20

/** Tamanho máximo de buffer usado para caminhos de arquivo */
#define MAX_TEXTO_CAMINHO 260

/** Quantidade inicial de vidas do jogador */
#define VIDAS_INICIAIS 3

/** Velocidade horizontal padrão do jogador em pixels por segundo */
#define VELOCIDADE_JOGADOR_PADRAO 180.0f

/** Velocidade vertical padrão ao subir ou descer escadas em pixels por segundo */
#define VELOCIDADE_ESCADA_PADRAO 150.0f

/** Velocidade horizontal padrão do inimigo em pixels por segundo */
#define VELOCIDADE_INIMIGO_PADRAO 90.0f

/** Aceleração horizontal padrão do jogador em pixels por segundo ao quadrado */
#define ACELERACAO_HORIZONTAL_PADRAO 1200.0f

/** Gravidade padrão aplicada ao jogador em pixels por segundo ao quadrado */
#define GRAVIDADE_PADRAO 1800.0f

/** Impulso vertical inicial do pulo em pixels por segundo */
/** altura maxima = v0^2 / (2 * GRAVIDADE) => 480^2 / 3600 = 64px ~ 2 tiles */
#define FORCA_PULO_PADRAO 480.0f

/** Intervalo base entre passos de animação do inimigo em segundos */
#define INIMIGO_INTERVALO_PASSO 0.30f

/** Diretório onde ficam os arquivos de mapa */
#define DIRETORIO_MAPAS "dados/mapas/"

/** Caminho do arquivo binário de placar */
#define CAMINHO_ARQUIVO_PLACAR "dados/placar/placar.bin"

/** Símbolo de tile vazio no arquivo de mapa */
#define TILE_VAZIO ' '

/** Símbolo de spawn do jogador no arquivo de mapa */
#define TILE_JOGADOR 'P'

/** Símbolo de spawn de inimigo no arquivo de mapa */
#define TILE_INIMIGO 'E'

/** Símbolo de plataforma sólida no arquivo de mapa */
#define TILE_CHAO 'Z'

/** Símbolo de escada que permite subir no arquivo de mapa */
#define TILE_ESCADA_SUBIDA 'S'

/** Símbolo de escada que permite descer no arquivo de mapa */
#define TILE_ESCADA_DESCIDA 'D'

/** Símbolo visual auxiliar de escada no arquivo de mapa */
#define TILE_ESCADA_PADRAO 'H'

/** Símbolo da porta de saída no arquivo de mapa */
#define TILE_PORTA 'F'
/** Simbolo da princesa no arquivo de mapa */
#define TILE_PRINCESA 'R'
/** Simbolo do macaco no arquivo de mapa */
#define TILE_DONKEY 'M'
/** Simbolo do powerup que reduz o tempo do jogador no arquivo de mapa */
#define TILE_POWERUP_TEMPO 'B'
/** Simbolo do powerup que concede vida extra no arquivo de mapa */
#define TILE_POWERUP_VIDA 'I'
/** Simbolo do powerup que concede invencibilidade temporaria no arquivo de mapa */
#define TILE_POWERUP_INVENCIVEL 'V'
/** Simbolo do inimigo veloz no arquivo de mapa */
#define TILE_INIMIGO_VELOZ 'C'
/** Tamanho maximo do array de nomes */
#define TAM_MAX_NOME 10
/** Quantidade de frames por cada linha das imagens */
#define FRAMES_POR_LINHA 6
/** Define o limite de segundos do jogo */
#define LIMITE_SEGUNDOS 999
/**
 * @brief Identifica a tela atual da aplicação
 */
typedef enum TelaAtual
{
    TELA_MENU_PRINCIPAL = 0, /**< Tela inicial com as opções principais */
    TELA_JOGANDO = 1, /**< Tela de jogo com a fase em andamento */
    TELA_PAUSADO = 2, /**< Tela de pausa aberta durante a partida */
    TELA_RANKING = 3, /**< Tela de exibição do ranking salvo */
    TELA_DIGITANDO_NOME = 4, /**< Tela de captura do nome para o ranking */
    TELA_VITORIA = 5, /**< Tela mostrada após concluir a partida */
    TELA_GAME_OVER = 6, /**< Tela mostrada quando o jogador perde a partida */
    TELA_SAIR = 7
} TelaAtual;

/**
 * @brief Representa a direção horizontal de movimento de uma entidade
 */
typedef enum DirecaoHorizontal
{
    DIRECAO_ESQUERDA = -1, /**< Movimento horizontal para a esquerda */
    DIRECAO_PARADO = 0, /**< Entidade sem deslocamento horizontal */
    DIRECAO_DIREITA = 1 /**< Movimento horizontal para a direita */
} DirecaoHorizontal;

/**
 * @brief Representa o estado atual de determinado personagem
 */
typedef enum AnimationType{
    STOPPED = 0, /**< Animação da entidade parada */
    WALKING_RIGHT = 1, /** Animação da entidade andando para a direita */
    WALKING_LEFT = 2, /** Animação da entidade andando para a esquerda */
    JUMPING = 3, /** Animação da entidade pulando */
} AnimationType;

/**
 * @brief Tipo de comportamento do inimigo
 */
typedef enum TipoInimigo
{
    INIMIGO_PATRULHEIRO = 0, /**< Patrulha horizontal em velocidade normal */
    INIMIGO_VELOZ = 1,       /**< Patrulha horizontal em velocidade dobrada */
} TipoInimigo;

/**
 * @brief Tipo de efeito do power-up coletado
 */
typedef enum TipoPowerUp
{
    POWERUP_TEMPO = 0,      /**< Reduz o tempo contado em REDUCAO_TEMPO_POWERUP segundos */
    POWERUP_VIDA = 1,       /**< Concede uma vida extra ao jogador */
    POWERUP_INVENCIVEL = 2, /**< Torna o jogador invencivel por DURACAO_INVENCIBILIDADE segundos */
} TipoPowerUp;
