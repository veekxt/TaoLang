#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"

int main()
{
    //Taolist *x = file_to_token_list("test/primer.tao");
    Taolist *x = file_to_token_list("test/exp.tao");
    puts("token:");
    if(x!=NULL)print_token_l(x);
    puts("\nAST:");
    AST *A = build_exp(x);
    if(A!=NULL)AST_print(A,0);
    return 0;
}
