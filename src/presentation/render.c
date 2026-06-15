#include <string.h>
#include "../core/models.h"
#include "render.h"
#include "raylib.h"
#include "menu.h"
#include "animacao.h"
#include "../core/constants.h" 
#include "audio.h"

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

    imagens.relogio = redimensiona_imagem_estatica("relogio");
    imagens.coracao = redimensiona_imagem_estatica("coracao");
    imagens.estrela = redimensiona_imagem_estatica("estrela");
    imagens.escada = redimensiona_imagem_estatica("escada");
    imagens.chao = redimensiona_imagem_estatica("chao");
    imagens.porta = redimensiona_imagem_estatica("porta");

    //Inicializa a fonte do jogo e a fonte dos demais textos

    fonte_jogo = LoadFontEx("assets/fontes/SuperMario256.ttf", TAMANHO_FONTE_PRINCIPAL, NULL, 0);
    fonte_demais_textos = LoadFontEx("assets/fontes/DemaisTextos.ttf", TAMANHO_FONTE_PRINCIPAL, NULL, 0);
    //Inicializando as structs das animações
    //Animacao mario:
    animacao_inicializar(&jogo->jogador.animacao);

    //Animacao donkey:
    animacao_inicializar(&jogo->donkey.animacao);

    //Animacao princesa:
    animacao_inicializar(&jogo->princesa.animacao);

    //Animacao fantasma:
    render_reinicializar_animacoes_inimigos(jogo);
}

// Reinicia as animações dos inimigos atuais — usado após carregar uma fase,
// pois render_inicializar é chamado antes de quantidade_inimigos ser preenchida.
void render_reinicializar_animacoes_inimigos(Jogo* jogo)
{
    int i;

    for (i = 0; i < jogo->quantidade_inimigos; i++)
    {
        animacao_inicializar(&jogo->inimigos[i].animacao);
    }
}

/*
TILE_POWERUP_TEMPO 'B'

TILE_POWERUP_VIDA 'I'

ILE_POWERUP_INVENCIVEL 'V'
*/

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
void desenha_elementos_HUD(Jogo* jogo){
    if (jogo -> tela_atual == TELA_JOGANDO){
        int tempo_exibido;
        int v;
        float pos_vidas_x;
        float pos_vidas_y;
        float gap;
        Vector2 pos_coracao;

        float tamanho_fonte_texto_tempo = TAMANHO_FONTE_TEMPO;

        // Subtrai o bonus acumulado por power-ups; clamp em 0 para não exibir negativo
        tempo_exibido = (int)GetTime() - (int)jogo->tempos_telas.segundos_ate_jogar - jogo->bonus_tempo_segundos;
        if (tempo_exibido < 0) tempo_exibido = 0;

        Vector2 tamanho_texto_tempo = MeasureTextEx(fonte_demais_textos, "TEMPO", tamanho_fonte_texto_tempo, 2.0f); 
        float pos_texto_tempo_x = ((float) JANELA_LARGURA * 0.9f) - (tamanho_texto_tempo.x / 2.0f);
        float pos_texto_tempo_y = ((float) JANELA_ALTURA * 0.05f) - (tamanho_texto_tempo.y / 2.0f);

        Vector2 posicao_texto_tempo = {
            pos_texto_tempo_x, pos_texto_tempo_y
        };
        DrawTextWithOutline(fonte_demais_textos, "TEMPO", posicao_texto_tempo, tamanho_fonte_texto_tempo, 2.0f, YELLOW, GRAY, 1.0f);

        float tamanho_fonte_texto_segundos = tamanho_fonte_texto_tempo;
        const char* texto_segundos = TextFormat("%d", tempo_exibido);
        Vector2 tamanho_texto_segundos = MeasureTextEx(fonte_demais_textos, texto_segundos, tamanho_fonte_texto_segundos, 2.0f);
        float pos_texto_segundos_x = pos_texto_tempo_x + (tamanho_texto_tempo.x / 2.0f) - (tamanho_texto_segundos.x / 2.0f);
        float pos_texto_segundos_y = pos_texto_tempo_y + (tamanho_texto_tempo.y * 1.5f);

        Vector2 posicao_texto_segundos = {
            pos_texto_segundos_x, pos_texto_segundos_y
        };
        DrawTextWithOutline(fonte_demais_textos, texto_segundos, posicao_texto_segundos, tamanho_fonte_texto_segundos, 2.0f, YELLOW, GRAY, 1.0f);

        // Corações no lado esquerdo do HUD: apenas os corações cheios, um por vida.
        // Mais fácil de contar que corações vazios. O João pode trocar por sprites depois.
        // Durante invencibilidade, pisca alternando entre vermelho e dourado.

        pos_vidas_x = (float)JANELA_LARGURA * 0.05f;
        pos_vidas_y = pos_texto_tempo_y;
        gap = (float)imagens.coracao.width * 2.0f + 5.0f;

        for (v = 0; v < jogo->jogador.vidas; v++)
        {
            Color cor_coracao;


            pos_coracao = (Vector2) {pos_vidas_x + (float)v * gap, pos_vidas_y};

            // Pisca entre vermelho e dourado enquanto invencível
            if (jogo->jogador.invencivel && ((int)(jogo->jogador.tempo_invencibilidade / 0.25f) % 2 == 0)){
                 cor_coracao = (Color){180, 180, 255, 255}; // azulado;
            }
            else{
                 cor_coracao = YELLOW;
            }

            if(jogo->jogador.invencivel){
                DrawTextureEx(imagens.coracao, pos_coracao, 0.0f, 2.0f, cor_coracao);
            }
            else{
                DrawTextureEx(imagens.coracao, pos_coracao, 0.0f, 2.0f, WHITE);
            }

            
        }

        // Fase atual no centro do HUD
        const char* texto_fase = TextFormat("FASE %d", jogo->fase_atual + 1);
        Vector2 tamanho_texto_fase = MeasureTextEx(fonte_demais_textos, texto_fase, tamanho_fonte_texto_tempo, 2.0f);
        float pos_fase_x = ((float)JANELA_LARGURA - tamanho_texto_fase.x) / 2.0f;
        DrawTextWithOutline(fonte_demais_textos, texto_fase, (Vector2){pos_fase_x, pos_texto_tempo_y}, tamanho_fonte_texto_tempo, 2.0f, WHITE, GRAY, 1.0f);
    }
}

