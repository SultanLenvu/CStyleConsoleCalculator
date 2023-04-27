#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#define EXPRESSION_MAX_LENGTH 100
#define FUNCTION_NAME_MAX_LENGTH 3

typedef enum 
{
    NoError,
    LexerInputError,
    LexerParseError,
    ExitCommand,
    UnprocessableError
} ReturnStatus;

// Тип токена
typedef enum
{
    TokenNumber,
    TokenFunction,
    TokenOperator,
    TokenLeftParenthesis,
    TokenRigthParenthesis,
    TokenComma
} TokenType;

// Структура одного токена
typedef struct
{
    char* value; // Значение токена
    int length; // Длина значения
    TokenType type; // Тип токена
} TokenCore;

// Шаблоны функции
typedef void (*IFunction)(void);
typedef ReturnStatus (*IReturnStatusFunction)(void);
typedef double (*ICalculateTokenTree)(void);
typedef void (*IBuildTokenTree)(TokenCore**);

#endif // COMMON_TYPES_H
