#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tokens.h"

#define MAX_MEMORIA_ANALISADOR 100
#define MAX_TAMANHO_SIMBOLOS 100

void __lerArquivo(char f[]){
    FILE *file = fopen(f, "r");
    char letra;

    if(file){
        printf("\n\tTEXTO LIDO DO ARQUIVO:\n");
        while(!feof(file)){
            letra = fgetc(file);
            printf("%c", letra);
        }

        fclose(file);
        printf("\n");
    }
    else{
        printf("\nERRO AO ABRIR O ARQUIVO!\n");
    }
}

typedef enum {

    TK_PALAVRA_CHAVE,
    TK_IDENTIFICADOR,
    TK_OPERADOR,
    TK_SIMBOLO,
    TK_INTEGER,
    TK_REALS,
    TK_ERRO


} TiposTokens;

typedef struct {
    TiposTokens tipo;
    char tamanho[MAX_MEMORIA_ANALISADOR];
    int linha_ocorrencia;
    int coluna_identificado;
    

} Token;

typedef struct {
    char lexema_analisado[MAX_MEMORIA_ANALISADOR];
    TiposTokens tipo;
} Simbolo;

Simbolo tabela_simbolos[MAX_TAMANHO_SIMBOLOS];
int indice_simbolo = 0;

void __analisarSimbolo(const char *lexema, TiposTokens tipo) {
    for (int i = 0; i < indice_simbolo; i++) {
        if (strcmp(tabela_simbolos[indice_simbolo].lexema_analisado, lexema) == 0) {
            return;
        }
    }

    strcpy(tabela_simbolos[indice_simbolo].lexema_analisado, lexema);
    tabela_simbolos[indice_simbolo].tipo = tipo;
    indice_simbolo++;
    
}

TiposTokens __getSimbolo(const char *lexema) {
    for (int i = 0; i < indice_simbolo; i++){
        if (strcmp(tabela_simbolos[i].lexema_analisado, lexema) == 0) {
            return tabela_simbolos[i].tipo;
        }
    }
    return TK_ERRO;
}

void __palavrasChaves(){
    const char *palavra_chave[] = {
        "program",
        "PROGRAM",
        "var",
        "VAR",
        "integer",
        "INTEGER",
        "real",
        "REAL",
        "begin",
        "BEGIN",
        "end",
        "END",
        "if",
        "IF",
        "then",
        "THEN",
        "else",
        "ELSE"
        "while",
        "WHILE"
        "do"
        "DO"
    };

    const char *simbolos[] = {
        "{",
        ",",
        "}",
        ";",
        "(",
        ")",
        "."
    };

    const char *operadores[] = {
        "=",
        ">=",
        "*",
        "<>",
        "<=",
        "/",
        ">",
        "+",
        "=",
        "<",
        "-"
    };

    const char *numeros[] = {
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9"
    };

    int total_palavras = sizeof(palavra_chave) / sizeof(palavra_chave[0]);
    int total_simbolos = sizeof(simbolos) / sizeof(simbolos[0]);
    int total_operadpres = sizeof(operadores) / sizeof(operadores[0]);
    int total_numeros = sizeof(numeros) / sizeof(numeros[0]);

    for (int i = 0; i < total_palavras; i++) {
       __analisarSimbolo(palavra_chave[i], TK_PALAVRA_CHAVE);
    }
    for (int j = 0; j < total_simbolos; j++) {
       
       __analisarSimbolo(simbolos[j], TK_SIMBOLO);
    }
    for (int k = 0; k < total_simbolos; k++) {
       
       __analisarSimbolo(operadores[k], TK_OPERADOR);
    }
    for (int l = 0; l < total_simbolos; l++) {
       
       __analisarSimbolo(numeros[l], TK_INTEGER);
    }
}


