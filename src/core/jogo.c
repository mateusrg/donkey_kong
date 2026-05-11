#include "jogo.h"

#include <string.h>

void jogo_inicializar(Jogo* jogo)
{
    if (jogo)
    {
        memset(jogo, 0, sizeof(Jogo));
        jogo->tela_atual = TELA_MENU_PRINCIPAL;
        jogo->fase_atual = 1;
    }
}

void jogo_atualizar(Jogo* jogo, float delta_tempo)
{
    (void)jogo;
    (void)delta_tempo;
}

void jogo_reiniciar_partida(Jogo* jogo)
{
    (void)jogo;
}

void jogo_reiniciar_fase_atual(Jogo* jogo)
{
    (void)jogo;
}

void jogo_encerrar(Jogo* jogo)
{
    (void)jogo;
}
