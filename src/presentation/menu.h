#pragma once

#include "../core/models.h"

/**
 * @brief Processa input e transições da tela de menu principal
 * @param[in,out] jogo Estado principal usado para navegar entre telas
 */
void menu_atualizar_menu_principal(Jogo* jogo);

/**
 * @brief Processa input e transições do menu de pausa durante a partida
 * @param[in,out] jogo Estado principal usado para retomar ou sair da partida
 */
void menu_atualizar_menu_pausa(Jogo* jogo);

/**
 * @brief Atualiza a captura do nome do jogador para o ranking
 * @param[in,out] jogo Estado principal que armazena o nome em edição
 */
void menu_atualizar_input_nome(Jogo* jogo);
