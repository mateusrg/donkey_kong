#include "mundo.h"
#include "jogador.h"
#include "inimigo.h"
#include "../io/mapa_io.h"
#include "../core/constants.h"

bool mundo_carregar_fase(Jogo* jogo) {
    char caminho[MAX_TEXTO_CAMINHO];
    int i;

    if (!mapa_montar_caminho_fase(jogo->fase_atual, caminho)) {
        return false;
    }

    if (!mapa_carregar(caminho, jogo)) {
        return false;
    }

    // preserva as vidas entre fases, porque jogador_inicializar reseta tudo para o padrão
    {
        int vidas_atuais = jogo->jogador.vidas;
        jogador_inicializar(&jogo->jogador, jogo->jogador.spawn_inicial);
        if (vidas_atuais > 0) {
            jogo->jogador.vidas = vidas_atuais;
        }
    }

    for (i = 0; i < jogo->quantidade_inimigos; i++) {
        // tipo já foi definido pelo mapa_io; lê antes do memset dentro de inicializar
        inimigo_inicializar(&jogo->inimigos[i], jogo->inimigos[i].spawn_inicial, jogo->inimigos[i].tipo);
    }

    return true;
}

void mundo_atualizar(Jogo* jogo, ComandosJogador comandos, float delta_tempo) {
    int i;

    if (!jogo->jogador.ativo) {
        return;
    }

    jogador_atualizar(&jogo->jogador, &jogo->mapa, comandos, delta_tempo);

    // desconta timer de invencibilidade
    if (jogo->jogador.invencivel) {
        jogo->jogador.tempo_invencibilidade -= delta_tempo;
        if (jogo->jogador.tempo_invencibilidade <= 0.0f) {
            jogo->jogador.invencivel = false;
            jogo->jogador.tempo_invencibilidade = 0.0f;
        }
    }

    for (i = 0; i < jogo->quantidade_inimigos; i++) {
        if (jogo->inimigos[i].ativo) {
            inimigo_atualizar(&jogo->inimigos[i], &jogo->mapa, delta_tempo);
        }
    }

    // coleta de power-up: AABB entre jogador e cada power-up ativo
    for (i = 0; i < jogo->quantidade_powerups; i++) {
        if (!jogo->powerups[i].ativo) {
            continue;
        }

        if (jogo->jogador.posicao_pixels.x < jogo->powerups[i].posicao_pixels.x + (float)TILE_SIZE &&
            jogo->jogador.posicao_pixels.x + (float)TILE_SIZE > jogo->powerups[i].posicao_pixels.x &&
            jogo->jogador.posicao_pixels.y < jogo->powerups[i].posicao_pixels.y + (float)TILE_SIZE &&
            jogo->jogador.posicao_pixels.y + (float)TILE_SIZE > jogo->powerups[i].posicao_pixels.y) {
            jogo->powerups[i].ativo = false;

            if (jogo->powerups[i].tipo == POWERUP_VIDA) {
                jogo->jogador.vidas++;
            } else if (jogo->powerups[i].tipo == POWERUP_INVENCIVEL) {
                jogo->jogador.invencivel = true;
                jogo->jogador.tempo_invencibilidade = DURACAO_INVENCIBILIDADE;
            } else {
                jogo->bonus_tempo_segundos += REDUCAO_TEMPO_POWERUP;
            }
        }
    }

    // vitória: jogador pisou no tile da porta ou encostou na princesa
    if (jogo->mapa.tiles[jogo->jogador.tile.linha][jogo->jogador.tile.coluna] == TILE_PORTA) {
        jogo->jogador.alcancou_porta = true;
    }

    if (jogo->princesa.ativo &&
        jogo->jogador.posicao_pixels.x < jogo->princesa.posicao_pixels.x + (float)TILE_SIZE &&
        jogo->jogador.posicao_pixels.x + (float)TILE_SIZE > jogo->princesa.posicao_pixels.x &&
        jogo->jogador.posicao_pixels.y < jogo->princesa.posicao_pixels.y + (float)TILE_SIZE &&
        jogo->jogador.posicao_pixels.y + (float)TILE_SIZE > jogo->princesa.posicao_pixels.y) {
        jogo->jogador.alcancou_porta = true;
    }

    // derrota: colidir com o Donkey Kong mata (invencibilidade não protege porque é o chefão)
    if (jogo->donkey.ativo &&
        jogo->jogador.posicao_pixels.x < jogo->donkey.posicao_pixels.x + (float)TILE_SIZE &&
        jogo->jogador.posicao_pixels.x + (float)TILE_SIZE > jogo->donkey.posicao_pixels.x &&
        jogo->jogador.posicao_pixels.y < jogo->donkey.posicao_pixels.y + (float)TILE_SIZE &&
        jogo->jogador.posicao_pixels.y + (float)TILE_SIZE > jogo->donkey.posicao_pixels.y) {
        jogo->jogador.vidas--;

        if (jogo->jogador.vidas <= 0) {
            jogo->jogador.ativo = false;
        } else {
            jogador_resetar(&jogo->jogador, jogo->jogador.spawn_inicial);
            for (i = 0; i < jogo->quantidade_inimigos; i++) {
                inimigo_resetar(&jogo->inimigos[i], jogo->inimigos[i].spawn_inicial);
            }
        }
    }

    // derrota: colisão com qualquer inimigo ativo (invencibilidade protege)
    if (!jogo->jogador.invencivel && mundo_jogador_colidiu_inimigo(jogo)) {
        jogo->jogador.vidas--;

        if (jogo->jogador.vidas <= 0) {
            jogo->jogador.ativo = false;
        } else {
            // reposiciona o jogador e reseta os inimigos para evitar toque imediato
            jogador_resetar(&jogo->jogador, jogo->jogador.spawn_inicial);
            for (i = 0; i < jogo->quantidade_inimigos; i++) {
                inimigo_resetar(&jogo->inimigos[i], jogo->inimigos[i].spawn_inicial);
            }
        }
    }
}

bool mundo_jogador_alcancou_saida(const Jogo* jogo) {
    return jogo->jogador.alcancou_porta;
}

bool mundo_jogador_colidiu_inimigo(const Jogo* jogo) {
    int i;

    for (i = 0; i < jogo->quantidade_inimigos; i++) {
        if (jogo->inimigos[i].ativo && inimigo_colide_com_jogador(&jogo->inimigos[i], &jogo->jogador)) {
            return true;
        }
    }

    return false;
}
