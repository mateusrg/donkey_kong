#include "jogador.h"

#include <string.h>

Vetor2D jogador_tile_para_pixels(PosicaoMapa tile)
{
    Vetor2D posicao_pixels;

    posicao_pixels.x = (float)(tile.coluna * TILE_SIZE);
    posicao_pixels.y = (float)(tile.linha * TILE_SIZE);

    return posicao_pixels;
}

bool tile_e_parede(char tile)
{
    return tile == TILE_CHAO;
}

// Uma linha é de plataforma se tem pelo menos um Z.
// Shaft de escada (H sozinho, sem Z na linha) retorna false.
bool linha_e_plataforma(const Mapa* mapa, int linha)
{
    int c;

    for (c = 0; c < MAPA_COLUNAS; c++)
    {
        if (mapa->tiles[linha][c] == TILE_CHAO)
        {
            return true;
        }
    }

    return false;
}

void jogador_inicializar(Jogador* jogador, PosicaoMapa spawn_inicial)
{
    if (jogador)
    {
        // Sem memset: preserva campos de animação inicializados por render_inicializar.
        // Todos os campos de gameplay são explicitamente definidos abaixo.
        jogador->spawn_inicial       = spawn_inicial;
        jogador->tile                = spawn_inicial;
        jogador->posicao_pixels      = jogador_tile_para_pixels(spawn_inicial);
        jogador->velocidade.x        = 0.0f;
        jogador->velocidade.y        = 0.0f;
        jogador->aceleracao.x        = 0.0f;
        jogador->aceleracao.y        = GRAVIDADE_PADRAO;
        jogador->direcao_horizontal  = DIRECAO_DIREITA;
        jogador->vidas               = VIDAS_INICIAIS;
        jogador->ativo               = true;
        jogador->esta_no_chao        = true;
        jogador->esta_em_escada      = false;
        jogador->esta_pulando        = false;
        jogador->alcancou_porta      = false;
        jogador->invencivel          = false;
        jogador->tempo_invencibilidade = 0.0f;
    }
}

void jogador_resetar(Jogador* jogador, PosicaoMapa spawn_inicial)
{
    if (jogador)
    {
        jogador->spawn_inicial = spawn_inicial;
        jogador->tile = spawn_inicial;
        jogador->posicao_pixels = jogador_tile_para_pixels(spawn_inicial);
        jogador->velocidade.x = 0.0f;
        jogador->velocidade.y = 0.0f;
        jogador->aceleracao.x = 0.0f;
        jogador->aceleracao.y = GRAVIDADE_PADRAO;
        jogador->direcao_horizontal = DIRECAO_DIREITA;
        jogador->ativo = true;
        jogador->esta_no_chao = true;
        jogador->esta_em_escada = false;
        jogador->esta_pulando = false;        jogador->invencivel      = false;
        jogador->tempo_invencibilidade = 0.0f;    }
}

bool jogador_esta_sobre_plataforma(const Jogador* jogador, const Mapa* mapa)
{
    int linha_base;
    int col_centro;
    char tile;

    // Usa o centro horizontal do sprite para evitar falsos positivos em colunas
    // de escada adjacentes, que causariam "plataforma fantasma" ao pular perto delas.
    linha_base = (int)((jogador->posicao_pixels.y + (float)TILE_SIZE) / TILE_SIZE);
    col_centro = (int)((jogador->posicao_pixels.x + (float)(TILE_SIZE / 2)) / TILE_SIZE);

    if (linha_base >= MAPA_LINHAS)
    {
        return true;
    }

    if (col_centro < 0 || col_centro >= MAPA_COLUNAS)
    {
        return false;
    }

    // Z é sempre chão firme. H só conta se estiver numa linha de plataforma
    // (linha com Z), ou seja, ZZZZHZZZZ. H de shaft (linha sem Z) não é chão.
    // S e D nunca contam aqui — ficam uma linha acima do Z/H e causariam phantom.
    tile = mapa->tiles[linha_base][col_centro];
    if (tile == TILE_CHAO)
    {
        return true;
    }
    if (tile == TILE_ESCADA_PADRAO)
    {
        return linha_e_plataforma(mapa, linha_base);
    }
    return false;
}

