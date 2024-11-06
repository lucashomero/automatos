#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

//DEFININDO TOKENS

#define PROGRAM 250
#define VAR 251
#define INTEGER 252
#define NUM 265
#define REAL 253
#define BEGIN 254
#define END 0
#define IF 256
#define THEN 257
#define ELSE 258
#define WHILE 259
#define DO 260
#define OP_EQ '='
#define OP_GE 261
#define OP_MUL '*'
#define OP_NE 262
#define OP_LE 263
#define OP_DIV '/'
#define OP_GT '>'
#define OP_AD '+'
#define OP_ASS 264
#define OP_LT '<'
#define OP_MIN '-'
#define SMB_OBC '{'
#define SMB_COM ','
#define SMB_CBC '}'
#define SMB_SEM ';'
#define SMB_OPA '('
#define SMB_CPA ')'
#define COMMA ':'
#define SMB_DOT 266
#define TK_ERROR 267
#define IDENTIFICADOR  268

typedef struct {
    int type;
    double value; //ARMAZENAR NÚMEROS
    char lexema[100];
} Token;

//VARIÁVEL GLOBAL PARA O TOKEN ATUAL

Token currentToken;
char *input;
int numTokens;
int line;

//FUNÇÃO LÉXICA PARA OBTER O PRÓXIMO TOKEN

Token getToken() {
    Token token;

    while (*input == ' ' || *input == '\t') {
        input++;
    }

    if (*input == '\0') {
        token.type = END;

        return token;
    }

    if (isdigit(*input)) {
        token.type = NUM;
        token.value = strtod(input, &input); //LÊ UM NÚMERO

        return token;
    }

    if (isalpha(*input)) {
        int length = 0;

        while (isalnum(*input)) {
            token.lexema[length++] = *input++;
        }

        if (strcmp(token.lexema, "program") == 0) token.type = PROGRAM;
        else if (strcmp(token.lexema, "var") == 0) token.type = VAR;
        else if (strcmp(token.lexema, "begin") == 0) token.type = BEGIN;
        else if (strcmp(token.lexema, "end") == 0) token.type = END;
        else if (strcmp(token.lexema, "if") == 0) token.type = IF;
        else if (strcmp(token.lexema, "then") == 0) token.type = THEN;
        else if (strcmp(token.lexema, "else") == 0) token.type = ELSE;
        else if (strcmp(token.lexema, "while") == 0) token.type = WHILE;
        else if (strcmp(token.lexema, "do") == 0) token.type = DO;
        else if (strcmp(token.lexema, "integer") == 0) token.type = INTEGER;
        else if (strcmp(token.lexema, "real") == 0) token.type = REAL;
        else token.type = IDENTIFICADOR;

        return token;
    }

    switch (*input) {
        case '(': token.type = SMB_OPA;break;
        case ')': token.type = SMB_CPA; break;
        case '{':token.type = SMB_OBC;break;
        case '}':token.type = SMB_CBC;break;
        case '=': token.type = OP_EQ; break;
        case '>':
                if (*(input + 1) == '=') {
                    input++;
                    token.type = OP_GE;
                } else {
                    token.type = OP_GT;
                }
                break;
            case '<':
                if (*(input + 1) == '=') {
                    input++;
                    token.type = OP_LE;
                } else {
                    token.type = OP_LT;
                }
                break;
            case ':':
                if (*(input + 1) == '=') {
                    token.type = OP_ASS;
                    input += 2;
                    return token;
                } else {
                    token.type = COMMA;
                }
                break;
        case ';': token.type = SMB_SEM; break;
        case '.': token.type = SMB_DOT; break;
        case ',': token.type = SMB_COM; break;
        default:
        printf("ERRO: %d: TOKEN NAO ESPERADO [%c].\n", line, *input);
            token.type = TK_ERROR;
            exit(1);
            break;
    }

    input++; //AVANÇA PARA O PRÓXIMO CARACTERE

    return token;
}

//FUNÇÃO PARA AVANÇAR O TOKEN ATUAL

void __advance() {
    currentToken = getToken();
    numTokens++;
}

//FUNÇÃO PARA AVANÇAR O TOKEN ATUAL

void __casaToken(int tkExpected) {
    if (currentToken.type == tkExpected) {
        __advance();
    }
    else {
        printf("%d TOKEN NAO ESPERADO [%s].", line, currentToken.lexema);
    }
}

//FUNÇÕES PARA PRECONHECIMENTO DE EXPRESSÕES

void __parseFactor() {
    if (currentToken.type == PROGRAM) {
        __advance();

        if (currentToken.type != IDENTIFICADOR) {
            printf("%d TOKEN NAO ESPERADO [%s].", line, currentToken.lexema);
            exit(1);
        }
        else {
            __advance(); //CONSOME IDENTIFICADOR

            if (currentToken.type != SMB_SEM) {
                printf("%d TOKEN NAO ESPERADO [%s].", line, currentToken.lexema);
                exit(1);
            }
            else {
                __advance(); //CONSOME ';'

                if (currentToken.type != VAR) {
                    printf("%d TOKEN NAO ESPERADO [%s].", line, currentToken.lexema);
                    exit(1);
                }
                __advance(); //COSNSOME 'VAR'
                printf("REGRA: <programa>::= program <identificador> ; <bloco> .");
            }
        }
    }
    else if (currentToken.type == VAR) {
        __advance();
        printf("REGRA: var\n");
    }
    else if (currentToken.type == INTEGER) {
        __advance();
        printf("REGRA: <declaração de variáveis> ::=<lista de identificadores> : <tipo>\n");
    }
    else if (currentToken.type == BEGIN) {
       __advance();
    }
    else if (currentToken.type == SMB_OPA) {
        __advance(); //CONSOME '('

        if (currentToken.type != SMB_CPA) {
            printf("%d TOKEN NAO ESPERADO [%s].", line, currentToken.lexema);
            exit(1);
        }
        __advance(); //CONSOME ')'
        printf("REGRA: <termo> ::= <fator>\n");
    }
    else if (currentToken.type == SMB_OBC) {
         __advance(); //CONSOME '{'

        if (currentToken.type != SMB_CBC) {
            printf("%d TOKEN NAO ESPERADO [%s].", line, currentToken.lexema);
            exit(1);
        }
        __advance(); //CONSOME '}'
        printf("REGRA: <termo> ::= <fator>\n");
    }
    
}

//FUNÇÃO PRINCIPAL DO ANALISADOR SINTÁTICO

void __parse() {
    __advance(); //OBTÉM O PRIMEIRO TOKEN
    __parseFactor();

    if (currentToken.type != END) {
        printf("%d FIM DE ARQUIVO NAO ESPERADO.", line);
        exit(1);
    }
}

int main() {
    numTokens = 0;

    input = "{}";
    printf("ANALISANDO: %s\n", input);

    __parse(); //INICIA O PROCESSO DE ANALISE (PARSING)

    printf("ANALISE REALIZADA COM SUCESSO.");

    return 0;
}
