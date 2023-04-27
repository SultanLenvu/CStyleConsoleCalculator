#include "token_tree.h"

// Лексер (он такой один)
TokenTreeCore TokenTree;

/* Объявления */
//==============================================================================
// Внешние методы
static double calculate(void); // Вычисление выражения закодированного в дереве
static void build(TokenCore** tokens); // Построение дерева из массива указателей на токены
static void clear(void); // Очистка дерева
static void print(void); // Вывод всех узлов дерева в консоль

// Внутренние методы
static double calculateRecursive(TokenNode* node);
static void clearRecursive(TokenNode* node);
static TokenNode* buildRecursive(TokenCore** tokens);
static void printRecursive(TokenNode* node);
static TokenNode* newTokenNode(TokenCore* token);
//==============================================================================

/* Реализации */
//==============================================================================
// Конструктор дерева токенов
void initTokenTree(void)
{
    TokenTree.root = NULL;
    TokenTree.nodeCount = 0;

    TokenTree.build = build;
    TokenTree.calculate = calculate;
    TokenTree.clear = clear;
    TokenTree.print = print;
}

static double calculate(void)
{
    if (!TokenTree.root)
        return 0;

    return calculateRecursive(TokenTree.root);
}

static void build(TokenCore** tokens)
{
    if (!(*tokens) || (!tokens))
        return;

    TokenTree.root = buildRecursive(tokens);
}

static void clear(void)
{
    clearRecursive(TokenTree.root);
}

static void print(void)
{
    printRecursive(TokenTree.root);
}

static double calculateRecursive(TokenNode* node)
{
    if (node->token->type == TokenNumber)
        return strtod(node->token->value, NULL);
    else
    {
        double leftOperand = calculateRecursive(node->left);
        double rightOperand = calculateRecursive(node->right);

        if (node->token->type == TokenFunction)
        {
            if (strncmp(node->token->value, "log", 3) == 0)
                return log(rightOperand)/log(leftOperand);
            else if (strncmp(node->token->value, "pow", 3) == 0)
                return pow(leftOperand, rightOperand);
            else
                return 0;
        }
        else
        {
            switch (*(node->token->value))
            {
                case '+':
                    return leftOperand + rightOperand;
                case '-':
                    return leftOperand - rightOperand;
                case '*':
                    return leftOperand * rightOperand;
                case '/':
                    return leftOperand / rightOperand;
                default:
                    return 0;
            }
        }
    }
}

static void clearRecursive(TokenNode* node)
{
    if (node->left) clearRecursive(node->left);
    if (node->right) clearRecursive(node->right);
    free(node);
}

static TokenNode* buildRecursive(TokenCore** tokens)
{
    TokenCore* token = *tokens;

    if (token->type == TokenNumber)
        return newTokenNode(token);


    TokenNode *newNode = newTokenNode(token);
    newNode->left = buildRecursive(++tokens);
    newNode->right = buildRecursive(++tokens);

    return newNode;
}

static void printRecursive(TokenNode* node)
{
    if (node->left) printRecursive(node->left);
    if (node->right) printRecursive(node->right);
    printf("%.*s\n", node->token->length, node->token->value);
}

static TokenNode* newTokenNode(TokenCore* token)
{
    TokenNode* node = (TokenNode*)calloc(1, sizeof (TokenNode));
    node->token = token;
    node->left = NULL;
    node->right = NULL;

    return node;
}
