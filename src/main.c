#include "raylib.h"
#include "presentation/audio.h"
#include "core/constants.h" 

int main(void) {
    //Configurações Iniciais usando as constantes do projeto
    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Donkey Kong - Prototipo");
    SetTargetFPS(FPS_PADRAO); 

    // Inicializa os audios
    inicializar_audio(); 

    //Loop Principal
    while (!WindowShouldClose()) {
        
        // Continuar com o loop da musica principal
        atualizar_audio_musica(); 
        
        // Exemplo de teste rápido
        if (IsKeyPressed(KEY_SPACE)) {
            tocar_audio_efeito("pulo");
        }

        // Desenho
        BeginDrawing();
            ClearBackground(BLACK); 

            DrawText("Sistema de Áudio: OK!", 10, 10, 20, GREEN);
            
        EndDrawing();
    }

    // Encerrar os audios na memoria
    encerrar_audio();

    //Encerrar o programa
    CloseWindow();

    return 0;
}