int __getOperadorandSimbolos(const char *operador) {
    if (strcmp(operador, "=") == 0) {
        return OP_EQ;
    }

    if (strcmp(operador, ">=") == 0) {
        return OP_GE;
    }

    if (strcmp(operador, "*") == 0) {
        return OP_MUL;
    }

    if (strcmp(operador, "<>") == 0) {
        return OP_NE;
    }

    if (strcmp(operador, "<=") == 0) {
        return OP_LE;
    }

    if (strcmp(operador, "/") == 0) {
        return OP_DIV;
    }

    if (strcmp(operador, ">") == 0) {
        return OP_GT;
    }

    if (strcmp(operador, "+") == 0) {
        return OP_AD;
    }

    if (strcmp(operador, ":=") == 0) {
        return OP_ASS;
    }

    if (strcmp(operador, "<") == 0) {
        return OP_LT;
    }

    if (strcmp(operador, "-") == 0) {
        return OP_MIN;
    }
    if (strcmp(operador, ":") == 0) {
        return TK_ATT_TIP_VAR;
    }
    if (strcmp(operador, "{") == 0) {
        return SMB_OBC;
    }
    if (strcmp(operador, ",") == 0) {
        return SMB_COM;
    }

    if (strcmp(operador, "}") == 0) {
        return SMB_CBC;
    }

    if (strcmp(operador, ";") == 0) {
        return SMB_SEM;
    }

    if (strcmp(operador, "(") == 0) {
        return SMB_OPA;
    }

    if (strcmp(operador, ")") == 0) {
        return SMB_CPA;
    }
    
    return TK_ERROR;
}

void __displayTokens(FILE *arquivo_destino, Token string){
    const char *tipo_analisado;
    int codigo;

    switch (string.tipo)
    {
    case TK_PALAVRA_CHAVE:
        tipo_analisado = "PALAVRA-CHAVE";
        codigo = TK_RESERVADO;
        break;

    case TK_IDENTIFICADOR:
        tipo_analisado = "IDENTIFICADOR";
        codigo = TK_ID;
        break;

    case TK_OPERADOR:
        tipo_analisado = "OPERADOR ";
        
        break;

    case TK_SIMBOLO:
        tipo_analisado = "SIMBOLO ";
        break;

    case TK_INTEGER:
        tipo_analisado = "INTEGER ";
        codigo = TK_INT;
        break;

    case TK_REALS:
        tipo_analisado = "REAL ";
        codigo = TK_REAL;
        break;
    
    default:
        codigo = TK_ERROR;
        break;
    }


    fprintf(arquivo_destino, "|TOKEN: %s | TOKEN ANALISADO: %s | CODIGO: %d | LINHA DE OCORRENCIA: %d | COLUNA: %d |\n\n", tipo_analisado, string.tamanho, codigo, string.linha_ocorrencia, string.coluna_identificado);
}

