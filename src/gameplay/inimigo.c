#include "inimigo.h"

#include <string.h>

Vetor2D inimigo_tile_para_pixels(PosicaoMapa tile) {
    Vetor2D posicao_pixels;

    posicao_pixels.x = (float)(tile.coluna * TILE_SIZE);
    posicao_pixels.y = (float)(tile.linha * TILE_SIZE);

    return posicao_pixels;
}

void inimigo_inicializar(Inimigo* inimigo, PosicaoMapa spawn_inicial, TipoInimigo tipo) {
    if (inimigo) {
        // preserva os campos de animação inicializados no render_inicializar
        inimigo->spawn_inicial = spawn_inicial;
        inimigo->tile = spawn_inicial;
        inimigo->posicao_pixels = inimigo_tile_para_pixels(spawn_inicial);
        inimigo->velocidade.y = 0.0f;
        inimigo->aceleracao.x = 0.0f;
        inimigo->aceleracao.y = 0.0f;
        inimigo->tipo = tipo;
        if (tipo == INIMIGO_VELOZ) {
            inimigo->velocidade.x = VELOCIDADE_INIMIGO_PADRAO * 2.0f;
        } else {
            inimigo->velocidade.x = VELOCIDADE_INIMIGO_PADRAO;
        }
        inimigo->direcao_horizontal = DIRECAO_DIREITA;
        inimigo->ativo = true;
    }
}

void inimigo_atualizar(Inimigo* inimigo, const Mapa* mapa, float delta_tempo) {
    float nova_x;
    int col_borda;
    int linha_abaixo;
    bool borda;
    bool parede;

    if (!inimigo->ativo) {
        return;
    }

    // sincroniza tile com pixels
    inimigo->tile.linha = (int)(inimigo->posicao_pixels.y / TILE_SIZE);
    inimigo->tile.coluna = (int)(inimigo->posicao_pixels.x / TILE_SIZE);

    nova_x = inimigo->posicao_pixels.x + inimigo->velocidade.x * (float)inimigo->direcao_horizontal * delta_tempo;

    // limita a posição explicitamente antes de calcular col_borda evita que o inimigo saia da tela
    if (nova_x < 0.0f) {
        nova_x = 0.0f;
        inimigo->direcao_horizontal = DIRECAO_DIREITA;
    }
    if (nova_x + (float)TILE_SIZE > (float)(MAPA_COLUNAS * TILE_SIZE)) {
        nova_x = (float)((MAPA_COLUNAS - 1) * TILE_SIZE);
        inimigo->direcao_horizontal = DIRECAO_ESQUERDA;
    }

    // usa a borda lateral do sprite na direção do movimento, igual ao jogador
    // sem isso, indo para a direita col_borda apontava para a borda esquerda,
    // deixando que o inimigo avançasse quase um tile a mais antes de inverter
    if (inimigo->direcao_horizontal == DIRECAO_DIREITA) {
        col_borda = (int)((nova_x + (float)TILE_SIZE - 1.0f) / TILE_SIZE);
    } else {
        col_borda = (int)(nova_x / TILE_SIZE);
    }

    linha_abaixo = inimigo->tile.linha + 1;

    // parede é o limite do mapa ou tile Z à frente no mesmo nível
    parede = (col_borda < 0 || col_borda >= MAPA_COLUNAS);
    if (!parede) {
        parede = (mapa->tiles[inimigo->tile.linha][col_borda] == TILE_CHAO);
    }

    // borda é sem plataforma abaixo na coluna de borda (escadas também contam como chão)
    borda = false;
    if (!parede && linha_abaixo < MAPA_LINHAS) {
        char tile_frente_baixo = mapa->tiles[linha_abaixo][col_borda];
        borda = (tile_frente_baixo != TILE_CHAO
              && tile_frente_baixo != TILE_ESCADA_SUBIDA
              && tile_frente_baixo != TILE_ESCADA_DESCIDA
              && tile_frente_baixo != TILE_ESCADA_PADRAO);
    }

    if (parede || borda) {
        if (inimigo->direcao_horizontal == DIRECAO_DIREITA) {
            inimigo->direcao_horizontal = DIRECAO_ESQUERDA;
        } else {
            inimigo->direcao_horizontal = DIRECAO_DIREITA;
        }
    } else {
        inimigo->posicao_pixels.x = nova_x;
        inimigo->tile.coluna = (int)(nova_x / TILE_SIZE);
    }
}

bool inimigo_colide_com_jogador(const Inimigo* inimigo, const Jogador* jogador) {
    float ix_esq, ix_dir, iy_topo, iy_base;
    float jx_esq, jx_dir, jy_topo, jy_base;

    // caixas em pixels, cada entidade ocupa exatamente TILE_SIZE x TILE_SIZE
    ix_esq = inimigo->posicao_pixels.x;
    ix_dir = inimigo->posicao_pixels.x + (float)TILE_SIZE;
    iy_topo = inimigo->posicao_pixels.y;
    iy_base = inimigo->posicao_pixels.y + (float)TILE_SIZE;

    jx_esq = jogador->posicao_pixels.x;
    jx_dir = jogador->posicao_pixels.x + (float)TILE_SIZE;
    jy_topo = jogador->posicao_pixels.y;
    jy_base = jogador->posicao_pixels.y + (float)TILE_SIZE;

    return ix_esq < jx_dir && ix_dir > jx_esq
        && iy_topo < jy_base && iy_base > jy_topo;
}

void inimigo_resetar(Inimigo* inimigo, PosicaoMapa spawn_inicial) {
    if (inimigo) {
        inimigo->spawn_inicial = spawn_inicial;
        inimigo->tile = spawn_inicial;
        inimigo->posicao_pixels.x = (float)(spawn_inicial.coluna * TILE_SIZE);
        inimigo->posicao_pixels.y = (float)(spawn_inicial.linha * TILE_SIZE);
        // preserva tipo e animacao, reajusta velocidade conforme o tipo
        if (inimigo->tipo == INIMIGO_VELOZ) {
            inimigo->velocidade.x = VELOCIDADE_INIMIGO_PADRAO * 2.0f;
        } else {
            inimigo->velocidade.x = VELOCIDADE_INIMIGO_PADRAO;
        }
        inimigo->velocidade.y = 0.0f;
        inimigo->direcao_horizontal = DIRECAO_DIREITA;
        inimigo->ativo = true;
    }
}
