#include <string.h>
#include "render.h"
#include "raylib.h"
#include "../core/constants.h" 
#define LARGURA_IMAGENS_GRANDES 900
#define  ALTURA_IMAGENS_GRANDES 300

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

void render_inicializar(void) {
    imagens.mario = LoadTexture("assets/imagens/mario.png");
    imagens.princesa = LoadTexture("assets/imagens/princesa.png");
    imagens.donkey = LoadTexture("assets/imagens/donkey.png");
    imagens.fantasma = LoadTexture("assets/imagens/fantasma.png");

    imagens.escada = redimensiona_imagem_estatica("escada");
    imagens.chao = redimensiona_imagem_estatica("chao");
    imagens.porta = redimensiona_imagem_estatica("porta");
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

void desenha_entidades (const Jogo* jogo){

    Rectangle source = { 0.0f, 0.0f, (float)imagens.mario.width/6, (float)imagens.mario.heigh/6 };
    
    // Se o seu jogador guarda a posição em pixels:
    Rectangle dest = { jogo->jogador.posicao_pixels.x, jogo->jogador.posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE };
    
    DrawTexturePro(imagens.mario, source, dest, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

/* for(int linha = 0; linha < MAPA_LINHAS; linha++){
        for(int coluna = 0; coluna < MAPA_COLUNAS; coluna++){
            Vector2 coordenadas = {(float)coluna * TILE_SIZE, (float)linha * TILE_SIZE};

            switch (jogo->mapa.tiles[linha][coluna])
            {
            case TILE_JOGADOR: 
                DrawTexturePro(imagens.mario, (Rectangle) {0.0f , 0.0f, (float)imagens.mario.width, (float)imagens.mario.height}, 
                (Rectangle) {(float) coluna *TILE_SIZE, (float) linha *  TILE_SIZE, (float)TILE_SIZE, (float)   TILE_SIZE}, (Vector2) {0.0f, 0.0f}, 0.0f, WHITE);
                break;
             case TILE_PRINCESA:
                DrawTexturePro();
                break;
            case TILE_DONKEY:
                DrawTexturePro();
                break;
            case TILE_INIMIGO:
                DrawTexturePro();
                break;
            case TILE_PORTA:
                DrawTexturePro();
                break;
                
            }
        }
    }
*/
  }

void render_desenhar(const Jogo* jogo);
            
void render_encerrar(void) {
    UnloadTexture(imagens.mario);
    UnloadTexture(imagens.fantasma);
    UnloadTexture(imagens.princesa);
    UnloadTexture(imagens.donkey);
    UnloadTexture(imagens.chao);
    UnloadTexture(imagens.escada);
    UnloadTexture(imagens.porta);
}