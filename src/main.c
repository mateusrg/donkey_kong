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

int main(void) {

    Jogo meuJogo = {0};

    meuJogo.tela_atual = TELA_MENU_PRINCIPAL;
    meuJogo.fase_atual = 0; // a fase equivale ao nome do arquivo nos mapaX.txt, mas na tela ele aparece sempre com um a mais
    meuJogo.opcao_pausa = OPCAO_PAUSA_NENHUMA;

    for (int i = 0; i < MAX_PLACAR; i++) {
        meuJogo.placar[i].time = LIMITE_SEGUNDOS;
    }

    if (!mundo_carregar_fase(&meuJogo)) {
        return 1; // dá erro
    }

    render_reinicializar_animacoes_inimigos(&meuJogo);

    if (!placar_carregar(CAMINHO_ARQUIVO_PLACAR, meuJogo.placar, MAX_PLACAR)) {
        // se não consegue carregar, salva um placar novo com 999, que é um valor arbitrariamente alto pra indicar que o placar tá vazio
        if (!placar_salvar(CAMINHO_ARQUIVO_PLACAR, meuJogo.placar, MAX_PLACAR)) {
            return 1;
        }
    }

    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Mario vs Donkey Kong");
    SetExitKey(KEY_NULL); // pra fazer o esc não fechar a janela, tem que tirar pq é o comportamento padrão

    SetTargetFPS(60);

    render_inicializar(&meuJogo);

    inicializar_audio();

    while (!WindowShouldClose() && meuJogo.tela_atual != TELA_SAIR) {
        ComandosJogador comandos = {0};
        meuJogo.enter_processado_neste_frame = false;
        comandos.acao_pulo = false;

        if (meuJogo.tela_atual == TELA_JOGANDO) {
            int tempo_jogador;

            atualizar_audio_musica();

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) comandos.horizontal = DIRECAO_DIREITA;
            else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) comandos.horizontal = DIRECAO_ESQUERDA;
            if (jogador_esta_sobre_plataforma(&meuJogo.jogador, &meuJogo.mapa) && (IsKeyDown(KEY_RIGHT)
            || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
                tocar_audio_efeito("passos");
            }

            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) comandos.vertical = DIRECAO_CIMA;
            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) comandos.vertical = DIRECAO_BAIXO;

            if (IsKeyPressed(KEY_SPACE) && jogador_esta_sobre_plataforma(&meuJogo.jogador, &meuJogo.mapa)) {
                comandos.acao_pulo = true;
                tocar_audio_efeito("pulo");
            }

            if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ESCAPE)) {
                meuJogo.tela_atual = TELA_PAUSADO;
            }

            mundo_atualizar(&meuJogo, comandos, GetFrameTime());

            if (mundo_jogador_alcancou_saida(&meuJogo)) {
                // avança pra próxima fase
                meuJogo.fase_atual++;
                meuJogo.jogador.alcancou_porta = false;

                // carrega a próxima fase
                if (mundo_carregar_fase(&meuJogo)) {
                    render_reinicializar_animacoes_inimigos(&meuJogo);
                    meuJogo.tela_atual = TELA_JOGANDO;
                } else {
                    // não tem mais nenhuma fase, acaba o jogo
                    tempo_jogador = (int)GetTime() - (int)meuJogo.tempos_telas.segundos_ate_jogar - meuJogo.bonus_tempo_segundos;
                    if (tempo_jogador < 0) tempo_jogador = 0;
                    meuJogo.tempos_telas.segundos_ate_fim_partida = (float)tempo_jogador;

                    // reseta pra fase 0 e reseta as vidas
                    meuJogo.fase_atual = 0;
                    mundo_carregar_fase(&meuJogo);
                    render_reinicializar_animacoes_inimigos(&meuJogo);
                    meuJogo.jogador.vidas = VIDAS_INICIAIS;
                    meuJogo.bonus_tempo_segundos = 0;

                    meuJogo.enter_processado_neste_frame = true;
                    meuJogo.tela_atual = TELA_VITORIA;
                }
            } else if (!meuJogo.jogador.ativo) {
                // game over calcula o tempo e exibe a tela de game over
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

        if (meuJogo.tela_atual == TELA_GAME_OVER) {
            atualizar_audio_musica();
            if (!meuJogo.enter_processado_neste_frame &&
                (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
                meuJogo.tela_atual = TELA_MENU_PRINCIPAL;
            }
        }

        if (meuJogo.tela_atual == TELA_VITORIA) {
            atualizar_audio_musica();
            if (!meuJogo.enter_processado_neste_frame &&
                (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
                if (placar_elegivel(meuJogo.placar, MAX_PLACAR, (int)meuJogo.tempos_telas.segundos_ate_fim_partida)) {
                    meuJogo.enter_processado_neste_frame = true;
                    meuJogo.tela_atual = TELA_DIGITANDO_NOME;
                } else {
                    meuJogo.tela_atual = TELA_MENU_PRINCIPAL;
                }
            }
        }

        if (meuJogo.tela_atual == TELA_PAUSADO) {
            atualiza_menu_pausa(&meuJogo);
        }

        if (meuJogo.tela_atual != TELA_PAUSADO) {
            atualiza_anim_entidades(&meuJogo);
        }

        render_desenhar(&meuJogo);
    }

    WaitTime(0.8f); // aguarda alguns décimos antes de fechar o jogo de fato pra tocar o sonzinho
    render_encerrar();
    encerrar_audio();
    CloseWindow();

    return 0;
}
