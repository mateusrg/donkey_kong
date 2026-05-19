#include "render.h"
#include "raylib.h"

typedef struct {
    Texture2D mario;
    Texture2D fantasma;
    Texture2D princesa;
    Texture2D donkey;
    Texture2D mapa;
} TexturasJogo;

static TexturasJogo imagens;

void render_inicializar(void){
    imagens.mario = LoadTexture("assets/imagens/mario.png");
    imagens.princesa = LoadTexture("assets/imagens/princesa.png");
    imagens.donkey = LoadTexture("assets/imagens/donkey.png");
    imagens.fantasma = LoadTexture("assets/images/donkey.png";)
}   
void render_desenhar(const Jogo* jogo)
{
    (void)jogo;
}