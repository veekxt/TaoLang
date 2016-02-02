#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser/tokenizer.h"

int main_t(void)
{
    struct token_list tl;
    file_to_token_to_array("tmp.xt",&tl);
    return 0;
}
