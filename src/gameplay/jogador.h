#pragma once

#include "../core/models.h"

/**
 * @brief Prepara o jogador para a fase usando o tile de spawn informado
 * @param[out] jogador Estrutura do jogador que será inicializada
 * @param[in] spawn_inicial Posição inicial do jogador em coordenadas de tile
 */
void jogador_inicializar(Jogador* jogador, PosicaoMapa spawn_inicial);

/**
 * @brief Recoloca o jogador no spawn e limpa o estado temporário de movimento
 * @param[in,out] jogador Estrutura do jogador que será resetada
 * @param[in] spawn_inicial Posição de respawn em coordenadas de tile
 */
void jogador_resetar(Jogador* jogador, PosicaoMapa spawn_inicial);

/**
 * @brief Atualiza movimento e estados do jogador
 * @param[in,out] jogador Estrutura do jogador que será atualizada
 * @param[in] mapa Mapa atual usado para colisões e validações de movimento
 * @param[in] comandos Comandos de entrada do frame atual
 * @param[in] delta_tempo Tempo decorrido desde o último frame em segundos
 */
void jogador_atualizar(Jogador* jogador, const Mapa* mapa, ComandosJogador comandos, float delta_tempo);

/**
 * @brief Informa se existe uma plataforma sustentando o jogador no mapa atual
 * @param[in] jogador Estado atual do jogador
 * @param[in] mapa Mapa consultado para verificar suporte abaixo do jogador
 * @return true se houver uma plataforma sustentando o jogador
 * @return false se o jogador estiver sem apoio no tile abaixo
 */
bool jogador_esta_sobre_plataforma(const Jogador* jogador, const Mapa* mapa);

/**
 * @brief Informa se o jogador está parado ou se movendo
 * @return true se nenhuma tecla de movimento estiver pressionada
 */
bool esta_parado(void);

/**
 * @brief Converte um tile do mapa para a origem em pixels usada pelo jogador
 * @param[in] tile Posição em coordenadas de mapa
 * @return Vetor com a posição equivalente em pixels
 */
Vetor2D jogador_tile_para_pixels(PosicaoMapa tile);

/**
 * @brief Informa se um tile bloqueia passagem em qualquer direção (apenas Z)
 * @param[in] tile Caractere do tile a ser verificado
 * @return true se o tile é uma parede sólida em todos os lados
 * @return false caso contrário
 */
bool tile_e_parede(char tile);

/**
 * @brief Informa se uma linha do mapa é uma linha de plataforma
 * @param[in] mapa Mapa consultado
 * @param[in] linha Índice da linha a verificar
 * @return true se a linha contém ao menos um TILE_CHAO
 * @return false se a linha é shaft de escada ou vazia
 */
bool linha_e_plataforma(const Mapa* mapa, int linha);

/**
 * @brief Controla entrada e movimento contínuo do jogador na escada
 * @param[in,out] jogador Estado do jogador que será lido e modificado
 * @param[in] mapa Mapa consultado para detectar tiles de escada
 * @param[in] comandos Comandos de entrada do frame atual
 * @param[in] delta_tempo Tempo decorrido desde o último frame em segundos
 */
void jogador_usar_escada(Jogador* jogador, const Mapa* mapa, ComandosJogador comandos, float delta_tempo);

/**
 * @brief Move o jogador horizontalmente resolvendo colisão por intervalo de pixels
 * @param[in,out] jogador Estado do jogador que será lido e modificado
 * @param[in] mapa Mapa consultado para checar colisão horizontal
 * @param[in] comandos Comandos de entrada do frame atual
 * @param[in] delta_tempo Tempo decorrido desde o último frame em segundos
 */
void jogador_mover_horizontal(Jogador* jogador, const Mapa* mapa, ComandosJogador comandos, float delta_tempo);

/**
 * @brief Move o jogador verticalmente resolvendo colisão por intervalo de pixels
 * @param[in,out] jogador Estado do jogador que será lido e modificado
 * @param[in] mapa Mapa consultado para detectar plataformas e teto
 * @param[in] delta_tempo Tempo decorrido desde o último frame em segundos
 */
void jogador_mover_vertical(Jogador* jogador, const Mapa* mapa, float delta_tempo);
