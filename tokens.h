// TABELA PARA A IDENTIFICACAO DOS OPERADORES, SIMBOLOS, TIPOS, ERRO, IDENTIFICADOR

// ÁREA DE OPERADORES DA LINGUAGEM
#define OP_EQ                   200 // =
#define OP_GE                   201 // >=
#define OP_MUL                  202 // *
#define OP_NE                   203 // <.
#define OP_LE                   204 // <=
#define OP_DIV                  205 // /
#define OP_GT                   206 // >
#define OP_AD                   207 // +
#define OP_ASS                  208 // :=
#define OP_LT                   209 // <
#define OP_MIN                  210 // -


// ÁREA DE SÍMBOLOS DA LINGAGEM
#define SMB_OBC                 211 // {
#define SMB_COM                 212 // ,
#define SMB_CBC                 213 // }
#define SMB_SEM                 214 // ;
#define SMB_OPA                 215 // (
#define SMB_CPA                 216 // )


// ÁREA DE TIPOS DA LINGUAGEM
#define TK_INT                 217 // integer
#define TK_REAL                218 // real


// ÁREA DE IDENTIFICADOR


// ÁREA DE PALAVRA RESERVADA DA LINGUAGEM
#define TK_RESERVADO            219 // program, var, integer, real, begin, end, if, then, else, while, do

// ÁREA DE IDENTIFICADORES
#define TK_ID                   220 // variavel ex: var a 

// ERRO PARA O CASO DE NENHUM DOS SIMBOLOS FOREM RECONHECIDOS

#define TK_ERROR                221 // ERRO

// ATRIBUIR TIP DA VARIAVEL
#define TK_ATT_TIP_VAR          222 // ATRIBUIR O TIPO DA VARIÁVEL, :