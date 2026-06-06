#include "raylib.h"
#include "menu.h"
#include <string.h>
#include "../io/placar_io.h"
#include "../core/constants.h"

#define QUANTIA_BOTOES 4

// ===== VARIÁVEIS ESTÁTICAS DO MÓDULO =====
static int indice_teclado_inicial = 0;
static bool ja_tocou_som_inicial = false;
static bool primeira_tela_um = true;
static int indice_nome = 0;
static char nomes[10 + 1];

// ===== FUNÇÕES AUXILIARES =====

void DrawTextWithOutline(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color textColor, Color outlineColor, float outlineThickness){
    // Desenha o contorno nas 4 direções cardinais (Esquerda, Direita, Cima, Baixo)
    DrawTextEx(font, text, (Vector2){ position.x - outlineThickness, position.y }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x + outlineThickness, position.y }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x, position.y - outlineThickness }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x, position.y + outlineThickness }, fontSize, spacing, outlineColor);
    
    // Por fim, desenha o texto principal por cima de tudo
    DrawTextEx(font, text, position, fontSize, spacing, textColor);
}

bool tocar_sons_botao(Rectangle retangulo, Vector2 posicao_teclado, Vector2 posicao_mouse, int num_botao){
    static int botao_que_ja_tocou = -1;
    
    // Detectar clique do mouse ou enter do teclado
    if(num_botao == 1){
        if (CheckCollisionPointRec(posicao_mouse, retangulo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            tocar_audio_efeito("inicio");
            return true;
        }
        if(CheckCollisionPointRec(posicao_teclado, retangulo) && IsKeyPressed(KEY_ENTER)){
            tocar_audio_efeito("inicio");
            return true;
        }
    }
    else if(num_botao == 2){
        if (CheckCollisionPointRec(posicao_mouse, retangulo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            tocar_audio_efeito("ver_placar");
            return true;
        }
        if(CheckCollisionPointRec(posicao_teclado, retangulo) && IsKeyPressed(KEY_ENTER)){
            tocar_audio_efeito("ver_placar");
            return true;
        }
    }
    else if(num_botao == 3){
        if (CheckCollisionPointRec(posicao_mouse, retangulo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            tocar_audio_efeito("botao_sair");
            return true;
        }
        if(CheckCollisionPointRec(posicao_teclado, retangulo) && IsKeyPressed(KEY_ENTER)){
            tocar_audio_efeito("botao_sair");
            return true;
        }
    }

    // Sons quando troca a opção
    if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
        tocar_audio_efeito("troca_opcao");
    }
    
    if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
        tocar_audio_efeito("troca_opcao");
    }

    // Toca som ao passar o mouse sobre um botão (controle para não tocar repetidamente)
    if(CheckCollisionPointRec(posicao_mouse, retangulo)){
        int botao_tocado_atual = num_botao;
        if(botao_tocado_atual != botao_que_ja_tocou){
            tocar_audio_efeito("troca_opcao");
            botao_que_ja_tocou = botao_tocado_atual;
        }
    } 
    else {
        if (botao_que_ja_tocou == num_botao) {
            botao_que_ja_tocou = -1;
        }
    }

    return false;
}

void determina_posicoes_inputs(Vector2 *posicoes_teclado, int *indice){
    if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
        switch (*indice){
            case 0: *indice = 1; break;
            case 1: *indice = 2; break;
            case 2: *indice = 3; break;
            case 3: *indice = 1; break;
        }
    }
    else if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
        switch (*indice){
            case 0: *indice = 1; break;
            case 1: *indice = 3; break;
            case 2: *indice = 1; break;
            case 3: *indice = 2; break;
        }
    }
}

