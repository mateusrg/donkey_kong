#include "animacao.h"
#define LARGURA_IMAGENS 900
#define ALTURA_IMAGENS 300


void animation_update(Animacao *animacaoPersonagem){
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
Rectangle animation_frame(const Animacao *animacaoPersonagem, int numero_frames_por_linha){
    // O tamanho real calculado pela divisão da imagem
    float tamanhoFrame = (float)LARGURA_IMAGENS / numero_frames_por_linha;
    
    float x = (float)(animacaoPersonagem->cur % numero_frames_por_linha) * tamanhoFrame;
    float y = (float)(animacaoPersonagem->cur / numero_frames_por_linha) * tamanhoFrame;

    return (Rectangle){  
        .x = x, 
        .y = y, 
        .width = tamanhoFrame,
        .height = tamanhoFrame,
    };
}

    void atualiza_entidades(Jogo *jogo) {
    // Pega o tempo que passou desde o último frame (Delta Time)
    // Nota: Se a sua função animation_update já chama GetFrameTime() lá dentro, 
    // você não precisa passar o dt aqui.
    
    // 1. Atualiza a animação do Mario (se ele estiver ativo)
    if (jogo->jogador.ativo) {
        animation_update(&jogo->jogador.animacao);
    }

    // 2. Atualiza a animação do Donkey
    if (jogo->donkey.ativo) {
        animation_update(&jogo->donkey.animacao);
    }

    // 3. Atualiza a animação da Princesa
    if (jogo->princesa.ativo) {
        animation_update(&jogo->princesa.animacao);
    }

    // 4. Atualiza a animação de todos os Fantasmas (Inimigos) ativos
    for (int numInimigos = 0; numInimigos < jogo->quantidade_inimigos; numInimigos++) {
        if (jogo->inimigos[numInimigos].ativo) {
            animation_update(&jogo->inimigos[numInimigos].animacao);
        }
    }
}