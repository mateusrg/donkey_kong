#include "raylib.h"
#include <stdio.h>
#include <string.h> 
#include "core/constants.h" 
#include "core/models.h"
#include "io/mapa_io.h"
#include "presentation/audio.h"
#include "presentation/animacao.h"
#include "presentation/render.h" // Certifique-se de que o caminho está correto

int main(void) {
    // 1. Inicialização de dados
    char arquivoMapa[100];
    Jogo meuJogo = { 0 }; // Zera toda a struct 

    // 2. Monta o caminho e tenta carregar o mapa
    if (!mapa_montar_caminho_fase(3, arquivoMapa)) {
        printf("ERRO: Nome da fase invalido.\n");
        return 1;
    }

    // Passamos os endereços do que está DENTRO da meuJogo
    if (!mapa_carregar(arquivoMapa, &meuJogo)) {
        printf("ERRO CRITICO: Nao foi possivel carregar o arquivo: %s\n", arquivoMapa);
        return 1;
    }

    // 3. Inicializa a Janela
    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Teste de Carregamento de Mapa");
    SetTargetFPS(60);

    // 4. Inicializa as texturas (chama aquela sua função que redimensiona)
    render_inicializar(&meuJogo);
    // 5. Inicializa os audios do jogo
    inicializar_audio();

    // Loop de teste
    while (!WindowShouldClose()) {

        //Atualizações

        atualizar_audio_musica();

        atualiza_entidades(&meuJogo);
        
        // --- DESENHO ---
        BeginDrawing();
            ClearBackground(BLACK); // Limpa a tela antes de desenhar

            // Desenha o mapa e as texturas/animacoes dos personagens
            
            render_desenhar(&meuJogo);
            // Texto de apoio para saber se carregou algo
            DrawText("Se o mapa nao aparecer, verifique os caracteres no .txt", 10, 10, 20, RAYWHITE);
            DrawText(TextFormat("Inimigos carregados: %d", meuJogo.quantidade_inimigos), 10, 40, 20, YELLOW);

        EndDrawing();
    }

    // 5. Finalização
    render_encerrar();
    encerrar_audio();
    CloseWindow();

    return 0;
}