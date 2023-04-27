#ifndef LEXER_H
#define LEXER_H

#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "stdbool.h"
#include "conio.h"

#include "common_types.h"

// Ожидаемый токен
typedef enum
{
    NextAny,
    NextNumber,
    NextOperator
} ExpectedToken;

// Приоритеты операндов
typedef enum
{
    Priority1, // Функции
    Priority2, // Сложение/вычитание
    Priority3  // Умножение/деление
} OperationPriority;

// Ядро лексера
typedef struct
{
    char string[EXPRESSION_MAX_LENGTH];
    char* iterator;
    int stringLength;
    TokenCore** tokens;
    int tokenCount;

    IReturnStatusFunction input;
    IReturnStatusFunction parse;
    IFunction clear;
    IFunction printTokens;
    IFunction convToPrefNot;
} LexerCore;

// Лексер (он такой один)
extern LexerCore Lexer;

// Конструктор лексера
void initLexer(void);

#endif // LEXER_H
