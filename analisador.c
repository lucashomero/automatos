#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tokens.h"

//Definindo os tamanhos máximos da memoria e dos simbolos
#define MAX_MEMORIA_ANALISADOR 100
#define MAX_TAMANHO_SIMBOLOS 100


//função para fazer a leitura de um arquivo que será passado como parametro
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

// Criando um enum para classificar os tokens
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

// Criando um struct para armazenar informações sobre cada token
typedef struct {
    TiposTokens tipo;
    char tamanho[MAX_MEMORIA_ANALISADOR];
    int linha_ocorrencia;
    int coluna_identificado;
} Token;

// Criando uma struct para armazenar informaçoes sobre simbolos
typedef struct {
    char lexema_analisado[MAX_MEMORIA_ANALISADOR];
    TiposTokens tipo;
} Simbolo;

// Criando um array do tipo Simbolo que serve como tabela de simbolos
Simbolo tabela_simbolos[MAX_TAMANHO_SIMBOLOS];
int indice_simbolo = 0; // essa variavel controla o o indice da tabela de simbolos

// Função responsável por verificar e, se necessário, adicionar um novo símbolo à tabela de simbolos
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

// Função responsavel por buscar um simbolo na tabela de simbolos a partir do seu lexama
// retorna o tipo de token correspondente a ela e se o simbolo nao for encontrado retorna o tipo TK_ERRO
TiposTokens __getSimbolo(const char *lexema) {
    for (int i = 0; i < indice_simbolo; i++){
        if (strcmp(tabela_simbolos[i].lexema_analisado, lexema) == 0) {
            return tabela_simbolos[i].tipo;
        }
    }
    return TK_ERRO;
}

// Função responsavel por inserir na tabela de simbolos as palavras chave associadas ao token TK_PALAVRA_CHAVE
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
       
       __analisarSimbolo(palavra_chave[i], TK_PALAVRA_CHAVE);
    }

}

// Função responsável por identificar e categorizar operadores e símbolos (como parênteses e chaves) com base no lexema passado como parametro
// Se o lexema não for reconhecido a função retorna TK_ERROR
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


// Função responsável por exibir os tokens analisados pelo analisador léxico, formatando a saída em um arquivo de destino
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

// Função  responsável por exibir a tabela de símbolos, que armazena lexemas e seus tipos formatando a saida em um arquivo 
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

// Principal função 
// lê um arquivo de origem, identifica tokens e escreve os resultados em um arquivo de destino
void __analisadorLexico(const char *arquivo_origem, const char *arquivo_destino) {
    FILE *file = fopen(arquivo_origem, "r");

    // Verifica se o arquivo de origem foi aberto corretamente
    if (!arquivo_origem) {
        perror("ERRO: O ARQUIVO INFORMADO NAO FOI LOCALIZADO, REVEJA SEUS PARAMETROS.");
        return;
    }

    FILE *file_destino = fopen(arquivo_destino, "w");

    // Verifica se o arquivo de destino foi aberto corretamente
    if (!arquivo_destino) {
        perror("ERRO: O ARQUIVO INFORMADO NAO FOI LOCALIZADO, REVEJA SEUS PARAMETROS.");
        fclose(file);
        return;
    }

    // Variaveis importantes para a execução do código
    char ch; // Armazena o caractere lido do arquivo
    char buffer[MAX_MEMORIA_ANALISADOR]; // Buffer para armazenar tokens
    int indice_buffer = 0; // Índice do buffer
    int linha = 1;  // Contador de linhas
    int coluna = 1;  // Contador de colunas
    int abriu_parenteses = 0;  // Contador de parênteses abertos
    int abriu_chaves = 0;  // Contador de chaves abertas

    // Loop principal para ler cada caractere do arquivo
    while ((ch = fgetc(file)) != EOF) {

        // Ignora espaços em branco e atualiza linha/coluna
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

        // Lida com identificadores e palavras-chave
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
        // Lida com números (inteiros e reais)
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
        // Lida com strings
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
        // Lida com operadores
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
        // Lida com símbolos
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
        // Lida com caracteres não reconhecidos
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

    // Verificação de parênteses e chaves não fechados
    if (abriu_parenteses > 0) {
        fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: %d PAREENTESES NAO FOI FECHADO CORRETAMENTE|\n\n", abriu_parenteses);
    }
    if (abriu_chaves > 0) {
        fprintf(file_destino, "\n|ERRO LEXICO IDENTIFICADO: %d CHAVES NAO FOI FECHADO CORRETAMENTE|\n\n", abriu_chaves);
    }

    // Exibe a tabela de símbolos
    __displaySimbolos(file_destino);

    // Fecha os arquivos
    fclose(file_destino);
    fclose(file); 
}

int main() {
    __lerArquivo("exemplo1.txt");
    printf("\n");
    __palavrasChaves();
    __analisadorLexico("exemplo1.txt", "lexico.txt");

    return 0;
}
