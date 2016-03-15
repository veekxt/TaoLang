#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"

int main()
{
    Taolist *x = file_to_token_list("test/tok.tao");
    if(x->state!=-1)print_token_l(x);
    return 0;
}
