#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"
#include "exec.h"

int test_a_file(char *s)
{
    //Taolist *x = file_to_token_list("test/primer.tao");
    Taolist *x = file_to_token_list(s);
    puts("token:");
    if(x!=NULL)print_token_l(x);
    if(x->status==-1)puts("note: tokenizer has error !");
    puts("");
    AST *A = build_root_stmt(x,A_STMT);
    if(!PARSER_STATUS.IS_RIGHT)puts("note: parser has error !");
    puts("\nAST:");
    if(A!=NULL)AST_print(A,0,0);

    if(PARSER_STATUS.IS_RIGHT)
    {
        puts("\ninterpret:");
        exec_env *env = make_init_env();
        exec_stmt(A,env);
    }else
    {
        puts("\nparser error , so will not intepret!");
    }
    return 0;
}

int main(void)
{
    test_a_file("test/primer.tao");
    return 0;
}