void __displaySimbolos(FILE *arquivo_destino) {
    fprintf(arquivo_destino, "\n+--------------------------TABELA ANALISE--------------------------+\n");
    for (int i = 0; i <indice_simbolo; i++) {
            if (strcmp("{", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, SMB_OBC);
            }
            else if (strcmp(",", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, SMB_COM);
            }
            else if (strcmp("}", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, SMB_CBC);
            }
            else if (strcmp(";", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, SMB_SEM);
            }
            else if (strcmp("(", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, SMB_OPA);
            }
            else if (strcmp(")", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, SMB_CPA);
            }
            else if (strcmp("program", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("PROGRAM", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("var", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("VAR", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("integer", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("INTEGER", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("real", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("REAL", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("begin", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("BEGIN", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("end", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("END", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("if", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("IF", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("then", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("THEN", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("end", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("END", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("else", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("ELSE", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("while", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("WHILE", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("do", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("DO", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_RESERVADO);
            }
            else if (strcmp("0", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("1", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("2", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("3", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("4", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("5", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("6", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("7", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("8", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else if (strcmp("9", tabela_simbolos[i].lexema_analisado) == 0){
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_INT);
            }
            else {
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, TK_ID);
            }
    }
    
}

void __analisadorLexico(const char *arquivo_origem, const char *arquivo_destino) {
    FILE *file = fopen(arquivo_origem, "r");

    if (!arquivo_origem) {
        perror("ERRO: O ARQUIVO INFORMADO NAO FOI LOCALIZADO, REVEJA SEUS PARAMETROS.");
        return;
    }

    FILE *file_destino = fopen(arquivo_destino, "w");

    if (!arquivo_destino) {
        perror("ERRO: O ARQUIVO INFORMADO NAO FOI LOCALIZADO, REVEJA SEUS PARAMETROS.");
        fclose(file);
        return;
    }

    char ch;
    char buffer[MAX_MEMORIA_ANALISADOR];
    int indice_buffer = 0;
    int linha = 1;
    int coluna = 1;
    int abriu_parenteses = 0;
    int linha_abertura_parenteses = 0;
    int coluna_abertura_parenteses = 0;
    int abriu_chaves = 0;
    int linha_abertura_chaves = 0;    
    int coluna_abertura_chaves = 0;

    while ((ch = fgetc(file)) != EOF) {

        if (isspace(ch)) {
            if (ch == '\n') {
                linha++;
                coluna = 1;
            }
            else {
                coluna++;
            }
            continue;
        }

        if (isalpha(ch)) {
            buffer[indice_buffer++] = ch;
            while (isalnum(ch = fgetc(file))) {
                buffer[indice_buffer++] = ch;
                coluna++;
            }

            buffer[indice_buffer] = '\0';
            indice_buffer = 0;
            ungetc(ch, file);

            Token token;
            token.linha_ocorrencia = linha;
            token.coluna_identificado = coluna - strlen(buffer);

            if (__getSimbolo(buffer) == TK_PALAVRA_CHAVE) {
                token.tipo = TK_PALAVRA_CHAVE;
            }
            else{
                token.tipo = TK_IDENTIFICADOR;
                __analisarSimbolo(buffer, TK_IDENTIFICADOR);
            }

            strcpy(token.tamanho, buffer);
            __displayTokens(file_destino, token);
        }
        else if (isdigit(ch)) {
            buffer[indice_buffer++] = ch;
            int eh_real = 0;
            while (isdigit(ch = fgetc(file)) || ch == '.') {
                if (ch == '.') {
                    eh_real = 1;
                }

                buffer[indice_buffer++] = ch;
                coluna++;
            }

            buffer[indice_buffer++] = '\0';
            indice_buffer = 0;
            ungetc(ch, file);

            Token token;

            token.linha_ocorrencia = linha;
            token.coluna_identificado = coluna - strlen(buffer);

            if (eh_real) {
                token.tipo = TK_REALS;
            }
            else {
                token.tipo = TK_INTEGER;
            }
            strcpy(token.tamanho, buffer);
            __displayTokens(file_destino, token);
        }

        else if(strchr("+-*/=<>:", ch)) { //strchr VAI PERCORRER UM PONTEIRO PROCUANDO A OCORRÊNCIA DO SIMBOLO
            buffer[indice_buffer++] = ch;
            char prox_caractere = fgetc(file);

            if ((ch == '<' && prox_caractere == '>') || (ch == '<' && prox_caractere == '=') || (ch == '>' && prox_caractere == '=') || (ch == ':' && prox_caractere == '=')) {
                buffer[indice_buffer++] = prox_caractere;
                coluna ++;
            }
            else {
                ungetc(prox_caractere, file);
            }

            buffer[indice_buffer++] = '\0';
            indice_buffer = 0;

            Token token;
            token.tipo = TK_OPERADOR;
            token.linha_ocorrencia = linha;
            token.coluna_identificado = coluna - strlen(buffer) + 1;
            strcpy(token.tamanho, buffer);

            int analisado = __getOperadorandSimbolos(token.tamanho);

            fprintf(file_destino, "|TOKEN: OPERADOR| TOKEN ANALISADO: %s | CODIGO:  %d| LINHA DE OCORRENCIA: %d | COLUNA: %d |\n\n", token.tamanho,__getOperadorandSimbolos(token.tamanho),  token.linha_ocorrencia, token.coluna_identificado);

            coluna++;
            
        }
        else if(strchr("{},;().", ch)) { //strchr VAI PERCORRER UM PONTEIRO PROCUANDO A OCORRÊNCIA DO SIMBOLO
            Token token;
            token.tipo = TK_SIMBOLO;
            token.linha_ocorrencia = linha;
            token.coluna_identificado = coluna;
            token.tamanho[0] = ch;
            token.tamanho[1] = '\0';
            
            fprintf(file_destino, "|TOKEN: SIMBOLO| TOKEN ANALISADO: %s | CODIGO:  %d| LINHA DE OCORRENCIA: %d | COLUNA: %d |\n\n", token.tamanho, __getOperadorandSimbolos(token.tamanho),  token.linha_ocorrencia, token.coluna_identificado);

            coluna++;

            if (ch == '(') {
                abriu_parenteses++;
                linha_abertura_parenteses = token.linha_ocorrencia;
                coluna_abertura_parenteses = token.coluna_identificado;
            }

            if (ch == ')') {
                if (abriu_parenteses == 1) {
                    abriu_parenteses--;
                }
                else if (abriu_parenteses > 1){
                    fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: O USUARIO ABRIU PARENTESES, MAS NAO REALIZOU O FECHAMENTO | LINHA %d | COLUNA: %d |\n\n", linha, coluna);
                }
            }
                
            if (ch == '{') {
                abriu_chaves++;
                linha_abertura_chaves = token.linha_ocorrencia;
                coluna_abertura_chaves = token.coluna_identificado;
            }
            else if (ch == '}') {
                if (abriu_chaves > 0) {
                    abriu_chaves--;
                }
                else {
                    fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: O USUARIO ABRIU CHAVES, MAS NAO REALIZOU O FECHAMENTO | LINHA %d | COLUNA: %d |\n\n", linha, coluna);
                }
            }
        }
        else {
            Token token;
            token.tipo = TK_ERRO;
            token.tamanho[0] = ch;
            token.tamanho[1] = '\0';
            token.linha_ocorrencia = linha;
            token.coluna_identificado = coluna;

            fprintf(file_destino, "\n|ERRO LEXICO: O CARACTERE INFORMADO NAO E RECONHECIDO PELA LINGUAGEM (%c) | LINHA: %d | COLUNA: %d|\n\n", ch, linha, coluna);
            coluna++;

            fprintf(file_destino, "\n|ERRO AO REALIZAR A ANÁLISE LÉXICA: %c NÃO FOI RECONHECIDO|\n\n", ch);
            return;
            
        }

    }

    if (abriu_parenteses > 0) {

        fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: %d PAREENTESES NAO FOI FECHADO CORRETAMENTE|\n\n", abriu_parenteses);
        fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: O USUARIO ABRIU PARENTESES, MAS NAO REALIZOU O FECHAMENTO | LINHA %d | COLUNA: %d |\n\n", linha_abertura_parenteses, coluna_abertura_parenteses);
        return;
    }
    if (abriu_chaves > 0) {
        fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: %d CHAVES NAO FOI FECHADO CORRETAMENTE|\n\n", abriu_chaves);
        fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: O USUARIO ABRIU CHAVES, MAS NAO REALIZOU O FECHAMENTO | LINHA %d | COLUNA: %d |\n\n", linha_abertura_chaves, coluna_abertura_chaves);
        return;
    }

    __displaySimbolos(file_destino);

    fclose(file_destino);
    fclose(file); //FECHAR O ARQUIVO APOS A REALIZAÇÃO DE TODO O PROCESSO
}

int main() {
    __lerArquivo("exemplo1.txt");
    printf("\n");
    __palavrasChaves();
    
    // GERAR ARQIVO .txt
    __analisadorLexico("exemplo1.txt", "lexico.txt");

    // GERAR ARQUIVO .lex
    __analisadorLexico("exemplo1.txt", "analise.lex");

    return 0;
}