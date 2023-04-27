#ifndef TOKEN_TREE_H
#define TOKEN_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "common_types.h"

// Узел дерева токенов
typedef struct TokenNode {
    TokenCore* token;
    struct TokenNode* left;
    struct TokenNode* right;
} TokenNode;

typedef struct {
    TokenNode* root;
    int nodeCount;
    ICalculateTokenTree calculate;
    IBuildTokenTree build;
    IFunction clear;
    IFunction print;
} TokenTreeCore;

// Лексер (он такой один)
extern TokenTreeCore TokenTree;

// Конструктор лексера
void initTokenTree(void);

#endif // TOKEN_TREE_H