// Desenha os objetos que se movem (as entidades)
void desenha_entidades(const Jogo *jogo)
{
    Color branco_transparente = WHITE;
    Color azulado_transparente = {180, 180, 255, 255};
    Color cores[2] = {branco_transparente, azulado_transparente};
    int indice;
    if(jogo->jogador.ativo == true){
        if(jogo->jogador.invencivel){
            if((int)(jogo->jogador.tempo_invencibilidade / 0.25f) % 2 == 0){
                indice = 1;
            }
            else{
                indice = 0;
            }
            
            DrawTexturePro(
                imagens.mario,
                tamanho_frames(&jogo->jogador.animacao, FRAMES_POR_LINHA, jogo->jogador.direcao_horizontal),                     // Define aonde será o corte na imagem original
                (Rectangle){jogo->jogador.posicao_pixels.x, jogo->jogador.posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE}, // Define aonde ficará o mario no jogo
                (Vector2){0.0f, 0.0f},                                                                                           // Define o eixo principal de rotação
                0.0f,                                                                                                            // Define a quantia de rotação
                cores[indice]                                                                                                    // Define a cor da imagem
            );                                                                                                         
        }
        else{
            DrawTexturePro(
                imagens.mario,
                tamanho_frames(&jogo->jogador.animacao, FRAMES_POR_LINHA, jogo->jogador.direcao_horizontal),                     // Define aonde será o corte na imagem original
                (Rectangle){jogo->jogador.posicao_pixels.x, jogo->jogador.posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE}, // Define aonde ficará o mario no jogo
                (Vector2){0.0f, 0.0f},                                                                                           // Define o eixo principal de rotação
                0.0f,                                                                                                            // define a quantia de rotação
                WHITE
        ); 

        }
    }

    if (jogo->princesa.ativo == true)
    {
        DrawTexturePro(
            imagens.princesa,
            tamanho_frames(&jogo->princesa.animacao, FRAMES_POR_LINHA, jogo->princesa.direcao_horizontal),
            (Rectangle){jogo->princesa.posicao_pixels.x, jogo->princesa.posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE},
            (Vector2){0.0f, 0.0f},
            0.0f,
            WHITE
        );
    }


    if (jogo->donkey.ativo == true){
    DrawTexturePro(
        imagens.donkey, 
        tamanho_frames(&jogo->donkey.animacao, FRAMES_POR_LINHA, jogo->donkey.direcao_horizontal),
        (Rectangle){jogo->donkey.posicao_pixels.x, jogo->donkey.posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE},
        (Vector2){0.0f, 0.0f},
        0.0f,
        WHITE
    );
    }                                                         

    for(int num_inimigos = 0; num_inimigos < jogo->quantidade_inimigos; num_inimigos++){
        if(jogo->inimigos[num_inimigos].ativo == true){
            // Inimigos velozes são desenhados em vermelho para se diferenciar dos patrulheiros
            Color cor_inimigo = (jogo->inimigos[num_inimigos].tipo == INIMIGO_VELOZ) ? RED : WHITE;
            DrawTexturePro(
                imagens.fantasma,
                tamanho_frames(&jogo->inimigos[num_inimigos].animacao, FRAMES_POR_LINHA, jogo->inimigos[num_inimigos].direcao_horizontal),
                (Rectangle){jogo->inimigos[num_inimigos].posicao_pixels.x, jogo->inimigos[num_inimigos].posicao_pixels.y, (float)TILE_SIZE, (float)TILE_SIZE},
                (Vector2){0.0f, 0.0f},
                0.0f,
                cor_inimigo
            );
        }
}
}

