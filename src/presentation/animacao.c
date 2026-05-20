#include "animacao.h"
#define LARGURA_IMAGENS 900
#define ALTURA_IMAGENS 300
#define NUM_FRAMES_HORIZONTAL 6

/*
typedef enum AnimationType{
    STOPPED = 0,
    WALKING_RIGHT = 1,
    WALKING_LEFT = 2,
    JUMPING = 3,
} AnimationType;
 


typedef struct Animation {
    int first;
    int last;
    int cur; // current
    float speed;
    float duration_left;
    AnimationType type;
    int step; // how far you are stepping to the next frame
} Animation;
 */
 

void animation_update(Animation *self){
    float dt = GetFrameTime();
    self -> duration_left -= dt;

    if(self->duration_left <= 0.0f){
        self->duration_left = self->speed;
    
    // Incrementa
        self->cur += self->step;
    }
    //Se a animação esta no last, ela volta pro first
    if(self->cur > self->last){
        self->cur = self->first;
    }
}
Rectangle animation_frame(Animation *self, int numero_frames_por_linha){
    // O tamanho real calculado pela divisão da imagem
    float tamanhoFrame = (float)LARGURA_IMAGENS / NUM_FRAMES_HORIZONTAL;
    
    float x = (float)(self->cur % numero_frames_por_linha) * tamanhoFrame;
    float y = (float)(self->cur / numero_frames_por_linha) * tamanhoFrame;

    return (Rectangle){  
        .x = x, 
        .y = y, 
        .width = tamanhoFrame,
        .height = tamanhoFrame,
    };
}