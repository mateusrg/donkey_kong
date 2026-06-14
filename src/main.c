#include "raylib.h"

#include <stdio.h>

#include <string.h>

#include "core/constants.h"

#include "core/models.h"

#include "io/mapa_io.h"

#include "io/placar_io.h"

#include "presentation/audio.h"

#include "presentation/animacao.h"

#include "presentation/render.h"

#include "presentation/menu.h"

#include "gameplay/mundo.h"

#include "gameplay/jogador.h"

#include "gameplay/inimigo.h"

int main(void)
{

    // 1. Inicialização de dados

    Jogo meuJogo = {0};
    

    meuJogo.tela_atual = TELA_MENU_PRINCIPAL;
    meuJogo.fase_atual = 0;
    meuJogo.opcao_pausa = -1;

    for (int i = 0; i < MAX_PLACAR; i++)
    {

        meuJogo.placar[i].time = LIMITE_SEGUNDOS;
    }

    // 2. Carrega a fase inicial usando mundo_carregar_fase

    if (!mundo_carregar_fase(&meuJogo))
    {

        printf("ERRO CRITICO: Nao foi possivel carregar o mapa da fase %d.\n", meuJogo.fase_atual);

        return 1;
    }

    render_reinicializar_animacoes_inimigos(&meuJogo);

    
    // if(!placar_salvar(CAMINHO_ARQUIVO_PLACAR, meuJogo.placar, MAX_PLACAR)){

    //     printf("Erro ao gerar novo arquivo");

    //     return 1;

    // }


    if (!placar_carregar(CAMINHO_ARQUIVO_PLACAR, meuJogo.placar, MAX_PLACAR))
    {

        printf("Arquivo de placar não encontrado. Criando novo placar...\n");

        // Se não conseguir carregar, salva um placar novo com os valores inicializados (999)

        if (!placar_salvar(CAMINHO_ARQUIVO_PLACAR, meuJogo.placar, MAX_PLACAR))

        {

            printf("Erro ao criar o arquivo de placar.\n");

            return 1;

        }
    }

    // 3. Inicializa a Janela

    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Mario vs Donkey Kong");
    SetExitKey(KEY_NULL); // ESC não fecha a janela

    SetTargetFPS(60);

    // 4. Inicializa as texturas

    render_inicializar(&meuJogo);

    // 5. Inicializa os audios do jogo

    inicializar_audio();

    // Loop de teste

    while (!WindowShouldClose() && meuJogo.tela_atual != TELA_SAIR)
    {
        ComandosJogador comandos = {0};
        meuJogo.enter_processado_neste_frame = false;
        comandos.acao_pulo = false;

        // Atualizações

        if (meuJogo.tela_atual == TELA_JOGANDO)
        {
            int tempo_jogador;

            atualizar_audio_musica();

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) comandos.horizontal = 1;
            else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) comandos.horizontal = -1;
            if(jogador_esta_sobre_plataforma(&meuJogo.jogador, &meuJogo.mapa) && (IsKeyDown(KEY_RIGHT)
            || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_A))){
                tocar_audio_efeito("passos");
            }
        
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) comandos.vertical = -1;
            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) comandos.vertical = 1;

            if (IsKeyPressed(KEY_SPACE) && jogador_esta_sobre_plataforma(&meuJogo.jogador, &meuJogo.mapa)){ comandos.acao_pulo = true; tocar_audio_efeito("pulo");}

            if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ESCAPE))
            {
                meuJogo.tela_atual = TELA_PAUSADO;
            }

            mundo_atualizar(&meuJogo, comandos, GetFrameTime());

            if (mundo_jogador_alcancou_saida(&meuJogo))
            {
                // Avança para a próxima fase
                meuJogo.fase_atual++;
                meuJogo.jogador.alcancou_porta = false;

                if (mundo_carregar_fase(&meuJogo))
                {
                    // Passa direto para o jogo sem tela intermediária.
                    // Não reseta segundos_ate_jogar: o tempo acumula entre fases.
                    render_reinicializar_animacoes_inimigos(&meuJogo);
                    meuJogo.tela_atual = TELA_JOGANDO;
                }
                else
                {
                    // Sem mais fases - fim de jogo completo
                    tempo_jogador = (int)GetTime() - (int)meuJogo.tempos_telas.segundos_ate_jogar - meuJogo.bonus_tempo_segundos;
                    if (tempo_jogador < 0) tempo_jogador = 0;
                    meuJogo.tempos_telas.segundos_ate_fim_partida = (float)tempo_jogador;

                    // Reseta para fase 0 e vidas antes de qualquer transição
                    meuJogo.fase_atual = 0;
                    mundo_carregar_fase(&meuJogo);
                    render_reinicializar_animacoes_inimigos(&meuJogo);
                    meuJogo.jogador.vidas = VIDAS_INICIAIS;
                    meuJogo.bonus_tempo_segundos = 0;

                    meuJogo.enter_processado_neste_frame = true;
                    meuJogo.tela_atual = TELA_VITORIA;
                }
            }
            else if (!meuJogo.jogador.ativo)
            {
                // Game over: calcula tempo, exibe tela de game over e depois placar
                tempo_jogador = (int)GetTime() - (int)meuJogo.tempos_telas.segundos_ate_jogar - meuJogo.bonus_tempo_segundos;
                if (tempo_jogador < 0) tempo_jogador = 0;
                meuJogo.tempos_telas.segundos_ate_fim_partida = (float)tempo_jogador;

                meuJogo.fase_atual = 0;
                mundo_carregar_fase(&meuJogo);
                render_reinicializar_animacoes_inimigos(&meuJogo);
                meuJogo.jogador.vidas = VIDAS_INICIAIS;
                meuJogo.bonus_tempo_segundos = 0;

                meuJogo.enter_processado_neste_frame = true;
                meuJogo.tela_atual = TELA_GAME_OVER;
            }
        }

        if (meuJogo.tela_atual == TELA_GAME_OVER)
        {
            atualizar_audio_musica();
            if (!meuJogo.enter_processado_neste_frame &&
                (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)))
            {
                meuJogo.tela_atual = TELA_MENU_PRINCIPAL;
            }
        }

        if (meuJogo.tela_atual == TELA_VITORIA)
        {
            atualizar_audio_musica();
            if (!meuJogo.enter_processado_neste_frame &&
                (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)))
            {
                if (placar_elegivel(meuJogo.placar, MAX_PLACAR, (int)meuJogo.tempos_telas.segundos_ate_fim_partida))
                {
                    meuJogo.enter_processado_neste_frame = true;
                    meuJogo.tela_atual = TELA_DIGITANDO_NOME;
                }
                else
                {
                    meuJogo.tela_atual = TELA_MENU_PRINCIPAL;
                }
            }
        }

        if (meuJogo.tela_atual == TELA_PAUSADO)
        {
            atualiza_menu_pausa(&meuJogo);
        }

        if (meuJogo.tela_atual != TELA_PAUSADO)
        {
            atualiza_anim_entidades(&meuJogo);
        }


        // --- DESENHO ---

        // Desenha os elementos gerais do jogo (Tela principal, animações dos personagens, tela de pausa, etc)

        render_desenhar(&meuJogo);
    }

    // 5. Finalização

    WaitTime(0.8f);

    printf("Tentando encerrar render...\n");

    render_encerrar();

    printf("Render encerrado com sucesso!\n");

    printf("Tentando encerrar audio...\n");

    encerrar_audio();

    printf("Audio encerrado com sucesso!\n");

    CloseWindow();

    return 0;
}