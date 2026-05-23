#include <string.h>
#include "../core/models.h"
#include "render.h"
#include "raylib.h"
#include "animacao.h"
#include "../core/constants.h" 
#define LARGURA_IMAGENS_GRANDES 900
#define ALTURA_IMAGENS_GRANDES 300


typedef struct {
    Texture2D mario;
    Texture2D fantasma;
    Texture2D princesa;
    Texture2D donkey;
    Texture2D mapa;
    Texture2D chao;
    Texture2D escada;
    Texture2D porta;
    
} TexturasJogo;

/*
typedef enum AnimationType{
    STOPPED = 0,
    WALKING_RIGHT = 1,
    WALKING_LEFT = 2,
    JUMPING = 3,
} AnimationType;
 


typedef struct Animation {
    int first;
    int last;
    int cur; // current
    float speed;
    float duration_left;
    AnimationType type;
    int step; // how far you are stepping to the next frame
} Animation;
 */

static TexturasJogo imagens;

Texture2D redimensiona_imagem_estatica(const char* nome_arquivo){
    //altera o nome do caminho para o nome do arquivo informado
    const char* caminho = TextFormat("assets/imagens/%s.png", nome_arquivo);
    // Carrega a imagem do caminho informado
    Image img = LoadImage(caminho);
    // Altera o tamanhoda mesma para ser o tamanho da Tile
    if(!strcmp(nome_arquivo, "porta")){
        ImageResize(&img, TILE_SIZE * 2, TILE_SIZE * 2); // Duplicar o tamanho da imagem caso for uma porta
    }
    else{
        ImageResize(&img, TILE_SIZE, TILE_SIZE);
    }
    // Carrega ela na variavel tex
    Texture2D tex = LoadTextureFromImage(img);
    // Descarrega a imagem da mem
    UnloadImage(img);
    // Retorna a variavel tex
    return tex;    
}

void render_inicializar(Jogo* jogo) {
    //Inicializando as texturas/imagens
    imagens.mario = LoadTexture("assets/imagens/mario.png");
    imagens.princesa = LoadTexture("assets/imagens/princesa.png");
    imagens.donkey = LoadTexture("assets/imagens/donkey.png");
    imagens.fantasma = LoadTexture("assets/imagens/fantasma.png");

    imagens.escada = redimensiona_imagem_estatica("escada");
    imagens.chao = redimensiona_imagem_estatica("chao");
    imagens.porta = redimensiona_imagem_estatica("porta");
    
    //Inicializando as structs das animações
    //Animacao mario:
    jogo->jogador.animacao = (Animacao){
    .first = 0,
    .last = 2,
    .cur = 0,
    .speed = 0.1,
    .duration_left = 0.1,
    .type = STOPPED,
    .step = 1,
    };

    //Animacao donkey:
    jogo ->donkey.animacao = (Animacao){
    .first = 0,
    .last = 2,
    .cur = 0,
    .speed = 0.1,
    .duration_left = 0.1,
    .type = STOPPED,
    .step = 1,
    };

    //Animacao princesa:
    jogo -> princesa.animacao = (Animacao){
    .first = 0,
    .last = 2,
    .cur = 0,
    .speed = 0.1,
    .duration_left = 0.1,
    .type = STOPPED,
    .step = 1,
    };
    //Animacao fantasma:
    for(int numInimigos = 0; numInimigos < jogo->quantidade_inimigos; numInimigos++){
        jogo->inimigos[numInimigos].animacao = (Animacao){
        .first = 0,
        .last = 2,
        .cur = 0,
        .speed = 0.1f,
        .duration_left = 0.1f,
        .type = STOPPED,
        .step = 1,
        };
    }
}

void desenha_mapa  (const Jogo* jogo){
    for(int linha = 0; linha < MAPA_LINHAS; linha++){
        for(int coluna = 0; coluna < MAPA_COLUNAS; coluna++){
            Vector2 coordenadas = {(float)coluna * TILE_SIZE, (float)linha * TILE_SIZE};

            switch (jogo->mapa.tiles[linha][coluna])
            {
            case TILE_CHAO: // Bloco na frente das escadas para a escada não ficar na frente do primeiro bloco depois da escada
                DrawTextureV(imagens.chao, coordenadas, WHITE);
                break;
            case TILE_ESCADA_SUBIDA:
            case TILE_ESCADA_PADRAO:
                DrawTextureV(imagens.escada, coordenadas, WHITE);
                break;
            case TILE_PORTA:
                DrawTextureV(imagens.porta, (Vector2) { coordenadas.x, coordenadas.y - TILE_SIZE}, WHITE); // Subtrair o Tile_size do eixo y, já que duplicamos pra todos os lados
                break;
            default:
                //Caso vazio e caso escada descida
                // O caso da escada descida está aí para a escada não atravessar o bloco
                break;
            }
        }
    }
}

// Desenha os objetivos que se movem (as entidades)
void desenha_entidades(const Jogo *jogo)
{

    if(jogo->jogador.ativo == true){
        DrawTexturePro(
            imagens.mario,
            animation_frame(&jogo->jogador.animacao, FRAMES_POR_LINHA),                                            // Define aonde será o corte na imagem original
            (Rectangle){jogo->jogador.posicao_pixels.x, jogo->jogador.posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE}, // Define aonde ficará o mario no jogo
            (Vector2){0.0f, 0.0f},                                                                                           // Define o eixo principal de rotação
            0.0f,                                                                                                            // define a quantia de rotação
            WHITE
        );                                                                                                          // Define a cor
    }

    if (jogo->princesa.ativo == true)
    {
        DrawTexturePro(
            imagens.princesa,
            animation_frame(&jogo->princesa.animacao, FRAMES_POR_LINHA),
            (Rectangle){jogo->princesa.posicao_pixels.x, jogo->princesa.posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE},
            (Vector2){0.0f, 0.0f},
            0.0f,
            WHITE
        );
    }

    if (jogo->donkey.ativo == true){
    DrawTexturePro(
        imagens.donkey, 
        animation_frame(&jogo->donkey.animacao, FRAMES_POR_LINHA),
        (Rectangle){jogo->donkey.posicao_pixels.x, jogo->donkey.posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE},
        (Vector2){0.0f, 0.0f},
        0.0f,
        WHITE
    );
    }                                                         

    for(int numInimigos = 0; numInimigos < jogo->quantidade_inimigos; numInimigos++){
        if(jogo->inimigos[numInimigos].ativo == true)
            DrawTexturePro(
                imagens.fantasma,
                animation_frame(&jogo->inimigos[numInimigos].animacao, 6),
                (Rectangle){jogo -> inimigos[numInimigos].posicao_pixels.x, jogo -> inimigos[numInimigos].posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE},
                (Vector2){0.0f, 0.0f},
                0.0f,
                WHITE
            );
        }
}

void render_desenhar(const Jogo* jogo){
    desenha_mapa(jogo);
    desenha_entidades(jogo);
}
            
void render_encerrar(void) {
    UnloadTexture(imagens.mario);
    UnloadTexture(imagens.fantasma);
    UnloadTexture(imagens.princesa);
    UnloadTexture(imagens.donkey);
    UnloadTexture(imagens.chao);
    UnloadTexture(imagens.escada);
    UnloadTexture(imagens.porta);
}