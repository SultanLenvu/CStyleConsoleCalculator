#include "lexer.h"

// Лексер 
LexerCore Lexer;

/* Объявления */
//==============================================================================
// Внешние методы 
static ReturnStatus input(void);
static ReturnStatus parse(void);
static void clear(void);
static void convToPrefNot(void);
static void printTokens(void);

// Внутренние методы
static TokenCore* newToken(char* value, int length, TokenType type);
static void addToken(TokenCore* token);
static OperationPriority getOperatorPriority(const char chr);
static void clearTokens(void);
static void reverseTokensOrder();

static bool isOperator(const char chr);
static bool isComma(const char chr);
static int isFunction(const char* chr);
static int isNumber(const char* str);
static bool isLeftParentheses(const char chr);
static bool isRigthParentheses(const char chr);
//==============================================================================

/* Реализации */
//==============================================================================
// Конструктор лексера 
void initLexer()
{
    Lexer.iterator = Lexer.string;
    Lexer.stringLength = 0;

    Lexer.tokens = NULL;
    Lexer.tokenCount = 0;

    Lexer.input = input;
    Lexer.parse = parse;
    Lexer.clear = clear;
    Lexer.printTokens = printTokens;
    Lexer.convToPrefNot = convToPrefNot;
}

// Пользовательский ввод
static ReturnStatus input(void)
{
    printf("Input expression: \n");
    fgets(Lexer.string, EXPRESSION_MAX_LENGTH, stdin);
    Lexer.stringLength = strlen(Lexer.string);

    if  (strncmp(Lexer.string, "quit", 4) == 0)
        return ExitCommand;
    else
        return NoError;
}

// Разбор пользовательского выражения
static ReturnStatus parse(void)
{
    TokenCore* token; // Текущий токен для сохранения
    ExpectedToken expected = NextNumber; // Ожидаемый токен
    Lexer.iterator = Lexer.string; // Сбрасываем итератор

    int leftParCount = 0; // Количество левых скобок
    int rigthParCount = 0; // Количество правых скобок
    int valueSize = 0; // Размер текущего токена

    while (Lexer.iterator - Lexer.string < Lexer.stringLength)
    {
        if ((isalpha(*Lexer.iterator)) && (expected == NextNumber))
        { // Любая буква должна быть частью имени одной из реализуемых функции
            valueSize = isFunction(Lexer.iterator);
            if (valueSize)
            {
                token = newToken(Lexer.iterator, valueSize, TokenFunction);
                addToken(token);
                expected = NextNumber;
                Lexer.iterator += valueSize;
            }
            else
                return LexerParseError;
        }
        else if ((isdigit(*Lexer.iterator)) && (expected == NextNumber))
        { // Проверяем возможность преобразования в число
            valueSize = isNumber(Lexer.iterator);
            if (valueSize > 0)
            {
                token = newToken(Lexer.iterator, valueSize, TokenNumber);
                addToken(token);
                expected = NextOperator;
                Lexer.iterator += valueSize;
            }
            else
                return LexerParseError;
        }
        else if ((isOperator(*Lexer.iterator)) && (expected == NextOperator))
        { // Проверка специальных символов, кодирующих реализуемые операторы
            token = newToken(Lexer.iterator, 1, TokenOperator);
            addToken(token);
            expected = NextNumber;
            Lexer.iterator++;
        }
        else if ((isLeftParentheses(*Lexer.iterator)) && (expected == NextNumber))
        {
            token = newToken(Lexer.iterator, 1, TokenLeftParenthesis);
            addToken(token);
            leftParCount++;
            Lexer.iterator++;

        }
        else if ((isRigthParentheses(*Lexer.iterator)) && (expected == NextOperator))
        {
            token = newToken(Lexer.iterator, 1, TokenRigthParenthesis);
            addToken(token);
            rigthParCount++;
            Lexer.iterator++;
        }
        else if (isspace(*Lexer.iterator))
        { // Пробелы пропускаем
            Lexer.iterator++;
        }
        else if (isComma(*Lexer.iterator) && (expected == NextOperator))
        {
            token = newToken(Lexer.iterator, 1, TokenComma);
            addToken(token);
            Lexer.iterator++;
            expected = NextNumber;
        }
        else
        {
            return LexerParseError;
        }
    }

    // Проверяем соотношение количества скобок
    if (leftParCount == rigthParCount)
        return NoError;
    else
        return LexerParseError;
}

// Сброс параметров лексера
static void clear(void)
{
    // Очищаем пользовательский ввод
    for (int i = 0; i < EXPRESSION_MAX_LENGTH; i++)
        Lexer.string[i] = 0x00;
    Lexer.stringLength = 0;
    Lexer.iterator = Lexer.string;

    // Очищаем токены
    clearTokens();
}