// Tela de game over provisória — o João pode elaborar depois.
void desenha_game_over(const Jogo* jogo, Font fonte)
{
    const char* texto = "GAME OVER";
    const char* instrucao = "Pressione ENTER ou ESPACO para continuar";
    Vector2 tamanho;
    float pos_x;
    float pos_y;

    (void)jogo;

    tamanho = MeasureTextEx(fonte, texto, TAMANHO_FONTE_DIGITACAO * 2.0f, 2.0f);
    pos_x = ((float)JANELA_LARGURA - tamanho.x) / 2.0f;
    pos_y = ((float)JANELA_ALTURA - tamanho.y) / 2.0f;
    DrawTextWithOutline(fonte, texto, (Vector2){pos_x, pos_y}, TAMANHO_FONTE_DIGITACAO * 2.0f, 2.0f, RED, DARKGRAY, 3.0f);

    Vector2 tam_instrucao = MeasureTextEx(fonte, instrucao, TAMANHO_FONTE_DIGITACAO * 0.6f, 2.0f);
    DrawTextEx(fonte, instrucao,
        (Vector2){((float)JANELA_LARGURA - tam_instrucao.x) / 2.0f, pos_y + tamanho.y * 1.8f},
        TAMANHO_FONTE_DIGITACAO * 0.6f, 2.0f, GRAY);
}

void desenha_vitoria(const Jogo* jogo, Font fonte)
{
    const char* texto = "VOCE VENCEU";
    const char* instrucao = "Pressione ENTER ou ESPACO para continuar";
    Vector2 tamanho;
    float pos_x;
    float pos_y;

    (void)jogo;

    tamanho = MeasureTextEx(fonte, texto, TAMANHO_FONTE_DIGITACAO * 2.0f, 2.0f);
    pos_x = ((float)JANELA_LARGURA - tamanho.x) / 2.0f;
    pos_y = ((float)JANELA_ALTURA - tamanho.y) / 2.0f;
    DrawTextWithOutline(fonte, texto, (Vector2){pos_x, pos_y}, TAMANHO_FONTE_DIGITACAO * 2.0f, 2.0f, GREEN, DARKGRAY, 3.0f);

    Vector2 tam_instrucao = MeasureTextEx(fonte, instrucao, TAMANHO_FONTE_DIGITACAO * 0.6f, 2.0f);
    DrawTextEx(fonte, instrucao,
        (Vector2){((float)JANELA_LARGURA - tam_instrucao.x) / 2.0f, pos_y + tamanho.y * 1.8f},
        TAMANHO_FONTE_DIGITACAO * 0.6f, 2.0f, GRAY);
}