bool determina_clique_retangulo(Rectangle retangulo_interagivel, Vector2 posicao_mouse){
    if(CheckCollisionPointRec(posicao_mouse, retangulo_interagivel) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        return true;
    }
    else{
        return false;
    }
}
// ===== MENU PRINCIPAL =====
void atualiza_menu_principal(Jogo *jogo, Vector2 *posicoes_teclado, Vector2 posicao_mouse, 
                              Rectangle retangulo_jogar, Rectangle retangulo_placar, Rectangle retangulo_sair){
    // Toca primeiro som do jogo
    if(!ja_tocou_som_inicial){
        tocar_audio_efeito("abre_jogo");
        ja_tocou_som_inicial = true;
    }

    // Atualizar posição do teclado
    determina_posicoes_inputs(posicoes_teclado, &indice_teclado_inicial);

    // Lógica do botão Jogar
    if(tocar_sons_botao(retangulo_jogar, posicoes_teclado[indice_teclado_inicial], posicao_mouse, 1)){
        jogo->tela_atual = TELA_JOGANDO;
        jogo->tempos_telas.segundos_ate_jogar = GetTime();
    }

    // Lógica do botão Ranking
    if(tocar_sons_botao(retangulo_placar, posicoes_teclado[indice_teclado_inicial], posicao_mouse, 2)){
        jogo->tela_atual = TELA_RANKING;
    }

    // Lógica do botão Sair
    if(tocar_sons_botao(retangulo_sair, posicoes_teclado[indice_teclado_inicial], posicao_mouse, 3)){
        jogo->tela_atual = TELA_SAIR;
    }
}