// Entrada na escada: S+cima, D+baixo, ou H em qualquer direção vertical
// (permite entrar na escada ao pular nela e aterrissar num tile H do shaft).
// Ao entrar, trava x, zera velocidade e limpa o estado de pulo.
void jogador_usar_escada(Jogador* jogador, const Mapa* mapa, ComandosJogador comandos, float delta_tempo)
{
    char tile_atual;
    float nova_y;
    int nova_linha;
    char tile_destino;
    bool entra;

    tile_atual = mapa->tiles[jogador->tile.linha][jogador->tile.coluna];

    if (!jogador->esta_em_escada)
    {
        entra = false;

        if ((tile_atual == TILE_ESCADA_SUBIDA || tile_atual == TILE_ESCADA_PADRAO) && comandos.vertical == DIRECAO_CIMA)
        {
            entra = true;
        }
        else if ((tile_atual == TILE_ESCADA_DESCIDA || tile_atual == TILE_ESCADA_PADRAO) && comandos.vertical == DIRECAO_BAIXO)
        {
            entra = true;
        }

        if (!entra)
        {
            return;
        }

        jogador->esta_em_escada = true;
        jogador->posicao_pixels.x = (float)(jogador->tile.coluna * TILE_SIZE);
        jogador->velocidade.y = 0.0f;
        jogador->esta_pulando = false;
    }

    if (comandos.vertical == DIRECAO_VERTICAL_NENHUMA)
    {
        return;
    }

    nova_y = jogador->posicao_pixels.y + (float)comandos.vertical * VELOCIDADE_ESCADA_PADRAO * delta_tempo;
    nova_linha = (int)(nova_y / TILE_SIZE);

    if (nova_linha < 0)
    {
        nova_linha = 0;
        nova_y = 0.0f;
    }
    if (nova_linha >= MAPA_LINHAS)
    {
        nova_linha = MAPA_LINHAS - 1;
        nova_y = (float)(nova_linha * TILE_SIZE);
    }

    tile_destino = mapa->tiles[nova_linha][jogador->tile.coluna];

    if (tile_destino == TILE_ESCADA_DESCIDA && comandos.vertical == DIRECAO_CIMA)
    {
        jogador->posicao_pixels.y = (float)(nova_linha * TILE_SIZE);
        jogador->tile.linha = nova_linha;
        jogador->velocidade.y = 0.0f;
        jogador->esta_em_escada = false;
        return;
    }

    if (tile_destino == TILE_ESCADA_SUBIDA && comandos.vertical == DIRECAO_BAIXO)
    {
        jogador->posicao_pixels.y = (float)(nova_linha * TILE_SIZE);
        jogador->tile.linha = nova_linha;
        jogador->velocidade.y = 0.0f;
        jogador->esta_em_escada = false;
        return;
    }

    jogador->posicao_pixels.y = nova_y;
    jogador->tile.linha = nova_linha;
}

// Resolve colisão horizontal por intervalo de pixels.
// A bounding box do jogador é [x, x+W) × [y, y+H).
// Para mover para a direita, verifica a coluna da borda direita na nova posição.
// Para mover para a esquerda, verifica a coluna da borda esquerda na nova posição.
// Varre todas as linhas que o sprite ocupa em Y para não entrar por cima.
void jogador_mover_horizontal(Jogador* jogador, const Mapa* mapa, ComandosJogador comandos, float delta_tempo)
{
    float nova_x;
    int linha_topo;
    int linha_base;
    int col_borda;
    int linha;
    bool bloqueado;

   

    nova_x = jogador->posicao_pixels.x + (float)comandos.horizontal * VELOCIDADE_JOGADOR_PADRAO * delta_tempo;

    // Clamp nos limites da tela antes de calcular col_borda.
    // Sem isso, nova_x ligeiramente negativa dá col_borda = 0 (truncação C),
    // passando no check col_borda < 0 e deixando o sprite sair da tela.
    if (nova_x < 0.0f)
    {
        nova_x = 0.0f;
    }
    if (nova_x + (float)TILE_SIZE > (float)(MAPA_COLUNAS * TILE_SIZE))
    {
        nova_x = (float)((MAPA_COLUNAS - 1) * TILE_SIZE);
    }

    // Linhas que o sprite ocupa verticalmente
    linha_topo = (int)(jogador->posicao_pixels.y / TILE_SIZE);
    linha_base = (int)((jogador->posicao_pixels.y + (float)TILE_SIZE - 1.0f) / TILE_SIZE);

    if (comandos.horizontal == DIRECAO_DIREITA)
    {
        // Borda direita do sprite na nova posição
        col_borda = (int)((nova_x + (float)TILE_SIZE - 1.0f) / TILE_SIZE);
    }
    else
    {
        // Borda esquerda do sprite na nova posição
        col_borda = (int)(nova_x / TILE_SIZE);
    }

    if (col_borda < 0 || col_borda >= MAPA_COLUNAS)
    {
        return;
    }

    bloqueado = false;
    for (linha = linha_topo; linha <= linha_base; linha++)
    {
        if (linha >= 0 && linha < MAPA_LINHAS && tile_e_parede(mapa->tiles[linha][col_borda]))
        {
            bloqueado = true;
            break;
        }
    }

    if (bloqueado)
    {
        // Snap na borda do tile bloqueante para não deixar gap nem overlap
        if (comandos.horizontal == DIRECAO_DIREITA)
        {
            jogador->posicao_pixels.x = (float)(col_borda * TILE_SIZE) - (float)TILE_SIZE;
        }
        else
        {
            jogador->posicao_pixels.x = (float)((col_borda + 1) * TILE_SIZE);
        }
        return;
    }

    jogador->posicao_pixels.x = nova_x;
    jogador->tile.coluna = (int)(nova_x / TILE_SIZE);

    if (comandos.horizontal == DIRECAO_NENHUMA) {
        // Não tem direção padrao -> Salva a ultima realizada
        return;
    }
    if (comandos.horizontal == DIRECAO_DIREITA)
    {
        jogador->direcao_horizontal = DIRECAO_DIREITA;
    }
    else if (comandos.horizontal == DIRECAO_ESQUERDA)
    {
        jogador->direcao_horizontal = DIRECAO_ESQUERDA;
    }
    
        
}

