#include <string.h>
#include "../core/models.h"
#include "render.h"
#include "raylib.h"
#include "menu.h"
#include "animacao.h"
#include "../core/constants.h" 
#include "audio.h"
#define LARGURA_IMAGENS_GRANDES 900
#define ALTURA_IMAGENS_GRANDES 300

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
static Font fonte_jogo;
static Font fonte_demais_textos;

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

// Inicializa as texturas, fontes e as structs das animações
void render_inicializar(Jogo* jogo) {
    //Inicializando as texturas/imagens
    imagens.mario = LoadTexture("assets/imagens/mario.png");
    imagens.princesa = LoadTexture("assets/imagens/princesa.png");
    imagens.donkey = LoadTexture("assets/imagens/donkey.png");
    imagens.fantasma = LoadTexture("assets/imagens/fantasma.png");

    imagens.escada = redimensiona_imagem_estatica("escada");
    imagens.chao = redimensiona_imagem_estatica("chao");
    imagens.porta = redimensiona_imagem_estatica("porta");

    //Inicializa a fonte do jogo e a fonte dos demais textos

    fonte_jogo = LoadFontEx("assets/fontes/SuperMario256.ttf", TAMANHO_FONTE_PRINCIPAL, NULL, 0);
    fonte_demais_textos = LoadFontEx("assets/fontes/DemaisTextos.ttf", TAMANHO_FONTE_PRINCIPAL, NULL, 0);
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
    for(int num_inimigos = 0; num_inimigos < jogo->quantidade_inimigos; num_inimigos++){
        jogo->inimigos[num_inimigos].animacao = (Animacao){
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

// Desenha o mapa (estruturas estáticas)
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

// Desenha o tempo ocorrido desde que o jogo foi para a tela da gameplay
void desenha_tempo(Jogo* jogo){
    if (jogo -> tela_atual == TELA_JOGANDO){
        // --------------------- Desenha o texto "TEMPO" -----------------
        float tamanho_fonte_texto_tempo = TAMANHO_FONTE_TEMPO;

        Vector2 tamanho_texto_tempo = MeasureTextEx(fonte_demais_textos, "TEMPO", tamanho_fonte_texto_tempo, 2.0f); 
        float pos_texto_tempo_x = ((float) JANELA_LARGURA * 0.9f) - (tamanho_texto_tempo.x / 2.0f);
        float pos_texto_tempo_y = ((float) JANELA_ALTURA * 0.05f) - (tamanho_texto_tempo.y / 2.0f);

        Vector2 posicao_texto_tempo = {
            pos_texto_tempo_x, pos_texto_tempo_y
        };
        // Desenha o texto com traçado "tempo"
        DrawTextWithOutline(fonte_demais_textos, "TEMPO", posicao_texto_tempo, tamanho_fonte_texto_tempo, 2.0f, YELLOW, GRAY, 1.0f);
        // ---------------------- Desenha texto dos SEGUNDOS ----------------------------------------------
        float tamanho_fonte_texto_segundos = tamanho_fonte_texto_tempo;

        const char* texto_segundos = TextFormat("%d", (int)GetTime() - jogo->tempos_telas.segundos_ate_jogar);
        Vector2 tamanho_texto_segundos = MeasureTextEx(fonte_demais_textos, texto_segundos, tamanho_fonte_texto_segundos, 2.0f);
        float pos_texto_segundos_x = pos_texto_tempo_x + (tamanho_texto_tempo.x / 2.0f) - (tamanho_texto_segundos.x / 2.0f);
        float pos_texto_segundos_y = pos_texto_tempo_y + (tamanho_texto_tempo.y * 1.5f);

        Vector2 posicao_texto_segundos = {
            pos_texto_segundos_x, pos_texto_segundos_y
        };

        DrawTextWithOutline(fonte_demais_textos, texto_segundos, posicao_texto_segundos, tamanho_fonte_texto_segundos, 2.0f, YELLOW, GRAY, 1.0f);
    }
}

// Desenha os objetos que se movem (as entidades)
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

    for(int num_inimigos = 0; num_inimigos < jogo->quantidade_inimigos; num_inimigos++){
        if(jogo->inimigos[num_inimigos].ativo == true){
            DrawTexturePro(
                imagens.fantasma,
                animation_frame(&jogo->inimigos[num_inimigos].animacao, 6),
                (Rectangle){jogo->inimigos[num_inimigos].posicao_pixels.x, jogo->inimigos[num_inimigos].posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE},
                (Vector2){0.0f, 0.0f},
                0.0f,
                WHITE
            );
        }
}
}

// Função que dita qual tela será desenhada no momento
void render_desenhar(Jogo* jogo){
    BeginDrawing();
    switch (jogo -> tela_atual)
        {
        case TELA_JOGANDO:
            ClearBackground(BLACK);
            desenha_mapa(jogo);
            desenha_tempo(jogo);
            desenha_entidades(jogo);
            break;
        case TELA_MENU_PRINCIPAL:
            ClearBackground(LIGHTGRAY);
            desenha_menu_principal(jogo, fonte_jogo, fonte_demais_textos, imagens.mario, imagens.princesa, imagens.donkey);
            break;
        case TELA_RANKING:
            ClearBackground(LIGHTGRAY);
            desenha_tela_ranking(jogo, fonte_jogo, fonte_demais_textos);
            break;
        case TELA_DIGITANDO_NOME:
            ClearBackground(BLACK);
            desenha_menu_nome(jogo, fonte_demais_textos);
            break;
        default:
            break;
        }
    EndDrawing();
}
// Descarrega as texturas e as fontes
void render_encerrar(void) {

    UnloadTexture(imagens.mario);
    UnloadTexture(imagens.fantasma);
    UnloadTexture(imagens.princesa);
    UnloadTexture(imagens.donkey);
    UnloadTexture(imagens.chao);
    UnloadTexture(imagens.escada);
    UnloadTexture(imagens.porta);

    UnloadFont(fonte_jogo);
    UnloadFont(fonte_demais_textos);
}