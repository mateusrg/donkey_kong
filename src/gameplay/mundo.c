#include "mundo.h"

bool mundo_carregar_fase(Jogo* jogo)
{
    (void)jogo;
    return false;
}

void mundo_atualizar(Jogo* jogo, ComandosJogador comandos, float delta_tempo)
{
    (void)jogo;
    (void)comandos;
    (void)delta_tempo;
}

bool mundo_jogador_alcancou_saida(const Jogo* jogo)
{
    (void)jogo;
    return false;
}

bool mundo_jogador_colidiu_inimigo(const Jogo* jogo)
{
    (void)jogo;
    return false;
}
