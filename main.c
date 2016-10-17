#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"
#include "exec.h"

int test_a_file(char *s, int debug)
{
    //Taolist *x = file_to_token_list("test/primer.tao");
    Taolist *x = file_to_token_list(s);
    if(debug) puts("token:");
    if(debug) if(x!=NULL)print_token_l(x);
    if(x->status==-1)puts("note: tokenizer has error !");
    if(debug) puts("");
    AST *A = build_root_stmt(x,A_STMT);
    if(!PARSER_STATUS.IS_RIGHT)puts("note: parser has error !");
    if(debug) puts("\nAST:");
    if(debug) if(A!=NULL)AST_print(A,0,0);

    if(PARSER_STATUS.IS_RIGHT)
    {
        if(debug) puts("\ninterpret:");
        exec_env *env = make_init_env();
        exec_stmt(A,env);
    }else
    {
        if(debug) puts("\nparser error , so will not intepret!");
    }
    return 0;
}

int main(int argc, char **argv)
{
    int debug = 0;
    if(argc >= 3 && 0 == strcmp(argv[2], "-D")){
        debug = 1;
    }
    test_a_file(argv[1], debug);
    return 0;
}