// Power-ups são desenhados com cores distintas por tipo enquanto não há sprite:
// amarelo = reduz tempo, verde = vida extra, ciano = invencibilidade
void desenha_powerups(const Jogo* jogo)
{
    int i;
    

    for (i = 0; i < jogo->quantidade_powerups; i++)
    {
        Vector2 posicao = {jogo->powerups[i].posicao_pixels.x, jogo->powerups[i].posicao_pixels.y};
        if (!jogo->powerups[i].ativo)
        {
            continue;
        }

        if(jogo->powerups[i].tipo == POWERUP_VIDA){
        
            DrawTextureV(imagens.coracao, posicao, WHITE);
    }
        if(jogo->powerups[i].tipo == POWERUP_TEMPO){
            DrawTextureV(imagens.relogio, posicao, WHITE);
    }
        if (jogo->powerups[i].tipo == POWERUP_INVENCIVEL){
            DrawTextureV(imagens.estrela, posicao, WHITE);
        }
    }
}

void desenha_menu_pausa(int opcao_selecionada)
{
    const char* titulo = "PAUSADO";
    const char* opcoes[3] = {"CONTINUAR", "VOLTAR AO MENU", "SAIR"};
    float tamanho_fonte = TAMANHO_FONTE_DIGITACAO;
    float gap = tamanho_fonte * 1.8f;
    Vector2 tamanho_titulo;
    Vector2 pos;
    int i;

    // Escurece o jogo congelado por trás para destacar o menu
    DrawRectangle(0, 0, JANELA_LARGURA, JANELA_ALTURA, (Color){0, 0, 0, 180});

    tamanho_titulo = MeasureTextEx(fonte_demais_textos, titulo, tamanho_fonte * 1.5f, 2.0f);
    pos.x = ((float)JANELA_LARGURA - tamanho_titulo.x) / 2.0f;
    pos.y = ((float)JANELA_ALTURA - (gap * 4.0f)) / 2.0f;
    DrawTextWithOutline(fonte_demais_textos, titulo, pos, tamanho_fonte * 1.5f, 2.0f, WHITE, DARKGRAY, 2.0f);

    pos.y += gap * 1.5f;
    for (i = 0; i < 3; i++)
    {
        Vector2 tamanho_opcao = MeasureTextEx(fonte_demais_textos, opcoes[i], tamanho_fonte, 2.0f);
        pos.x = ((float)JANELA_LARGURA - tamanho_opcao.x) / 2.0f;
        Color cor = (i == opcao_selecionada) ? YELLOW : WHITE;
        DrawTextEx(fonte_demais_textos, opcoes[i], pos, tamanho_fonte, 2.0f, cor);
        pos.y += gap;
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
            desenha_powerups(jogo);
            desenha_elementos_HUD(jogo);
            desenha_entidades(jogo);
            break;
        case TELA_PAUSADO:
            ClearBackground(BLACK);
            desenha_mapa(jogo);
            desenha_powerups(jogo);
            desenha_elementos_HUD(jogo);
            desenha_entidades(jogo);
            desenha_menu_pausa(jogo->opcao_pausa);
            break;
        case TELA_MENU_PRINCIPAL:
            ClearBackground(LIGHTGRAY);
            desenha_menu_principal(jogo, fonte_jogo, fonte_demais_textos, imagens.mario, imagens.princesa, imagens.donkey);
            break;
        case TELA_RANKING:
            ClearBackground(LIGHTGRAY);
            desenha_tela_ranking(jogo, fonte_jogo, fonte_demais_textos);
            break;
        case TELA_VITORIA:
            ClearBackground(BLACK);
            desenha_vitoria(jogo, fonte_demais_textos);
            break;
        case TELA_GAME_OVER:
            ClearBackground(BLACK);
            desenha_game_over(jogo, fonte_demais_textos);
            break;
        case TELA_DIGITANDO_NOME:
            ClearBackground(BLACK);
            atualiza_input_nome();
            toca_audio_digitando();
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
    UnloadTexture(imagens.relogio);
    UnloadTexture(imagens.coracao);
    UnloadTexture(imagens.estrela);

    UnloadFont(fonte_jogo);
    UnloadFont(fonte_demais_textos);
}