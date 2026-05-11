#pragma once

#include <stdbool.h>

#include "../core/models.h"

/**
 * @brief Carrega do disco as entradas do placar até a capacidade informada
 * @param[in] caminho_arquivo Caminho do arquivo binário do placar
 * @param[out] placar Vetor que receberá as entradas carregadas
 * @param[in] capacidade Quantidade máxima de entradas que podem ser lidas
 * @return true se o placar foi carregado com sucesso
 * @return false se houve erro ao abrir ou ler o arquivo
 */
bool placar_carregar(const char* caminho_arquivo, TipoPlacar* placar, int capacidade);

/**
 * @brief Persiste em disco o placar atual usando o formato binário do projeto
 * @param[in] caminho_arquivo Caminho do arquivo binário do placar
 * @param[in] placar Vetor com as entradas que devem ser salvas
 * @param[in] capacidade Quantidade de posições consideradas na gravação
 * @return true se o placar foi salvo com sucesso
 * @return false se houve erro ao abrir ou gravar o arquivo
 */
bool placar_salvar(const char* caminho_arquivo, const TipoPlacar* placar, int capacidade);

/**
 * @brief Informa se um novo tempo merece entrar no ranking atual
 * @param[in] placar Vetor com as entradas atuais do ranking
 * @param[in] capacidade Quantidade total de posições disponíveis no ranking
 * @param[in] novo_tempo Tempo que será avaliado para entrada no placar
 * @return true se o novo tempo for elegível ao ranking
 * @return false se o novo tempo não alcançar o placar atual
 */
bool placar_elegivel(const TipoPlacar* placar, int capacidade, int novo_tempo);

/**
 * @brief Insere uma nova pontuação no placar mantendo a ordenação esperada
 * @param[in,out] placar Vetor de ranking que será reorganizado
 * @param[in] capacidade Quantidade total de posições disponíveis no ranking
 * @param[in] nome Nome do jogador associado ao novo tempo
 * @param[in] novo_tempo Tempo que será inserido no ranking
 */
void placar_inserir(TipoPlacar* placar, int capacidade, const char* nome, int novo_tempo);