// Resolve colisão vertical por intervalo de pixels.
// delta_tempo é limitado a 50ms internamente para evitar tunneling no primeiro
// frame, quando GetFrameTime() pode retornar o tempo gasto na inicialização inteira.
// Ao cair: varre apenas as linhas ABAIXO do corpo atual (não re-verifica tiles que
// o sprite já ocupa). Só Z (TILE_CHAO) e H (TILE_ESCADA_PADRAO) bloqueiam a queda;
// S e D são passáveis para que o jogador não flutue ao pular de cima deles.
void jogador_mover_vertical(Jogador* jogador, const Mapa* mapa, float delta_tempo)
{
    float dt;
    float nova_y;
    int col_esq;
    int col_dir;
    int linha_base_atual;
    int nova_linha_base;
    int linha_verificar;
    bool bloqueado;
    int col;

    // Clamp: previne tunneling quando delta_tempo é grande (lag spike, primeiro frame)
    dt = delta_tempo;
    if (dt > 0.05f)
    {
        dt = 0.05f;
    }

    jogador->velocidade.y += GRAVIDADE_PADRAO * dt;
    nova_y = jogador->posicao_pixels.y + jogador->velocidade.y * dt;

    // Colunas que o sprite ocupa horizontalmente (usa posição atual, estável)
    col_esq = (int)(jogador->posicao_pixels.x / TILE_SIZE);
    col_dir = (int)((jogador->posicao_pixels.x + (float)TILE_SIZE - 1.0f) / TILE_SIZE);

    if (col_esq < 0) col_esq = 0;
    if (col_dir >= MAPA_COLUNAS) col_dir = MAPA_COLUNAS - 1;

    if (jogador->velocidade.y > 0.0f)
    {
        // Caindo: varre da primeira linha ABAIXO do corpo atual até a nova linha base.
        // Iniciar depois do corpo atual evita bloquear em D/S que o sprite já ocupa,
        // o que causaria o snap prematuro (flutuar um tile acima do destino correto).
        linha_base_atual = (int)((jogador->posicao_pixels.y + (float)TILE_SIZE - 1.0f) / TILE_SIZE);
        nova_linha_base  = (int)((nova_y               + (float)TILE_SIZE - 1.0f) / TILE_SIZE);

        bloqueado = false;
        for (linha_verificar = linha_base_atual + 1;
             linha_verificar <= nova_linha_base && linha_verificar < MAPA_LINHAS;
             linha_verificar++)
        {
            for (col = col_esq; col <= col_dir; col++)
            {
                char t;

                t = mapa->tiles[linha_verificar][col];

                // Z bloqueia sempre. H bloqueia só se for linha de plataforma (tem Z).
                // H de shaft e S/D não bloqueiam — evita phantom acima de escadas.
                if (t == TILE_CHAO ||
                    (t == TILE_ESCADA_PADRAO && linha_e_plataforma(mapa, linha_verificar)))
                {
                    bloqueado = true;
                    break;
                }
            }

            if (bloqueado)
            {
                nova_y = (float)((linha_verificar - 1) * TILE_SIZE);
                jogador->velocidade.y = 0.0f;
                jogador->esta_pulando = false;
                break;
            }
        }

        if (!bloqueado && nova_linha_base >= MAPA_LINHAS)
        {
            nova_y = (float)((MAPA_LINHAS - 1) * TILE_SIZE);
            jogador->velocidade.y = 0.0f;
            jogador->esta_pulando = false;
        }
    }
    else if (jogador->velocidade.y < 0.0f)
    {
        // Subindo: verifica a linha do topo do sprite na nova posição
        linha_verificar = (int)(nova_y / TILE_SIZE);

        if (linha_verificar < 0)
        {
            nova_y = 0.0f;
            jogador->velocidade.y = 0.0f;
        }
        else
        {
            bloqueado = false;
            for (col = col_esq; col <= col_dir; col++)
            {
                if (tile_e_parede(mapa->tiles[linha_verificar][col]))
                {
                    bloqueado = true;
                    break;
                }
            }

            if (bloqueado)
            {
                // Snap: topo do sprite encosta na base do tile bloqueante
                nova_y = (float)((linha_verificar + 1) * TILE_SIZE);
                jogador->velocidade.y = 0.0f;
            }
        }
    }

    jogador->posicao_pixels.y = nova_y;
    jogador->tile.linha = (int)(nova_y / TILE_SIZE);
}

