#include "animacao.h"
#include <math.h>
#include "../gameplay/jogador.h"
#define LARGURA_IMAGENS 900
#define ALTURA_IMAGENS 300


void atualiza_animacao(Animacao *animacaoPersonagem){
    float dt = GetFrameTime();
    animacaoPersonagem -> duration_left -= dt;

    if(animacaoPersonagem->duration_left <= 0.0f){
        animacaoPersonagem->duration_left = animacaoPersonagem->speed;
    
    // Incrementa
        animacaoPersonagem->cur += animacaoPersonagem->step;
    }
    //Se a animação esta no last, ela volta pro first
    if(animacaoPersonagem->cur > animacaoPersonagem->last){
        animacaoPersonagem->cur = animacaoPersonagem->first;
    }
}
Rectangle tamanho_frames(const Animacao *animacaoPersonagem, int numero_frames_por_linha, const DirecaoHorizontal DirecaoHorizontal){
    // O tamanho real calculado pela divisão da imagem
    float tamanhoFrameLargura = (float)LARGURA_IMAGENS / numero_frames_por_linha;
    float tamanhoFrameAltura = (float)LARGURA_IMAGENS / numero_frames_por_linha;
    
    float x = (float)(animacaoPersonagem->cur % numero_frames_por_linha) * tamanhoFrameLargura;
    float y = (float)(animacaoPersonagem->cur / numero_frames_por_linha) * tamanhoFrameAltura;

    if(DirecaoHorizontal == DIRECAO_DIREITA){
        tamanhoFrameLargura = fabs(tamanhoFrameLargura);
    }

    if(DirecaoHorizontal == DIRECAO_ESQUERDA){
        tamanhoFrameLargura *= (-1);
    }

    return (Rectangle){  
        .x = x, 
        .y = y, 
        .width = tamanhoFrameLargura,
        .height = tamanhoFrameAltura,
    };
}

void atualiza_anim_entidades(Jogo *jogo) {
    // Pega o tempo que passou desde o último frame (Delta Time)
    // Nota: Se a sua função atualiza_animacao já chama GetFrameTime() lá dentro, 
    // você não precisa passar o dt aqui.
    if(esta_parado(jogo->jogador.comandos)){
        jogo->jogador.animacao.first = 0;
        jogo->jogador.animacao.last = 0;
    }
    else{
        jogo->jogador.animacao.first = 1;
        jogo->jogador.animacao.last = 2;
    }

    if(!jogador_esta_sobre_plataforma(&jogo->jogador, &jogo->mapa)){
        jogo-> jogador.animacao.first = 3;
        jogo-> jogador.animacao.last = 3;
    }
   
    
    
    // 1. Atualiza a animação do Mario (se ele estiver ativo)
    if (jogo->jogador.ativo) {
        atualiza_animacao(&jogo->jogador.animacao);
    }

    // 2. Atualiza a animação do Donkey
    if (jogo->donkey.ativo) {
        atualiza_animacao(&jogo->donkey.animacao);
    }

    // 3. Atualiza a animação da Princesa
    if (jogo->princesa.ativo) {
        atualiza_animacao(&jogo->princesa.animacao);
    }

    // 4. Atualiza a animação de todos os Fantasmas (Inimigos) ativos
    for (int numInimigos = 0; numInimigos < jogo->quantidade_inimigos; numInimigos++) {
        if (jogo->inimigos[numInimigos].ativo) {
            atualiza_animacao(&jogo->inimigos[numInimigos].animacao);
        }
    }
}