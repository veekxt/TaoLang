#include "tools.h"
#include "tokenizer.h"

int is_relation_sign(int n)
{
    if(
       n==I_LEFT_K
       || n==I_SMALLER_EQAUL
       || n==I_RIGHT_K
       || n==I_BIGER_EQAUL
       || n==I_EQAUL_VAR
       )return 1;
    return 0;
}
