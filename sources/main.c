#include "lexer.h"
#include "token_tree.h"
#include "common_types.h"

int main()
{
    ReturnStatus ret = UnprocessableError;

    // Конструируем лексер
    initLexer();

    // Конструируем дерево токенов
    initTokenTree();

    printf("Calculator started. \n");

    while(1)
    {
        if (Lexer.input() == ExitCommand)
            return 0;

        ret = Lexer.parse();

        if (ret != NoError)
        {
            printf("Invalid input. Try again. \n");
            Lexer.clear();
        }
        else
        {
            //Lexer.printTokens();
            Lexer.convToPrefNot();
            //printf("Prefix notation: \n");
            //Lexer.printTokens();

            //printf("TokenTree nodes: \n");
            TokenTree.build(Lexer.tokens);
            //TokenTree.print();
            printf("Result = %lf\n", TokenTree.calculate());

            TokenTree.clear();
            Lexer.clear();
        }
    }
    return 0;
}
