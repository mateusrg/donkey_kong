#include "placar_io.h"
#include <stdio.h>
#include <string.h>

bool placar_carregar(const char* caminho_arquivo, TipoPlacar* placar, int capacidade)
{
    FILE *arq1;
    if(!(arq1 = fopen(CAMINHO_ARQUIVO_PLACAR, "rb"))){
        printf("Erro ao abrir arquivo do placar");
        return false;
    }
    else{
        if(fread(placar, sizeof(TipoPlacar), capacidade, arq1) == capacidade){
            return true;
        }
        else{
            printf("Erro ao ler o arquivo");
            return false;
        }
    }

    fclose(arq1);
}

bool placar_salvar(const char* caminho_arquivo, const TipoPlacar* placar, int capacidade)
{
    FILE *arq1;
    if(!(arq1 = fopen(CAMINHO_ARQUIVO_PLACAR, "wb"))){
        printf("Erro ao abrir arquivo escrita do placar");
        return false;
    }
    else{
        if(fwrite(placar, sizeof(TipoPlacar), capacidade, arq1) == capacidade){
            return true;
        }
        else{
            printf("Erro ao escrever no arquivo");
            return false;
        }
    }

    fclose(arq1);
}

bool placar_elegivel(const TipoPlacar placar[], int capacidade, int novo_tempo)
{
    int flag = 0;
    int indice = 0;

    while(!flag && indice < capacidade){
        if(novo_tempo > placar[indice].time){
            flag = 1;
        }
        indice++;
    }
}

void placar_inserir(TipoPlacar* placar, int capacidade,char* nome, int novo_tempo)
{
    int flag = 0;
    int indice = 0;
        while(!flag && indice < capacidade){
            if(novo_tempo < placar[indice].time){
                for(int i = capacidade - 1; i > indice; i--){
                    placar[i] = placar[i - 1];
                }
                placar[indice].time = novo_tempo;
                strcpy(placar[indice].nome, nome);
                flag = 1;
            }
            indice++;
        }
}
