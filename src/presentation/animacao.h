#include "raylib.h"
/**
 * @brief Define o estado atual  do personagem
 */
typedef enum AnimationType{
    STOPPED = 0,
    WALKING_RIGHT = 1,
    WALKING_LEFT = 2,
    JUMPING = 3,
} AnimationType;
 
/**
 * Define as caracteristicas da animação e que frames serão utilizados
 */
typedef struct Animation {
    int first;
    int last;
    int cur; // current
    float speed;
    float duration_left;
    AnimationType type; // Stopped, Walking, etc.
    int step; // how far you are stepping to the next frame
} Animation;

/**
 * @brief função responsável pelo looping da animação
 * @param[in] self recebe os dados necessários da animação (frame atual, velocidade, ultimo frame, etc..)
 */
void animation_update(Animation *self);

/**
 * @brief função responsável por selecionar quais serão os frames do looping
 * @param[in] self recebe os dados necessários da animação (frame atual, velocidade, ultimo frame, etc..)
 * @param[in] numero_frames_por_linha Informa para a função quantos frames a imagem possui no total
 */
Rectangle animation_frame(Animation *self, int numero_frames_por_linha);