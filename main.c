#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"
#include "object.h"

int main()
{
    //Taolist *x = file_to_token_list("test/primer.tao");
    Taolist *x = file_to_token_list("test/exp.tao");
    puts("token:");
    if(x!=NULL)print_token_l(x);
    if(x->status==-1)puts("note: tokenizer has error !");
    puts("");
    AST *A = build_root_stmt(x,A_STMT);
    if(!PARSER_STATUS.IS_RIGHT)puts("note: parser has error !");
    puts("\nAST:");
    if(A!=NULL)AST_print(A,0,0);
    return 0;
}
