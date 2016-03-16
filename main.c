#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"

int main()
{
    Taolist *x = file_to_token_list("test/primer.tao");
    if(x->status!=-1)print_token_l(x);
    return 0;
}
