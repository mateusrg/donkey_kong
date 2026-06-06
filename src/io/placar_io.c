#include "placar_io.h"
#include <stdio.h>
#include <string.h>

bool placar_carregar(const char* caminho_arquivo, TipoPlacar* placar, int capacidade)
{
    FILE *arq1 = fopen(CAMINHO_ARQUIVO_PLACAR, "rb");
    if(!arq1){
        printf("Erro ao abrir arquivo do placar");
        fclose(arq1);
        return false;
    }
    else{
        if(fread(placar, sizeof(TipoPlacar), capacidade, arq1) == capacidade){
            fclose(arq1);
            return true;
        }
        else{
            printf("Erro ao ler o arquivo");
            fclose(arq1);
            return false;
        }
    }

}

bool placar_salvar(const char* caminho_arquivo, const TipoPlacar* placar, int capacidade)
{
    FILE *arq1 = fopen(CAMINHO_ARQUIVO_PLACAR, "wb") ;
    if(!arq1){
        printf("Erro ao abrir arquivo escrita do placar");
        fclose(arq1);
        return false;
    }
    else{
        if(fwrite(placar, sizeof(TipoPlacar), capacidade, arq1) == capacidade){
            fclose(arq1);
            return true;
        }
        else{
            printf("Erro ao escrever no arquivo");
            fclose(arq1);
            return false;
        }
    }

    fclose(arq1);
}

bool placar_elegivel(const TipoPlacar placar[], int capacidade, int novo_tempo)
{
    int indice = 0;

    while(indice < capacidade){
        if(novo_tempo < placar[indice].time){
            return true;
        }
        indice++;
    }

    return false;
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
