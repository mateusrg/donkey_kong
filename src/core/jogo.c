#include "jogo.h"

#include <string.h>

#include "../gameplay/mundo.h"
#include "../io/mapa_io.h"
#include "constants.h"

void jogo_reiniciar_partida(Jogo* jogo)
{
    if (!jogo)
    {
        return;
    }

    jogo->fase_atual = 0;
    jogo->jogador.vidas = VIDAS_INICIAIS;
    jogo->bonus_tempo_segundos = 0;
    jogo->nome_jogador[0] = '\0';
    jogo->tempos_telas.segundos_ate_jogar = 0.0f;
    jogo->tempos_telas.segundos_ate_fim_partida = 0.0f;
    jogo->tempos_telas.segundos_ate_pausar = 0.0f;
    jogo->opcao_pausa = -1;
    jogo->ranking_atualizado = false;

    mundo_carregar_fase(jogo);
}
