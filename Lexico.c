#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <Tokens.h>

// ÁREA PRA DEMONSTRAR OS ÍNDICES (TABELA HASH)
#define INDEX_SIZE 100

typedef struct No{
    char *valor;
    struct No *proximo;
} No;

typedef struct TabelaHash{
    No *tabela[INDEX_SIZE];
} TabelaHash;

unsigned int hash(char *chave){
    unsigned int hashvalue = 0;

    while (*chave != '\0'){
        hashvalue = *chave + (hashvalue << 5) - hashvalue;
        chave++;
    }
    return hashvalue % INDEX_SIZE;
}

TabelaHash *criarTabela(){
    TabelaHash *nova_tabela = malloc(sizeof(TabelaHash));
    memset(nova_tabela->tabela, 0, sizeof(No *) *INDEX_SIZE);
    return nova_tabela;
}

void inserir(TabelaHash *tabela, char *valor){
    unsigned int indice = hash(valor);
    No *atual = tabela->tabela[indice];

    while (atual != NULL){
        if(strcmp(atual->valor, valor) == 0){
            return;
        }
        atual = atual->proximo;
    }
    No *novo_No = malloc(sizeof(No));
    novo_No->valor = strdup(valor);
    novo_No->proximo = tabela->tabela[indice];
    tabela->tabela[indice] = novo_No;
}

void displayTabela(TabelaHash *tabela){
    for (int i = 0; i < INDEX_SIZE; i++){
        No *atual = tabela->tabela[i];
        printf("LINHA %d:", i);

        while (atual != NULL){
            printf("%s", atual->proximo);
        }
        printf("\n");
    }
    
}

int main(){



    char caracteres_especiais[] = {
        '=',
        '>=',
        '*',
        '<>',
        '<=',
        '/',
        '>',
        '+',
        '=',
        '<',
        '-'
    };

    char simbolos[] = {
        '{',
        ',',
        '}',
        ';',
        '(',
        ')'
    };
}