void desenha_menu_principal(Jogo *jogo, Font fonte_jogo, Font fonte_botoes, Texture2D mario, Texture2D princesa, Texture2D donkey){
    float x_centro = (float)JANELA_LARGURA / 2.0f;
    float y_centro = (float)JANELA_ALTURA / 2.0f;
    
    // ===== POSICIONAMENTO DO TÍTULO =====
    float tamanho_fonte_titulo = TAMANHO_FONTE_TITULO_MENU;
    Vector2 posicao_texto = {x_centro - 300.0f, (float)JANELA_ALTURA * 0.1f};
    Vector2 posicao_sombra = {posicao_texto.x + 8.0f, posicao_texto.y + 8.0f};

    // ===== DEFINIR TAMANHO DOS RETÂNGULOS =====
    float retangulo_base = 190.0f;
    float retangulo_altura = 80.0f;
    float x_centro_retangulo = x_centro - (retangulo_base / 2.0f);
    float y_centro_retangulo = y_centro - (retangulo_altura / 2.0f);

    Rectangle retangulo_jogar = {x_centro_retangulo, y_centro_retangulo - (retangulo_altura * 1.5), retangulo_base, retangulo_altura};
    Rectangle retangulo_placar = {x_centro_retangulo, y_centro_retangulo, retangulo_base, retangulo_altura};
    Rectangle retangulo_sair = {x_centro_retangulo, y_centro_retangulo + (retangulo_altura * 1.5), retangulo_base, retangulo_altura};

    // ===== POSIÇÕES DAS PERSONAGENS =====
    float x_bonecos = x_centro - ((float)TILE_SIZE / 2.0f);
    Vector2 pos_mario = {x_bonecos, retangulo_jogar.y - (float)TILE_SIZE};
    Vector2 pos_princesa = {x_bonecos, retangulo_placar.y - (float)TILE_SIZE};
    Vector2 pos_donkey = {x_bonecos, retangulo_sair.y - (float)TILE_SIZE};

    // ===== POSIÇÕES DO TECLADO =====
    Vector2 posicao_mouse = GetMousePosition();
    Vector2 posicoes_teclado[QUANTIA_BOTOES] = {
        {0, 0}, {retangulo_jogar.x, retangulo_jogar.y}, {retangulo_placar.x, retangulo_placar.y}, {retangulo_sair.x, retangulo_sair.y}
    };

    // Atualizar lógica do menu
    atualiza_menu_principal(jogo, posicoes_teclado, posicao_mouse, retangulo_jogar, retangulo_placar, retangulo_sair);

    // ===== DESENHAR RETÂNGULOS =====
    
    // Retângulo Jogar
    if(!(CheckCollisionPointRec(posicao_mouse, retangulo_jogar) || CheckCollisionPointRec(posicoes_teclado[indice_teclado_inicial], retangulo_jogar))){
        DrawRectangleRounded(retangulo_jogar, 0.2, 4, GREEN);
    } else {
        DrawRectangleRounded(retangulo_jogar, 0.2, 4, RED);
        DrawTexturePro(mario, animation_frame(&jogo->jogador.animacao, FRAMES_POR_LINHA), 
                      (Rectangle){pos_mario.x, pos_mario.y, (float)TILE_SIZE, (float)TILE_SIZE}, 
                      (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }

    // Retângulo Placar
    if(!(CheckCollisionPointRec(posicao_mouse, retangulo_placar) || CheckCollisionPointRec(posicoes_teclado[indice_teclado_inicial], retangulo_placar))){
        DrawRectangleRounded(retangulo_placar, 0.2, 4, GREEN);
    } else {
        DrawRectangleRounded(retangulo_placar, 0.2, 4, RED);
        DrawTexturePro(princesa, animation_frame(&jogo->princesa.animacao, FRAMES_POR_LINHA), 
                      (Rectangle){pos_princesa.x, pos_princesa.y, (float)TILE_SIZE, (float)TILE_SIZE}, 
                      (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }

    // Retângulo Sair
    if(!(CheckCollisionPointRec(posicao_mouse, retangulo_sair) || CheckCollisionPointRec(posicoes_teclado[indice_teclado_inicial], retangulo_sair))){
        DrawRectangleRounded(retangulo_sair, 0.2, 4, GREEN);
    } else {
        DrawRectangleRounded(retangulo_sair, 0.2, 4, RED);
        DrawTexturePro(donkey, animation_frame(&jogo->donkey.animacao, FRAMES_POR_LINHA), 
                      (Rectangle){pos_donkey.x, pos_donkey.y, (float)TILE_SIZE, (float)TILE_SIZE}, 
                      (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }

    // Desenhar contornos dos retângulos
    DrawRectangleRoundedLines(retangulo_jogar, 0.2, 4, BLACK);
    DrawRectangleRoundedLines(retangulo_placar, 0.2, 4, BLACK);
    DrawRectangleRoundedLines(retangulo_sair, 0.2, 4, BLACK);

    // ===== DESENHAR TEXTOS NOS BOTÕES =====
    float tamanho_fonte_botao = TAMANHO_FONTE_BOTAO_MENU;

    // Texto Jogar
    Vector2 texto_jogar = MeasureTextEx(fonte_botoes, "JOGAR", tamanho_fonte_botao, 2.0);
    Vector2 pos_texto_jogar = {
        retangulo_jogar.x + (retangulo_base - texto_jogar.x) / 2.0f,
        retangulo_jogar.y + (retangulo_altura - texto_jogar.y) / 2.0f
    };
    DrawTextWithOutline(fonte_botoes, "JOGAR", pos_texto_jogar, tamanho_fonte_botao, 2.0f, WHITE, BLACK, 2.0f);

    // Texto Placar
    Vector2 texto_placar = MeasureTextEx(fonte_botoes, "PLACAR", tamanho_fonte_botao, 2.0);
    Vector2 pos_texto_placar = {
        retangulo_placar.x + (retangulo_base - texto_placar.x) / 2.0f,
        retangulo_placar.y + (retangulo_altura - texto_placar.y) / 2.0f
    };
    DrawTextWithOutline(fonte_botoes, "PLACAR", pos_texto_placar, tamanho_fonte_botao, 2.0f, WHITE, BLACK, 2.0f);

    // Texto Sair
    Vector2 texto_sair = MeasureTextEx(fonte_botoes, "SAIR", tamanho_fonte_botao, 2.0);
    Vector2 pos_texto_sair = {
        retangulo_sair.x + (retangulo_base - texto_sair.x) / 2.0f,
        retangulo_sair.y + (retangulo_altura - texto_sair.y) / 2.0f
    };
    DrawTextWithOutline(fonte_botoes, "SAIR", pos_texto_sair, tamanho_fonte_botao, 2.0f, WHITE, BLACK, 2.0f);

    // ===== DESENHAR TÍTULO COM EFEITOS =====
    DrawTextEx(fonte_jogo, "Mario vs. Donkey Kong", posicao_sombra, tamanho_fonte_titulo, 2.0f, BLACK);
    DrawTextWithOutline(fonte_jogo, "Mario vs. Donkey Kong", posicao_texto, tamanho_fonte_titulo, 2.0f, RED, BLACK, 5.0f);
}

// ===== TELA DE RANKING =====

Vector2 determina_posicao_ranking(TipoPlacar placar_atual, char opcao, int indice, Font fonte_nomes, Rectangle retangulo_calculos){
    switch (opcao)
    {
    case 'n':
    case 'N':
        Vector2 tamanho_texto = MeasureTextEx(fonte_nomes, placar_atual.nome, TAMANHO_FONTE_NOMES_RANKING,  2.0f);
        Vector2 posicao_texto = {
            retangulo_calculos.x + (retangulo_calculos.width - tamanho_texto.x) / 2.0f,
            retangulo_calculos.y + (retangulo_calculos.height - tamanho_texto.y) / 2.0f + (retangulo_calculos.height * indice)
        };
        return posicao_texto;
        break;
    case 't':
    case 'T':
        Vector2 tamanho_texto_tempo = MeasureTextEx(fonte_nomes, TextFormat("%d",placar_atual.time), TAMANHO_FONTE_NOMES_RANKING,  2.0f);
        Vector2 posicao_texto_dois = {
            retangulo_calculos.x + (retangulo_calculos.width - tamanho_texto_tempo.x) / 2.0f + retangulo_calculos.width,
            retangulo_calculos.y + (retangulo_calculos.height - tamanho_texto_tempo.y) / 2.0f + (retangulo_calculos.height * indice)
        };
        return posicao_texto_dois;
    
    default:
        break;
    }
}

void desenha_retangulos_menores_ranking(const Rectangle retangulo_exterior, Font fonte_nomes, Jogo* jogo){
    Vector2 pos_mouse =  GetMousePosition();
    
    static const int NUM_LINHAS = 7;
    static const int NUM_COLUNAS = 2;
    
    float retangulos_base = retangulo_exterior.width / NUM_COLUNAS;
    float retangulos_altura = retangulo_exterior.height / NUM_LINHAS;
    float retangulos_x_in = retangulo_exterior.x;
    float retangulos_y_in = retangulo_exterior.y;
    
    // Define cabeçalhos (primeira linha)
    Rectangle retangulo_posicao_um = {retangulos_x_in, retangulos_y_in, retangulos_base, retangulos_altura};
    Rectangle retangulo_posicao_dois = {retangulos_x_in + retangulos_base, retangulos_y_in, retangulos_base, retangulos_altura};
    // Define ultima linha (interagível)
    Rectangle ultimo_retangulo_um = {
        retangulos_x_in, retangulos_y_in + retangulos_altura * (NUM_LINHAS - 1), retangulos_base, retangulos_altura
    };
    Rectangle ultimo_retangulo_dois = {
        retangulos_x_in + retangulos_base, retangulos_y_in + retangulos_altura * (NUM_LINHAS - 1), retangulos_base, retangulos_altura
    };
    //Define a posição dos titulos (1° linha)

    Vector2 tamanho_titulo = MeasureTextEx(fonte_nomes, "NOMENOMENO", TAMANHO_FONTE_NOMES_RANKING, 2.0f);
    float posicao_titulo_um_x =  retangulos_x_in + (retangulos_base - tamanho_titulo.x) / 2.0f;
    float posicao_titulo_um_y = retangulos_y_in + (retangulos_altura - tamanho_titulo.y) / 2.0f;
    Vector2 posicao_titulo_um = {posicao_titulo_um_x, posicao_titulo_um_y};
    Vector2 posicao_titulo_dois = {posicao_titulo_um_x + retangulos_base, posicao_titulo_um_y};

    // Define a posição do numero dos placares (6° linha)
    Vector2 tamanho_numeros_placar = MeasureTextEx(fonte_nomes, TextFormat("%d", 1), TAMANHO_FONTE_NOMES_RANKING, 2.0f);
    float posicao_numeros_placar_x = retangulos_x_in + (retangulos_base - tamanho_numeros_placar.x) / 2.0f;
    float posicao_numeros_placar_y = retangulos_y_in + (retangulos_altura - tamanho_titulo.y) / 2.0f;
    Vector2 posicao_numero_um = {posicao_numeros_placar_x, posicao_numeros_placar_y + (retangulos_altura * (NUM_LINHAS - 1))};
    Vector2 posicao_numero_dois = {posicao_numeros_placar_x + retangulos_base, posicao_numeros_placar_y + (retangulos_altura * (NUM_LINHAS - 1))};


    // Define qual parte do ranking que será exibida
    if(determina_clique_retangulo(ultimo_retangulo_um, pos_mouse)){
        primeira_tela_um = true;
    }
    else if(determina_clique_retangulo(ultimo_retangulo_dois, pos_mouse)){
        primeira_tela_um = false;
        
    }

    // Desenhar linhas de dados (restante das linhas)
    for(int indice = 0; indice < NUM_LINHAS; indice++){

        // Declara os outros retângulos que serão desenhador
        Rectangle retangulo_abaixo_um = {retangulos_x_in, retangulos_y_in + retangulos_altura * indice, retangulos_base, retangulos_altura};

        Rectangle retangulo_abaixo_dois = {retangulos_x_in + retangulos_base, retangulos_y_in + retangulos_altura * indice, retangulos_base, retangulos_altura};


        // Desenha os da esquerda
        DrawRectangleRec(retangulo_abaixo_um, RAYWHITE);
        DrawRectangleLinesEx(retangulo_abaixo_um, 5, BLACK);
        // Desenha os da direita
        DrawRectangleRec(retangulo_abaixo_dois, RAYWHITE);
        DrawRectangleLinesEx(retangulo_abaixo_dois, 5, BLACK);

        if(primeira_tela_um == false) {
            if(indice > 0 && indice < 6 && jogo->placar[indice - 1 + MAX_APARECE_RANKING].time != LIMITE_SEGUNDOS){
                char n = 'n', t = 't'; // Informa para a função se quero extrair o nome do placar ou o tempo do placar 
                Vector2 posicao_nome = determina_posicao_ranking(jogo->placar[indice - 1 + MAX_APARECE_RANKING], n, indice, fonte_nomes, retangulo_posicao_um);
                Vector2 posicao_tempo = determina_posicao_ranking(jogo->placar[indice - 1 + MAX_APARECE_RANKING], t, indice, fonte_nomes, retangulo_posicao_um);
                DrawTextEx(fonte_nomes,jogo->placar[indice - 1 + MAX_APARECE_RANKING].nome, posicao_nome, TAMANHO_FONTE_NOMES_RANKING, 2.0f, BLACK);
                DrawTextEx(fonte_nomes, TextFormat("%d", jogo->placar[indice - 1 + MAX_APARECE_RANKING].time), posicao_tempo, TAMANHO_FONTE_NOMES_RANKING, 2.0f, BLACK);
            }
        }
        else{
            if(indice > 0 && indice < 6 && jogo->placar[indice - 1].time != LIMITE_SEGUNDOS){
                char n = 'n', t = 't'; // Informa para a função se quero extrair o nome do placar ou o tempo do placar 
                Vector2 posicao_nome = determina_posicao_ranking(jogo->placar[indice - 1 ], n, indice, fonte_nomes, retangulo_posicao_um);
                Vector2 posicao_tempo = determina_posicao_ranking(jogo->placar[indice - 1], t, indice, fonte_nomes, retangulo_posicao_um);
                DrawTextEx(fonte_nomes,jogo->placar[indice - 1].nome, posicao_nome, TAMANHO_FONTE_NOMES_RANKING, 2.0f, BLACK);
                DrawTextEx(fonte_nomes, TextFormat("%d", jogo->placar[indice - 1].time), posicao_tempo, TAMANHO_FONTE_NOMES_RANKING, 2.0f, BLACK);
            }
        }

        if(indice == 0){
        
            DrawRectangleRec(retangulo_abaixo_um, GRAY);
            DrawRectangleLinesEx(retangulo_abaixo_um, 5, BLACK);
            DrawRectangleRec(retangulo_abaixo_dois, GRAY);
            DrawRectangleLinesEx(retangulo_abaixo_dois, 5, BLACK);
            // Escreve textos do titulo
            DrawTextEx(fonte_nomes, "NOME", posicao_titulo_um, TAMANHO_FONTE_NOMES_RANKING, 2.0f, BLACK);
            DrawTextEx(fonte_nomes, "TEMPO", posicao_titulo_dois, TAMANHO_FONTE_NOMES_RANKING, 2.0f, BLACK);
        }

        if(indice == 6){
            if(CheckCollisionPointRec(pos_mouse, ultimo_retangulo_um)){
                DrawRectangleRec(retangulo_abaixo_um, LIGHTGRAY);
            }
            else{
                DrawRectangleRec(retangulo_abaixo_um, GRAY);
            }
                DrawRectangleLinesEx(retangulo_abaixo_um, 5, BLACK);
            
            if(CheckCollisionPointRec(pos_mouse, ultimo_retangulo_dois)){
                DrawRectangleRec(retangulo_abaixo_dois, LIGHTGRAY);
            }
            else{
                DrawRectangleRec(retangulo_abaixo_dois, GRAY);
            }
                DrawRectangleLinesEx(retangulo_abaixo_dois, 5, BLACK);
            
            // Escreve os numeros
            DrawTextEx(fonte_nomes, TextFormat("%d", 1), posicao_numero_um, TAMANHO_FONTE_NOMES_RANKING, 2.0f, BLACK);
            DrawTextEx(fonte_nomes, TextFormat("%d", 2), posicao_numero_dois, TAMANHO_FONTE_NOMES_RANKING, 2.0f, BLACK); 

        }

    

    }
        
    }

void desenha_tela_ranking(Jogo *jogo, Font fonte_nomes, Font fonte_jogo){
    Vector2 pos_mouse = GetMousePosition();
    
    float x_centro = JANELA_LARGURA / 2.0f;
    float y_centro = JANELA_ALTURA / 2.0f;

    // ===== TÍTULO DO RANKING =====
    float tamanho_fonte_titulo = TAMANHO_FONTE_TITULO_RANKING;
    Vector2 tamanho_titulo = MeasureTextEx(fonte_jogo, "RANKING", tamanho_fonte_titulo, 2.0f);
    float x_centro_texto = x_centro - (tamanho_titulo.x / 2.0f);
    float y_texto = JANELA_ALTURA - (JANELA_ALTURA * 0.9f) - (tamanho_titulo.y / 2);
    Vector2 pos_titulo_ranking = {x_centro_texto, y_texto};

    DrawTextWithOutline(fonte_jogo, "RANKING", pos_titulo_ranking, tamanho_fonte_titulo, 2.0f, BLACK, WHITE, 2.0f);

    // ===== TABELA DE RANKING =====
    float retangulo_ranking_base = JANELA_LARGURA * 0.8f;
    float retangulo_ranking_altura = JANELA_ALTURA * 0.7f;
    float retangulo_ranking_pos_x = x_centro - (retangulo_ranking_base / 2);
    float retangulo_ranking_pos_y = y_centro - (retangulo_ranking_altura / 2);
    Rectangle retangulo_ranking = {
        retangulo_ranking_pos_x, retangulo_ranking_pos_y, retangulo_ranking_base, retangulo_ranking_altura
    };

    // Retângulo menor (interior)
    float retangulo_ranking_menor_base = retangulo_ranking_base * 0.8;
    float retangulo_ranking_menor_altura = retangulo_ranking_altura * 0.8;
    float retangulo_ranking_menor_pos_x = retangulo_ranking_pos_x + ((retangulo_ranking_base - retangulo_ranking_menor_base) / 2);
    float retangulo_ranking_menor_pos_y = retangulo_ranking_pos_y + ((retangulo_ranking_altura - retangulo_ranking_menor_altura) / 2);
    Rectangle retangulo_ranking_menor = {
        retangulo_ranking_menor_pos_x, retangulo_ranking_menor_pos_y, retangulo_ranking_menor_base, retangulo_ranking_menor_altura
    };

    DrawRectangleRounded(retangulo_ranking, 0.2, 3, RED);
    DrawRectangleRec(retangulo_ranking_menor, RAYWHITE);
    DrawRectangleLinesEx(retangulo_ranking_menor, 5, BLACK);
    DrawRectangleRoundedLinesEx(retangulo_ranking, 0.2, 3, 5, BLACK);

    // Desenhar os retângulos menores da tabela
    desenha_retangulos_menores_ranking(retangulo_ranking_menor, fonte_nomes, jogo);

    // ===== BOTÃO DE VOLTAR =====
    float tamanho_fonte_x = TAMANHO_FONTE_BOTAO_UI;
    Vector2 tamanho_x = MeasureTextEx(fonte_nomes, "<", tamanho_fonte_x, 2.0f);
    
    float retangulo_x_base = tamanho_x.x * 1.5f;
    float retangulo_x_altura = tamanho_x.y * 1.5f;
    float retangulo_x_pos = 20.0f;
    float retangulo_y_pos = 20.0f;
    
    Rectangle retangulo_x = {retangulo_x_pos, retangulo_y_pos, retangulo_x_base, retangulo_x_altura};
    
    float posicao_xx = retangulo_x.x + (retangulo_x.width - tamanho_x.x) / 2.0f;
    float posicao_xy = retangulo_x.y + (retangulo_x.height - tamanho_x.y) / 2.0f;
    Vector2 posicao_x = {posicao_xx, posicao_xy};
    
    DrawRectangleRounded(retangulo_x, 0.2f, 4, RED);
    DrawRectangleRoundedLines(retangulo_x, 0.2f, 4, BLACK);
    DrawTextEx(fonte_nomes, "<", posicao_x, tamanho_fonte_x, 2.0f, BLACK);

    // Lógica do botão de voltar para o menu
    if(CheckCollisionPointRec(pos_mouse, retangulo_x)){
        DrawRectangleRounded(retangulo_x, 0.2f, 4, GRAY);
        DrawRectangleRoundedLines(retangulo_x, 0.2f, 4, BLACK);
        DrawTextEx(fonte_nomes, "<", posicao_x, tamanho_fonte_x, 2.0f, BLACK);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            jogo->tela_atual = TELA_MENU_PRINCIPAL;
        }
    }
}

// ===== DIGITAÇÃO DE NOME =====

void desenha_menu_nome(Jogo *jogo, Font fonte_textos){
    float tamanho_fonte = TAMANHO_FONTE_DIGITACAO;

    // ===== CAPTURAR ENTRADAS DO TECLADO =====
    char letra_atual = GetCharPressed();
    while(letra_atual > 0) {
        if(indice_nome < TAM_MAX_NOME){
            nomes[indice_nome] = letra_atual;
            indice_nome++;
            nomes[indice_nome] = '\0';
        }
        letra_atual = GetCharPressed();
    }

    // Backspace com proteção contra índice negativo
    if(IsKeyPressed(KEY_BACKSPACE) && indice_nome > 0){
        indice_nome--;
        nomes[indice_nome] = '\0';
    }

    // ===== DESENHAR RETÂNGULO DE ENTRADA =====
    Vector2 tamanho_titulo = MeasureTextEx(fonte_textos, "EEEEEEEEEEEEEEEE", tamanho_fonte, 2.0f);
    float retangulo_nome_base = tamanho_titulo.x + tamanho_titulo.x * 0.05f;
    float retangulo_nome_altura = tamanho_titulo.y + tamanho_titulo.y;
    float retangulo_nome_x = JANELA_LARGURA / 2 - retangulo_nome_base / 2;
    float retangulo_nome_y = JANELA_ALTURA / 2 - retangulo_nome_altura / 2;

    Rectangle retangulo_nome = {retangulo_nome_x, retangulo_nome_y, retangulo_nome_base, retangulo_nome_altura};

    DrawRectangleRec(retangulo_nome, RAYWHITE);
    DrawRectangleLinesEx(retangulo_nome, 2.0, GRAY);

    // ===== DESENHAR TEXTO DIGITADO =====
    Vector2 tamanho_titulo_nome = MeasureTextEx(fonte_textos, nomes, tamanho_fonte, 2.0f);
    float pos_escrita_nome_x = retangulo_nome.x + (retangulo_nome_base - tamanho_titulo_nome.x) / 2.0f;
    float pos_escrita_nome_y = retangulo_nome.y + (retangulo_nome_altura - tamanho_titulo_nome.y) / 2.0f;
    Vector2 pos_escrita_nome = {pos_escrita_nome_x, pos_escrita_nome_y};

    DrawTextEx(fonte_textos, nomes, pos_escrita_nome, tamanho_fonte, 4.0f, BLACK);

    // ===== DESENHAR LABEL =====
    float tamanho_fonte_label = tamanho_fonte;
    Vector2 tamanho_label = MeasureTextEx(fonte_textos, "DIGITE O SEU NOME", tamanho_fonte_label, 2.0f);
    float pos_label_x = JANELA_LARGURA / 2.0f - tamanho_label.x / 2.0f;
    float pos_label_y = (JANELA_ALTURA / 2.0f) - retangulo_nome_altura;
    Vector2 pos_label = {pos_label_x, pos_label_y};

    DrawTextWithOutline(fonte_textos, "DIGITE O SEU NOME", pos_label, tamanho_fonte_label, 2.0f, RED, GRAY, 1.0f);

    // ===== LÓGICA DE CONFIRMAÇÃO =====
    if(IsKeyPressed(KEY_ENTER) && jogo->tela_atual == TELA_DIGITANDO_NOME && !jogo->enter_processado_neste_frame){
        placar_inserir(jogo->placar, MAX_PLACAR, nomes, jogo->tempos_telas.segundos_ate_fim_partida);
        placar_salvar(CAMINHO_ARQUIVO_PLACAR, jogo->placar, MAX_PLACAR);

        // Reseta as variáveis de nome para a próxima partida
        indice_nome = 0;
        nomes[0] = '\0';

        jogo->tela_atual = TELA_MENU_PRINCIPAL;
        jogo->enter_processado_neste_frame = true;
    }
}