static void convToPrefNot(void)
{
    TokenCore** prefix = (TokenCore**)calloc(Lexer.tokenCount, sizeof(TokenCore*));
    TokenCore* stack[Lexer.tokenCount];
    int top = -1;
    int i, j;
    int len = Lexer.tokenCount;

    for (i = len - 1, j = 0; i >= 0; i--)
    {
        if (Lexer.tokens[i]->type == TokenNumber/*isalnum(infix[i])*/ ||
            Lexer.tokens[i]->type == TokenFunction)
            prefix[j++] = Lexer.tokens[i];
        else if (Lexer.tokens[i]->type == TokenComma)
        {
            free(Lexer.tokens[i]);
            Lexer.tokenCount--;
        }
        else if (Lexer.tokens[i]->type == TokenRigthParenthesis /*infix[i] == ')'*/)
        {
            stack[++top] = Lexer.tokens[i];
            Lexer.tokenCount--;
        }
        else if (Lexer.tokens[i]->type == TokenLeftParenthesis/*infix[i] == '('*/)
        {
            while (stack[top]->type != TokenRigthParenthesis)
                prefix[j++] = stack[top--];

            // pop ')'
            free(stack[top]);
            top--;

            free(Lexer.tokens[i]);
            Lexer.tokenCount--;
        }
        else if (Lexer.tokens[i]->type == TokenOperator /*isOperator(infix[i])*/)
        {
            while (top != -1 && getOperatorPriority(*(stack[top]->value)) >= getOperatorPriority(*(Lexer.tokens[i]->value)))
                prefix[j++] = stack[top--];

            stack[++top] = Lexer.tokens[i];
        }
    }

    while (top != -1)
        prefix[j++] = stack[top--];

    free(Lexer.tokens);
    Lexer.tokens = prefix;

    // Инвертируем порядок токенов
    reverseTokensOrder();
}

static void printTokens(void)
{
    for(int i = 0; i < Lexer.tokenCount; i++)
        printf("Token %d: %.*s\n", i, Lexer.tokens[i]->length, Lexer.tokens[i]->value);
}

static TokenCore* newToken(char* value, int length, TokenType type)
{
    TokenCore* token = (TokenCore*)calloc(1, sizeof(TokenCore));

    token->value = value;
    token->length = length;
    token->type = type;

    return token;
}
static void addToken(TokenCore* token)
{
    Lexer.tokenCount++;
    Lexer.tokens = realloc(Lexer.tokens, Lexer.tokenCount * sizeof(TokenCore*));
    Lexer.tokens[Lexer.tokenCount - 1] = token;
}

static OperationPriority getOperatorPriority(const char chr)
{
    if (chr == '+' || chr == '-')
        return 1;
    else if (chr == '*' || chr == '/')
        return 2;
//    else if (chr == '^')
//        return 3;
    else
        return 0;
}

static void clearTokens(void)
{
    for (int i = 0; i < Lexer.tokenCount; i++)
    {
        free(Lexer.tokens[i]);
        Lexer.tokens[i] = NULL;
    }
    free(Lexer.tokens);
    Lexer.tokens = NULL;
    Lexer.tokenCount = 0;
}

static void reverseTokensOrder(void)
{
    int i, j;
    TokenCore* temp;
    for (i = 0, j = Lexer.tokenCount - 1; i < j; i++, j--)
    {
        temp = Lexer.tokens[i];
        Lexer.tokens[i] = Lexer.tokens[j];
        Lexer.tokens[j] = temp;
    }
}

static bool isOperator(const char chr)
{
    if (chr == '+' || chr == '-' || chr == '*' || chr == '/')
        return 1;
    else
        return 0;
}

static bool isComma(const char chr)
{
    if (chr == ',')
        return 1;
    else
        return 0;
}

static int isFunction(const char* str)
{
    if (strncmp(str, "log", 3) == 0)
        return 3;
    else if (strncmp(str, "pow", 3) == 0)
        return 3;
    else
        return 0;
}

static inline bool isLeftParentheses(const char chr)
{
    if (chr == '(')
        return 1;
    else
        return 0;
}

static inline bool isRigthParentheses(const char chr)
{
    if (chr == ')')
        return 1;
    else
        return 0;
}

static int isNumber(const char* str)
{
    double temp = 0.0;
    char* end;

    temp = strtod(str, &end);

    if (temp != 0.0)
        return (end - str)/sizeof(char);
    else
        return 0;
}
