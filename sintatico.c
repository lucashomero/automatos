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

    while (*input == ' ' || *input == '\t' || *input == '\n') {
        if (*input == '\n') line++;
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
        token.lexema[length++] = '\0';

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
        case '+': token.type = OP_AD; break;
        case '-': token.type = OP_MIN; break;
        case '*': token.type = OP_MUL; break;
        case '/': token.type = OP_DIV; break;
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
        exit(1);
    }
}

//FUNÇÕES PARA PRECONHECIMENTO DE EXPRESSÕES

void __parseBloco(); //PROTÓTIPO
void __parseParteDeclaracoes(); //PROTÓTIPO
void __parseDeclaracaoVariaveis(); //PROTÓTIPO
void __parseComandoComposto(); //PROTÓTIPO
void __parseComando(); //PROTÓTIPO
void __parseExpressao(); //PROTÓTIPO
void __parseTermo(); //PROTÓTIPO
void __parseFactor(); //PROTÓTIPO

void __parse() {
    __advance();

    if (currentToken.type == PROGRAM) {
        __advance(); //OBTÉM O PRIMEIRO TOKEN
        __casaToken(IDENTIFICADOR);
        __casaToken(SMB_SEM);

        printf("REGRA: <programa>::= program <identificador> ; <bloco> .\n\n");

        __parseBloco();
        __casaToken(SMB_DOT); //CONOSME O PONTO FINAL
    } 
    else {
        printf("%d TOKEN NAO ESPERADO [%s].\n", line, currentToken.lexema);
        exit(1);
    }

    if (currentToken.type != END) {
        printf("%d FIM DE ARQUIVO NAO ESPERADO.\n", line);
        exit(1);
    }
}

void __parseBloco() {
    printf("REGRA: <bloco> ::= <parte de declaracoes de variaveis> <comando composto>\n\n");

    __parseParteDeclaracoes();

    if (currentToken.type == BEGIN) {
        __parseComandoComposto();

    }
    else {
        printf("%d TOKEN NAO ESPERADO [%s].\n", line, currentToken.lexema);
        exit(1);
    }
        
}

void __parseParteDeclaracoes() {
    if (currentToken.type == VAR) {
        printf("REGRA: <parte de declaracoes de variaveis> ::= var <declaracao de variaveis> { ; <declaracao de variaveis> } ;\n\n");

        __advance(); //CONSOME 'VAR'
        __parseDeclaracaoVariaveis();

        while (currentToken.type == SMB_SEM) {
            __advance(); //CONSOME ';'

            if (currentToken.type == IDENTIFICADOR) {
                __parseDeclaracaoVariaveis();
            }
            else {
                break;
            }
        }
        //__casaToken(SMB_SEM); //ESPERA ';' AO FINAL DA DECLARAÇÃO
    }
}

void __parseDeclaracaoVariaveis() {
    printf("REGRA: <declaracao de variaveis> ::= <lista de identificadores> : <tipo>\n\n");

    if (currentToken.type == IDENTIFICADOR){
        while (currentToken.type == IDENTIFICADOR) {
            __advance(); //AVANÇA PARA O PRÓXIMO IDENTIFICADOR OU VÍRGULA

            if (currentToken.type == SMB_COM) {
                __advance(); //CONSOME A VÍRGULA
            }
            else {
                break;
            }
        }
    }
    __casaToken(COMMA); //VERIFICA O ':'

    if (currentToken.type == INTEGER || currentToken.type == REAL) {
        __advance(); //CONSOME VARIÁVEIS DO TIPO INTERGER OU REAL
    }
    else {
        printf("%d TOKEN NAO ESPERADO [%s].\n", line, currentToken.lexema);
        exit(1);
    }
}

void __parseComandoComposto() {
    if (currentToken.type == BEGIN) {
        printf("REGRA: <comando composto> ::= begin <comando> ; { <comando> ; } end\n\n");

        __advance(); //CONSOME 'BEGIN'
        __parseComando();

        while (currentToken.type == SMB_SEM) {
            __advance(); //CONSOME ';'
            if (currentToken.type == END) {
                break;
            }
            __parseComando();
        }
        __casaToken(END);
    }
    else {
        printf("%d TOKEN NAO ESPERADO [%s].\n", line, currentToken.lexema);
        exit(1);
    }
}

void __parseComando() {
    if (currentToken.type == IDENTIFICADOR) {
        printf("REGRA: <comando> ::= <atribuicao>\n\n");

        __advance();
        __casaToken(OP_ASS);
        __parseExpressao();
    }
    else if (currentToken.type == IF) {
        printf("REGRA: <comando> ::= <comando condicional>\n\n");

        __advance();
        __parseExpressao(); //PROCESSA A EXPRESSÃO APÓS O IF
        __casaToken(THEN);
        __parseComando();

        if (currentToken.type == ELSE) {
            __advance();
            __parseComando();
        }
    }
    else if (currentToken.type == WHILE) {
        printf("REGRA: <comando> ::= <comando repetitivo>\n\n");

        __advance();
        __parseExpressao();
        __casaToken(DO);
        __parseComando();
    }
    else if (currentToken.type == BEGIN) {
        __parseComandoComposto();
    }
    else {
        printf("%d TOKEN NAO ESPERADO [%s].\n", line, currentToken.lexema);
        exit(1);
    }
}

void __parseExpressao() {
    __parseTermo();

    while (currentToken.type == OP_AD || currentToken.type == OP_MIN || currentToken.type == OP_EQ || currentToken.type == OP_GE || currentToken.type == OP_LE || currentToken.type == OP_LT || currentToken.type == OP_GT || currentToken.type == OP_NE) {
        __advance();
        __parseTermo();
    }
}

void __parseTermo() {
    __parseFactor();

    while (currentToken.type == OP_MUL || currentToken.type == OP_DIV) {
        __advance();
        __parseFactor();
    }
}

void __parseFactor() {
    if (currentToken.type == IDENTIFICADOR) {
        __advance();
    }
    else if (currentToken.type == NUM) {
        __advance();
    }
    else if (currentToken.type == SMB_OPA) {
        __advance(); //CONSOME '('
        __parseExpressao(); //AVALIA A EXPRESSÃO DENTRO DOS PARÊNTESES
        if (currentToken.type != SMB_CPA) {
            printf("%d TOKEN NAO ESPERADO [%s]. ESPERADO: )\n", line, currentToken.lexema);
            exit(1);
        }
        __casaToken(SMB_CPA); //COSNOME ')'
    }
    else {
        printf("%d TOKEN NAO ESPERADO [%s].\n", line, currentToken.lexema);
        exit(1);
    }
    
}

int main() {
    numTokens = 0;
    line = 1;

    input = "\n program example;\n var x, y: integer;\n begin x := (1 + 2); if x > y then y := y + 1 else y := y - 1;\n while y < 10 do y := y + 1;\n end.";
    //input = "program teste; var a: integer; begin a := 5; if a < 5 then a := 1 else a := 6; while a = 5 do a := 1; end.";
    printf("ANALISANDO: %s\n", input);

    __parse(); //INICIA O PROCESSO DE ANALISE (PARSING)

    printf("ANALISE REALIZADA COM SUCESSO.");

    return 0;
}
