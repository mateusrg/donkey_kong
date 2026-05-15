#include <string.h>
#include "audio.h"
#include "raylib.h"

// Declaro o tipo das variáveis, static pois só este arquivo enxerga elas
static Music musica_fundo;
static Sound som_pulo;
static Sound som_morte;
static Sound som_inicio;
static Sound som_porta;
static Sound som_passos;
static Sound som_abre_jogo;
static Sound som_fase_final;
static Sound som_botao_sair;
static Sound som_upgrade;
static Sound som_ver_placar;
static Sound som_pausa;

void inicializar_audio(void) {
    InitAudioDevice(); // Inicia o áudio

    // Carrega a musica de fundo
    musica_fundo = LoadMusicStream("assets/audios/musica_fundo.mp3");
    musica_fundo.looping = true; // Garante que ela entre em loop quando acabe
    PlayMusicStream(musica_fundo);
    SetMusicVolume(musica_fundo, 0.2f); //Deixa mais baixa
    
    // Carregando os efeitos
    som_pulo = LoadSound("assets/audios/pulo.wav");
    som_morte = LoadSound("assets/audios/morte.wav");
    som_inicio = LoadSound("assets/audios/iniciar.wav");
    som_porta = LoadSound("assets/audios/alcancou porta.wav");
    som_passos = LoadSound("assets/audios/passos.wav");
    som_abre_jogo = LoadSound("assets/audios/jogo abre.wav");
    som_fase_final = LoadSound("assets/audios/fase final.wav");
    som_botao_sair = LoadSound("assets/audios/botao sair.wav");
    som_upgrade = LoadSound("assets/audios/upgrade.wav");
    som_ver_placar = LoadSound("assets/audios/ver placar.wav");
    som_pausa = LoadSound("assets/audios/pause.wav");
    
    // Configurando o som dos passos
    SetSoundVolume(som_pulo, 0.5f);
    SetSoundVolume(som_morte, 0.7f);
    SetSoundVolume(som_inicio, 0.7f);
    SetSoundVolume(som_porta, 0.3f);
    SetSoundVolume(som_passos, 0.2f);
    SetSoundVolume(som_abre_jogo, 0.7f);
    SetSoundVolume(som_fase_final, 0.6f);
    SetSoundVolume(som_botao_sair, 0.3f);
    SetSoundVolume(som_upgrade, 0.4f);
    SetSoundVolume(som_ver_placar, 0.3f);
    SetSoundVolume(som_pausa, 0.5f);
}

void atualizar_audio_musica(void) {
    UpdateMusicStream(musica_fundo);
}

void tocar_audio_efeito(const char* nome_efeito) {
    if (strcmp(nome_efeito, "pulo") == 0) {
        PlaySound(som_pulo);
    } 
    else if (strcmp(nome_efeito, "morte") == 0) {
        PlaySound(som_morte);
    } 
    else if (strcmp(nome_efeito, "inicio") == 0) {
        PlaySound(som_inicio);
    } 
    else if (strcmp(nome_efeito, "porta") == 0) {
        PlaySound(som_porta);
    } 
    else if (strcmp(nome_efeito, "abre_jogo") == 0) {
        PlaySound(som_abre_jogo);
    } 
    else if (strcmp(nome_efeito, "fase_final") == 0) {
        PlaySound(som_fase_final);
    } 
    else if (strcmp(nome_efeito, "botao_sair") == 0) {
        PlaySound(som_botao_sair);
    } 
    else if (strcmp(nome_efeito, "upgrade") == 0) {
        PlaySound(som_upgrade);
    } 
    else if (strcmp(nome_efeito, "ver_placar") == 0) {
        PlaySound(som_ver_placar);
    } 
    else if (strcmp(nome_efeito, "pausa") == 0) {
        PlaySound(som_pausa);
    } 
    else if (strcmp(nome_efeito, "passos") == 0) {
        if (!IsSoundPlaying(som_passos)) {
            PlaySound(som_passos);
        }
    }
}

void encerrar_audio(void) {
    // Descarrega tudo da RAM
    UnloadMusicStream(musica_fundo);
    UnloadSound(som_pulo);
    UnloadSound(som_morte);
    UnloadSound(som_inicio);
    UnloadSound(som_porta);
    UnloadSound(som_passos);
    UnloadSound(som_abre_jogo);
    UnloadSound(som_fase_final);
    UnloadSound(som_botao_sair);
    UnloadSound(som_upgrade);
    UnloadSound(som_ver_placar);
    UnloadSound(som_pausa);

    CloseAudioDevice();
}