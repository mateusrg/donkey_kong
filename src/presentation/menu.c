#include "raylib.h"
#include "menu.h"
#include <string.h>
#include "../io/placar_io.h"
#define QUANTIA_BOTOES 4

static int indiceTecladoInicial = 0;
static bool ja_tocou = false;
static int indInicial = 0;
static char nomes[TAM_MAX_NOME + 1];


void DrawTextWithOutline(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color textColor, Color outlineColor, float outlineThickness){
    // Desenha o contorno nas 4 direções cardinais (Esquerda, Direita, Cima, Baixo)
    DrawTextEx(font, text, (Vector2){ position.x - outlineThickness, position.y }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x + outlineThickness, position.y }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x, position.y - outlineThickness }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x, position.y + outlineThickness }, fontSize, spacing, outlineColor);
    
    // Se quiser um contorno ainda mais grosso e robusto, descomente as diagonais abaixo:
    /*
    DrawTextEx(font, text, (Vector2){ position.x - outlineThickness, position.y - outlineThickness }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x + outlineThickness, position.y - outlineThickness }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x - outlineThickness, position.y + outlineThickness }, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, (Vector2){ position.x + outlineThickness, position.y + outlineThickness }, fontSize, spacing, outlineColor);
    */

    // Por fim, desenha o texto principal por cima de tudo
    DrawTextEx(font, text, position, fontSize, spacing, textColor);
}

