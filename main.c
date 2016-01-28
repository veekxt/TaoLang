#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser/tokenizer.h"

int main(void)
{
    struct token_list tl;
    file_to_token_to_array("tmp.xt",&tl);
    for(int i=0;i<tl.max_len;i++)
    {
        print_token(&tl.t[i]);
    }
    return 0;
}
