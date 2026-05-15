#pragma once
#include "../core/models.h"

/**
 * @brief Inicializa o dispositivo de áudio e carrega todos os sons/músicas
 */
void inicializar_audio(void);

/** 
 * @brief Atualiza o streaming da musica de fundo, que deve ser chamado todo frame
 */
void atualizar_audio_musica(void);
/**
 * @brief Toca o audio que possui o nome do efeito passado de argumento
 * @param[in] nome_efeito String que possui o nome do som
 */
void tocar_audio_efeito(const char* nome_efeito);
/**
 * @brief Encerra o dispositivo de áudio e descarrega todos os sons/músicas
 */
void encerrar_audio (void);