bool tocar_sons_botao(Rectangle retângulo, Vector2 posicaoTeclado, Vector2 posicaoMouse, int numBotao){
    static int botaoQueJaTocou = -1;
    
    //Sons do clique/enter
    if(numBotao == 1){
        if (CheckCollisionPointRec(posicaoMouse, retângulo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            
            tocar_audio_efeito("inicio");
            return true;
        }
    

        if(CheckCollisionPointRec(posicaoTeclado, retângulo) && IsKeyPressed(KEY_ENTER)){
            tocar_audio_efeito("inicio");
            return true;
        }
    }

    else if(numBotao == 2){
        if (CheckCollisionPointRec(posicaoMouse, retângulo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            tocar_audio_efeito("ver_placar");
            return true;
        }

        if(CheckCollisionPointRec(posicaoTeclado, retângulo) && IsKeyPressed(KEY_ENTER)){
            tocar_audio_efeito("ver_placar");
            return true;
        }
    }
    else if(numBotao == 3){
        if (CheckCollisionPointRec(posicaoMouse, retângulo) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            tocar_audio_efeito("botao_sair");
            return true;
        }

        if(CheckCollisionPointRec(posicaoTeclado, retângulo) && IsKeyPressed(KEY_ENTER)){
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

   
    if(CheckCollisionPointRec(posicaoMouse, retângulo)){
        int botaoTocadoAtual = numBotao; // Identifica qual botão o mouse está colidindo agora
        
        // A TRAVA: Só toca se o botão atual for diferente do que guardamos na memória
    if(botaoTocadoAtual != botaoQueJaTocou){
            tocar_audio_efeito("troca_opcao");
            botaoQueJaTocou = botaoTocadoAtual; // Atualiza a memória para dizer: "Esse botão já tocou!"
        }
    } 
    else {
        // Se a função foi chamada para este botão e o mouse NÃO está colidindo com ele...
        // E a memória achava que este era o último botão tocado, precisamos "limpar" a memória.
        if (botaoQueJaTocou == numBotao) {
            botaoQueJaTocou = -1; // Libera o botão para poder tocar de novo se o mouse voltar
        }
    }

    return false;   
    
}
void determina_posicoes_inputs(Vector2 *posicoesTeclado, int *indice){

    if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
        switch (*indice)
        {
        case 0:
            *indice = 1;
            break;
        case 1:
            *indice = 2;
            break;
        case 2:
            *indice = 3;
            break;
        case 3:
            *indice = 1;
            break;
        }
    }

    else if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
        switch (*indice)
        {
        case 0:
            *indice = 1;
            break;
        case 1:
            *indice = 3;
            break;
        case 2:
            *indice = 1;
            break;
        case 3:
            *indice = 2;
            break;
        }
    }

}


void desenha_menu_principal(Jogo *jogo, Font fonteTitulo, Font fonteBotoes, Texture2D mario, Texture2D princesa, Texture2D donkey){
    // ----------------------- TOCA PRIMEIRO SOM DO JOGO ---------------------------

    static bool ja_tocou = false;
    if(!ja_tocou){
        tocar_audio_efeito("abre_jogo");
        ja_tocou = true;
    }

    float x_centro = (float)JANELA_LARGURA / 2.0f;
    float y_centro = (float)JANELA_ALTURA / 2.0f;
    // ----------------------- DESENHA O TITULO ------------------------------------
    float tamanho_fonte_titulo = 40.0f;

    Vector2 posicaoTexto = {x_centro - 300.0f, (float) JANELA_ALTURA *  0.1f};
    Vector2 posicaoSombra = { posicaoTexto.x + 8.0f, posicaoTexto.y + 8.0f };

    // ----------------------- DEFINE TAMANHO DOS RETANGULOS ----------------------------------
    float retangulo_base = 190.0f;
    float retangulo_altura = 80.0f;
    float x_centro_retangulo = x_centro - (retangulo_base/2.0f);
    float y_centro_retangulo = ((float) JANELA_ALTURA / 2) - (retangulo_altura/2.0f);

    Rectangle retanguloJogar = {x_centro_retangulo, y_centro_retangulo - (retangulo_altura * 1.5), retangulo_base,  retangulo_altura};
    Rectangle retanguloPlacar = {x_centro_retangulo, y_centro_retangulo, retangulo_base, retangulo_altura};
    Rectangle retanguloSair = {x_centro_retangulo, y_centro_retangulo + (retangulo_altura * 1.5), retangulo_base,  retangulo_altura};

    // ---------------------- DEFINE A POSIÇÃO DAS ANIMAÇÕES QUE SERÃO UTILIZADAS EM CIMA DOS BOTÕES --------------------------

    float x_bonecos = x_centro - ((float)TILE_SIZE / 2.0f);
    Vector2 posMario = {
        x_bonecos, retanguloJogar.y - (float) TILE_SIZE
    };

    Vector2 posPrincesa = {
        x_bonecos, retanguloPlacar.y - (float) TILE_SIZE
    };

    Vector2 posDonkey = {
        x_bonecos, retanguloSair.y - (float) TILE_SIZE
    };
    // ---------------------- DESENHA OS RETÂNGULOS --------------------------------------
    //Declara um vetor posições de um determinado ponto, que muda de acordo com os inputs do teclado

    Vector2 posicaoMouse = GetMousePosition(); // Verifica em que posição está o cursor do mouse (para verificar colisoes)
    
    Vector2 posicoesTeclado[QUANTIA_BOTOES] = {
        {0, 0}, {retanguloJogar.x, retanguloJogar.y}, {retanguloPlacar.x, retanguloPlacar.y}, {retanguloSair.x, retanguloSair.y}
    };

    determina_posicoes_inputs(posicoesTeclado, &indiceTecladoInicial);
    //Desenha os retângulos com bordas redondas
    //E verifica a colisão com o mouse (Se sim, ele fica vermelho, se não, verde).
    // Retangulo Jogar:
    if(!(CheckCollisionPointRec(posicaoMouse, retanguloJogar) || CheckCollisionPointRec(posicoesTeclado[indiceTecladoInicial], retanguloJogar))){
         DrawRectangleRounded(retanguloJogar, 0.2, 4, GREEN);
    }
    else{
        DrawRectangleRounded(retanguloJogar, 0.2, 4, RED);
        DrawTexturePro(
            mario, animation_frame(&jogo->jogador.animacao, FRAMES_POR_LINHA), (Rectangle){posMario.x, posMario.y, (float)TILE_SIZE, (float)TILE_SIZE}, 
            (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
      
    }
    if(tocar_sons_botao(retanguloJogar, posicoesTeclado[indiceTecladoInicial], posicaoMouse, 1)){
        jogo->tela_atual = TELA_JOGANDO;
    }
    // Retangulo Placar:
    if(!(CheckCollisionPointRec(posicaoMouse, retanguloPlacar) || CheckCollisionPointRec(posicoesTeclado[indiceTecladoInicial], retanguloPlacar))){
        DrawRectangleRounded(retanguloPlacar, 0.2, 4,  GREEN);
    }
    else{
        DrawRectangleRounded(retanguloPlacar, 0.2, 4, RED);
        DrawTexturePro(
            princesa, animation_frame(&jogo->princesa.animacao, FRAMES_POR_LINHA), (Rectangle){posPrincesa.x, posPrincesa.y, (float)TILE_SIZE, (float)TILE_SIZE}, 
            (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
      }
 
    if(tocar_sons_botao (retanguloPlacar, posicoesTeclado[indiceTecladoInicial], posicaoMouse, 2)){
        jogo->tela_atual = TELA_RANKING;
    }


    // Retangulo Sair:
    if(!(CheckCollisionPointRec(posicaoMouse, retanguloSair) || CheckCollisionPointRec(posicoesTeclado[indiceTecladoInicial], retanguloSair))){
        DrawRectangleRounded(retanguloSair, 0.2, 4, GREEN);
    }
    else{
     DrawRectangleRounded(retanguloSair, 0.2, 4, RED);
     DrawTexturePro(
            donkey, animation_frame(&jogo->donkey.animacao, FRAMES_POR_LINHA), (Rectangle){posDonkey.x, posDonkey.y, (float)TILE_SIZE, (float)TILE_SIZE}, 
            (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    }
    if(tocar_sons_botao(retanguloSair, posicoesTeclado[indiceTecladoInicial], posicaoMouse, 3)){
        jogo->tela_atual = TELA_SAIR;
    }

   
    
    //Desenha o traçado dos retangulos de bordas redondas
    DrawRectangleRoundedLines(retanguloJogar, 0.2, 4, BLACK);
    DrawRectangleRoundedLines(retanguloPlacar, 0.2, 4, BLACK);
    DrawRectangleRoundedLines(retanguloSair, 0.2, 4, BLACK);
    
    // --------------------- ESCREVE OS TEXTOS NOS RETANGULOS ------------------------------
    float tamanho_fonte_botao = 24.0f;

    //desenha texto jogar

    Vector2 textoJogar = MeasureTextEx(fonteBotoes, "JOGAR", tamanho_fonte_botao,  2.0);
    Vector2 posTextoJogar = {
        retanguloJogar.x + (retangulo_base -  textoJogar.x) / 2.0f,  //Cálculo para saber aonde o texto deve começar a ser escrito
        retanguloJogar.y +  (retangulo_altura - textoJogar.y) / 2.0f
    };

    DrawTextWithOutline(fonteBotoes, "JOGAR", posTextoJogar,  tamanho_fonte_botao, 2.0f, WHITE, BLACK, 2.0f);

    //desenha texto placar

    Vector2 textoPlacar = MeasureTextEx(fonteBotoes, "PLACAR", tamanho_fonte_botao,  2.0);
    Vector2 posTextoPlacar = {
        retanguloPlacar.x + (retangulo_base -  textoPlacar.x) / 2.0f,  //Cálculo para saber aonde o texto deve começar a ser escrito
        retanguloPlacar.y +  (retangulo_altura - textoPlacar.y) / 2.0f
    };

    DrawTextWithOutline(fonteBotoes, "PLACAR", posTextoPlacar,  tamanho_fonte_botao, 2.0f, WHITE, BLACK, 2.0f);

    //desenha texto sair

    Vector2 textoSair = MeasureTextEx(fonteBotoes, "SAIR", tamanho_fonte_botao,  2.0);
    Vector2 posTextoSair = {
        retanguloSair.x + (retangulo_base -  textoSair.x) / 2.0f,  //Cálculo para saber aonde o texto deve começar a ser escrito
        retanguloSair.y +  (retangulo_altura - textoSair.y) / 2.0f
    };

    DrawTextWithOutline(fonteBotoes, "SAIR", posTextoSair,  tamanho_fonte_botao, 2.0f, WHITE, BLACK, 2.0f);

    // ---------------------- ESCREVE O TITULO COM EFEITOS ----------------------------------
    //Com a sombra embaixo do texto
    DrawTextEx(fonteTitulo, "Mario vs. Donkey Kong", posicaoSombra, tamanho_fonte_titulo, 2.0f /*espaçamento*/ , BLACK);    
    //Escreve texto com traçado
    DrawTextWithOutline(fonteTitulo, "Mario vs. Donkey Kong", posicaoTexto, tamanho_fonte_titulo, 2.0f, RED, BLACK, 5.0f);

}

void desenha_tela_ranking(Jogo *jogo, Font fonteJogo, Font fonteTextos){
    Vector2 posMouse = GetMousePosition();
    // ------------------- ESCREVE TITULO COM E DECLARAÇÃO DAS POSIÇÕES DO MESMO ----------
    float x_centro = JANELA_LARGURA / 2.0f;
    float y_centro = JANELA_ALTURA / 2.0f;
    float tamanhoFonteTitulo = 70.0f;
    Vector2 tamanhoTitulo = MeasureTextEx(fonteJogo,"RANKING", tamanhoFonteTitulo, 2.0f);
    float x_centro_texto = x_centro - (tamanhoTitulo.x / 2.0f);
    float y_texto = JANELA_ALTURA -  (JANELA_ALTURA * 0.9f) - (tamanhoTitulo.y / 2);
    Vector2 posTituloRanking = {
        x_centro_texto, y_texto
    };

    DrawTextWithOutline(fonteJogo, "RANKING", posTituloRanking, tamanhoFonteTitulo, 2.0f, BLACK, WHITE, 2.0f);

    // ------------------- DESENHA TABELA RANKING ------------------------------------------

    // Declara retangulo maior
    float retanguloRankingBase = JANELA_LARGURA * 0.8f ;
    float retanguloRankingAltura = JANELA_ALTURA * 0.7f;
    float retanguloRankingPosX = x_centro - (retanguloRankingBase / 2);
    float retanguloRankingPosY = y_centro - (retanguloRankingAltura / 2);
    Rectangle retanguloRanking = {
        retanguloRankingPosX, retanguloRankingPosY, retanguloRankingBase, retanguloRankingAltura
    };

    // Declara retangulo menor
    float retanguloRankingMenorBase = retanguloRankingBase * 0.8;
    float retanguloRankingMenorAltura = retanguloRankingAltura * 0.8; 
    float retanguloRankingMenorPosX = retanguloRankingPosX + ((retanguloRankingBase - retanguloRankingMenorBase) / 2);
    float retanguloRankingMenorPosY = retanguloRankingPosY + ((retanguloRankingAltura - retanguloRankingMenorAltura) / 2);
    Rectangle retanguloRankingMenor = {
        retanguloRankingMenorPosX, retanguloRankingMenorPosY, retanguloRankingMenorBase, retanguloRankingMenorAltura
    };
     DrawRectangleRounded(retanguloRanking, 0.2, 3, RED);
     DrawRectangleRounded(retanguloRankingMenor, 0.2, 3, RAYWHITE);
     DrawRectangleRoundedLinesEx(retanguloRankingMenor, 0.2, 3, 5, BLACK);
     DrawRectangleRoundedLinesEx(retanguloRanking, 0.2, 3, 5, BLACK);
    
    // ------------------ DESENHA BOTAO DE SAIR ----------------------------------------

    float tamanhoFonteX = 30.0f;
    Vector2 tamanhoX = MeasureTextEx(fonteTextos, "<", tamanhoFonteX, 2.0f);
    
    float retanguloBase = tamanhoX.x * 1.5f;
    float retanguloAltura = tamanhoX.y * 1.5f;
    

    float retanguloX_pos = 20.0f;
    float retanguloY_pos = 20.0f;
    
    Rectangle retanguloX = {
        retanguloX_pos, 
        retanguloY_pos, 
        retanguloBase, 
        retanguloAltura
    };
    
    float posicaoXX = retanguloX.x + (retanguloX.width - tamanhoX.x) / 2.0f;
    float posicaoXY = retanguloX.y + (retanguloX.height - tamanhoX.y) / 2.0f;
    Vector2 posicaoX = {
        posicaoXX, posicaoXY
    };
    
    DrawRectangleRounded(retanguloX, 0.2f, 4, RED);
    DrawRectangleRoundedLines(retanguloX, 0.2f, 4, BLACK);
    DrawTextEx(fonteTextos, "<", posicaoX, tamanhoFonteX, 2.0f, BLACK);

    if(CheckCollisionPointRec(posMouse, retanguloX)){
        DrawRectangleRounded(retanguloX, 0.2f, 4, GRAY);
        DrawRectangleRoundedLines(retanguloX, 0.2f, 4, BLACK);
        DrawTextEx(fonteTextos, "<", posicaoX, tamanhoFonteX, 2.0f, BLACK);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            jogo -> tela_atual = TELA_MENU_PRINCIPAL;
        }
    }
}

void desenha_menu_nome(Jogo *jogo, Font fonteTextos){
    float tamanhoFonte = 30.0f;
    char letraAtual = GetCharPressed();
    while(letraAtual > 0) {
        if(indInicial < TAM_MAX_NOME){
            nomes[indInicial] = letraAtual;
            indInicial++;
            nomes[indInicial] = '\0';
        }
        letraAtual = 0;
    }

    if(IsKeyPressed(KEY_BACKSPACE)){
        indInicial--;
        nomes[indInicial] = '\0';
    }

    // ----------------------- DESENHA O RETANGULO  -------------------------------
    Vector2 tamanhoTexto = MeasureTextEx(fonteTextos, "EEEEEEEEEEEEEEEE", tamanhoFonte, 2.0f);

    float retanguloNomeBase = tamanhoTexto.x + tamanhoTexto.x * 0.05f;
    float retanguloNomeAltura = tamanhoTexto.y + tamanhoTexto.y;
    float retanguloNomeX = JANELA_LARGURA / 2  - retanguloNomeBase / 2;
    float retanguloNomeY = JANELA_ALTURA / 2 - retanguloNomeAltura / 2;

    Rectangle retanguloNome = {
        retanguloNomeX,  retanguloNomeY, retanguloNomeBase, retanguloNomeAltura
    };

    DrawRectangleRec(retanguloNome, RAYWHITE);
    DrawRectangleLinesEx(retanguloNome, 2.0, GRAY);

    // ----------------------- DESENHA O TEXTO DIGITADO ---------------------------
    Vector2 tamanhoTextoNome = MeasureTextEx(fonteTextos, nomes, tamanhoFonte, 2.0f);
    float posEscritaNomeX = retanguloNome.x + (retanguloNomeBase - tamanhoTextoNome.x) / 2.0f;
    float posEscritaNomeY = retanguloNome.y + (retanguloNomeAltura - tamanhoTextoNome.y) / 2.0f;
    Vector2 posEscritaNome = {
        posEscritaNomeX, posEscritaNomeY
    };
    DrawTextEx(fonteTextos, nomes, posEscritaNome, tamanhoFonte, 4.0f, BLACK);

    // ---------------------- DESENHA TEXTO EM CIMA DO RETANGULO ------------------
    float tamanhoFonteTextoCima = tamanhoFonte;
    Vector2 tamanhoTextoCima = MeasureTextEx(fonteTextos, "DIGITE O SEU NOME", tamanhoFonteTextoCima, 2.0f);
    float posTextoX = JANELA_LARGURA / 2.0f - tamanhoTextoCima.x / 2.0f;
    float posTextoY = (JANELA_ALTURA / 2.0f) - retanguloNomeAltura;
    Vector2 posTexto = {
        posTextoX, posTextoY
    };

    DrawTextWithOutline(fonteTextos,  "DIGITE O SEU NOME", posTexto, tamanhoFonteTextoCima, 2.0f, RED, GRAY, 1.0f);

    if(IsKeyPressed(KEY_ENTER)){
        
        placar_inserir(jogo->placar, MAX_PLACAR, nomes, jogo->placar->time);

        jogo->tela_atual = TELA_MENU_PRINCIPAL;
    }

}