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
    TK_STRING,
    TK_ERRO

    // TOKENS DE OPERADORES
/*
    op_eq = '=',
    op_ge = '>=',
    op_mul = '*',
    op_ne = '<>',
    op_le = '<=',
    op_div = '/',
    op_gt = '>',
    op_ad = '+',
    op_ass = '=',
    op_lt = '<',
    op_min = '-',

    // TOKENS DE SIMBOLOS

    smb_obc = '{',
    smb_com = ',',
    smb_cbc = '}',
    smb_sem = ';',
    smb_opa = '(',
    smb_cpa = ')',
*/
    // TOKEN PARA PALAVRA-CHAVE(RESERVADO)


    // TOKEN PARA IDENFIFICADORES


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
        "var",
        "integer",
        "real",
        "begin",
        "end",
        "if",
        "then",
        "else",
        "while",
        "do"
    };

    int total_palavras = sizeof(palavra_chave) / sizeof(palavra_chave[0]);

    for (int i = 0; i < total_palavras; i++) {
        /*if (strcmp(string, palavra_chave[i]) == 0) {
            return 1;
        }*/
       __analisarSimbolo(palavra_chave[i], TK_PALAVRA_CHAVE);
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
    fprintf(arquivo_destino, "\nTABELA SIMBOLOS\n");
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
            else {
                fprintf(arquivo_destino, "|LEXEMA: %s | TIPO: %d |\n", tabela_simbolos[i].lexema_analisado, tabela_simbolos[i].tipo);
            }
    }
    
}


/*
int __operadores(const char *string) {
    const char *operador[] = {
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
    int total_operadores = sizeof(operador) / sizeof(operador[0]);

    for (int i = 0; i < total_operadores; i++) {
        if (strcmp(string, total_operadores[i] == 0)){
            return 1;
        }
        
    }
    


}
*/

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
    int abriu_chaves = 0;

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
        else if (ch == '"') {
            buffer[indice_buffer++] = ch;
            int ocorrencia_abertura = coluna;

            while ((ch = fgetc(file)) != EOF && ch != '"') {
                if (ch == '\n') {
                    fprintf(arquivo_destino, "\n|ERRO LEXICO IDENTIFICADO, STRING NAO FOI FECHADA CORRETAMENTE | LINHA: %d | COLUNA: %d |\n\n", linha, ocorrencia_abertura);
                    linha++;
                    coluna = 1;
                    indice_buffer = 0;
                    break;
                }

                buffer[indice_buffer++] = ch;
                coluna++;
            }
            if (ch == '"') {
                buffer[indice_buffer++] = ch;
                buffer[indice_buffer] = '\0';

                Token token;
                token.tipo = TK_STRING;
                token.linha_ocorrencia = linha;
                token.coluna_identificado = ocorrencia_abertura;
                strcpy(token.tamanho, buffer);
                __displayTokens(arquivo_destino, token);
                coluna++;
            }
            else {
                indice_buffer = 0;
            }
        }
        else if(strchr("+-*/=<>:", ch)) { //strchr VAI PERCORRER UM PONTEIRO PROCUANDO A OCORRÊNCIA DO SIMBOLO
            /*Token token;
            token.tipo = TK_OPERADOR;
            token.tamanho[0] = ch;
            token.tamanho[1] = '\0';
            __displayTokens(file_destino, token);*/

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
            }
            else if (ch == ')') {
                if (abriu_parenteses > 0) {
                    abriu_parenteses--;
                }
            else {
                    fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: O USUARIO ABRIU PARENTESES, MAS NAO REALIZOU O FECHAMENTO | LINHA %d | COLUNA: %d |\n\n", linha, coluna);
                }
            }
                
            if (ch == '{') {
                abriu_chaves++;
                if (ch == '}') {
                    if (abriu_chaves > 0) {
                        abriu_chaves--;
                    }
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
            
            
        }

    }

    if (abriu_parenteses > 0) {
        fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: %d PAREENTESES NAO FOI FECHADO CORRETAMENTE|\n\n", abriu_parenteses);
    }
    if (abriu_chaves > 0) {
        fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: %d CHAVES NAO FOI FECHADO CORRETAMENTE|\n\n", abriu_chaves);
    }

    __displaySimbolos(file_destino);

    fclose(file_destino);
    fclose(file); //FECHAR O ARQUIVO APOS A REALIZAÇÃO DE TODO O PROCESSO
}

int main() {
    __lerArquivo("exemplo1.txt");
    printf("\n");
    __palavrasChaves();
    __analisadorLexico("exemplo1.txt", "lexico.txt");

    return 0;
}