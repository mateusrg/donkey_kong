#include "raylib.h"

#include <stdio.h>

#include <string.h>

#include "core/constants.h"

#include "core/models.h"

#include "io/mapa_io.h"

#include "io/placar_io.h"

#include "presentation/audio.h"

#include "presentation/animacao.h"

#include "presentation/render.h" // Certifique-se de que o caminho está correto



int main(void) {

    // 1. Inicialização de dados

    char arquivoMapa[100];

    Jogo meuJogo = { 0 }; // Zera toda a struct

    meuJogo.tela_atual = TELA_MENU_PRINCIPAL;



    for(int i = 0; i < MAX_PLACAR; i++){

        meuJogo.placar[i].time = LIMITE_SEGUNDOS;

    }



    // 2. Monta o caminho e tenta carregar o mapa



    if (!mapa_montar_caminho_fase(3, arquivoMapa)) {

        printf("ERRO: Nome da fase invalido.\n");

        return 1;

    }







    // Passamos os endereços do que está DENTRO da meuJogo

    if (!mapa_carregar(arquivoMapa, &meuJogo)) {

        printf("ERRO CRITICO: Nao foi possivel carregar o arquivo: %s\n", arquivoMapa);

        return 1;

    }



    /*

    if(!placar_salvar(CAMINHO_ARQUIVO_PLACAR, meuJogo.placar, MAX_PLACAR)){

        printf("Erro ao gerar novo arquivo");

        return 1;

    }

    */







    if (!placar_carregar(CAMINHO_ARQUIVO_PLACAR, meuJogo.placar, MAX_PLACAR)){

        printf("Arquivo de placar não encontrado. Criando novo placar...\n");

        // Se não conseguir carregar, salva um placar novo com os valores inicializados (999)

        placar_salvar(CAMINHO_ARQUIVO_PLACAR, meuJogo.placar, MAX_PLACAR);

        return 1;

    }



    // 3. Inicializa a Janela

    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Teste de Carregamento de Mapa");

    SetTargetFPS(60);



    // 4. Inicializa as texturas (chama aquela sua função que redimensiona)

    render_inicializar(&meuJogo);

    // 5. Inicializa os audios do jogo

    inicializar_audio();



    // Loop de teste

    while (!WindowShouldClose() && meuJogo.tela_atual != TELA_SAIR) {

        meuJogo.enter_processado_neste_frame = false;



        //Atualizações



        if(meuJogo.tela_atual == TELA_JOGANDO){

            atualizar_audio_musica();

        }



            atualiza_entidades(&meuJogo);



            if(IsKeyPressed(KEY_ENTER) && meuJogo.tela_atual == TELA_JOGANDO && !meuJogo.enter_processado_neste_frame){

                int tempoJogador = (int)GetTime() - meuJogo.tempos_telas.segundos_ate_jogar;

                meuJogo.tempos_telas.segundos_ate_fim_partida = tempoJogador;

                if(placar_elegivel(meuJogo.placar, MAX_PLACAR, tempoJogador)){



                    meuJogo.enter_processado_neste_frame = true;

                    meuJogo.tela_atual = TELA_DIGITANDO_NOME;

                }

                else{

                    meuJogo.tela_atual = TELA_MENU_PRINCIPAL;

                }

            }

       

        // --- DESENHO ---



            // Desenha os elementos gerais do jogo (Tela principal, animações dos personagens, tela de pausa, etc)

            render_desenhar(&meuJogo);



         



    }



    // 5. Finalização

    // --- Na finalização da sua main.c ---

    WaitTime(0.8f);

    printf("Tentando encerrar render...\n");

    render_encerrar(); // <--- Comente essa linha se o crash for aqui

    printf("Render encerrado com sucesso!\n");



    printf("Tentando encerrar audio...\n");

    encerrar_audio(); // <--- Comente essa linha se o crash for aqui

    printf("Audio encerrado com sucesso!\n");



    CloseWindow();

    return 0;

}