void jogador_atualizar(Jogador* jogador, const Mapa* mapa, ComandosJogador comandos, float delta_tempo)
{
    int linha_plataforma;

    if (!jogador->ativo)
    {
        return;
    }

    // Escada tem prioridade — se em escada, pula física do restante do frame
    jogador_usar_escada(jogador, mapa, comandos, delta_tempo);

    if (jogador->esta_em_escada)
    {
        jogador->tile.linha   = (int)(jogador->posicao_pixels.y / TILE_SIZE);
        jogador->tile.coluna  = (int)(jogador->posicao_pixels.x / TILE_SIZE);
        return;
    }

    jogador->esta_no_chao = jogador_esta_sobre_plataforma(jogador, mapa);

    // Pulo: só possível quando apoiado no chão
    if (comandos.acao_pulo && jogador->esta_no_chao && !jogador->esta_pulando)
    {
        jogador->velocidade.y = -FORCA_PULO_PADRAO;
        jogador->esta_pulando = true;
    }

    jogador_mover_horizontal(jogador, mapa, comandos, delta_tempo);

    // Recalcula após horizontal — pode ter saído da borda de uma plataforma
    jogador->esta_no_chao = jogador_esta_sobre_plataforma(jogador, mapa);

    if (jogador->esta_no_chao && !jogador->esta_pulando)
    {
        // No chão: snap preciso na superfície da plataforma e zera velocidade vertical.
        // Sem isso, gravidade acumula frame a frame e no primeiro frame (delta grande)
        // o jogador atravessaria a plataforma antes de ser corrigido.
        linha_plataforma = (int)((jogador->posicao_pixels.y + (float)TILE_SIZE) / TILE_SIZE);
        jogador->posicao_pixels.y = (float)((linha_plataforma - 1) * TILE_SIZE);
        jogador->velocidade.y = 0.0f;
    }
    else
    {
        jogador_mover_vertical(jogador, mapa, delta_tempo);
        jogador->esta_no_chao = jogador_esta_sobre_plataforma(jogador, mapa);
        // Só limpa esta_pulando ao pousar (velocidade >= 0 = caindo ou parado).
        // Sem esse check, D e H abaixo do corpo durante a subida zerariam esta_pulando
        // prematuramente, fazendo o snap disparar no frame seguinte ("plataforma fantasma").
        if (jogador->esta_no_chao && jogador->velocidade.y >= 0.0f)
        {
            jogador->esta_pulando = false;
        }
    }

    // Sincroniza tile com a posição final em pixels
    jogador->tile.linha   = (int)(jogador->posicao_pixels.y / TILE_SIZE);
    jogador->tile.coluna  = (int)(jogador->posicao_pixels.x / TILE_SIZE);
}

bool esta_parado(void)
{
    return !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)
        && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_S) && !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP);
}
