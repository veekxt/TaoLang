#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "string.h"

Tao_value *new_int(long a)
{
    Tao_value *tmp=malloc(sizeof(Tao_value));
    tmp->type=C_INT;
    tmp->value.int_value.val = a;
    return tmp;
}

Tao_value *new_float(double a)
{
    Tao_value *tmp=malloc(sizeof(Tao_value));
    tmp->type=C_FLOAT;
    tmp->value.float_value.val = a;
    return tmp;
}

Tao_value *new_str(char *a)
{
    Tao_value *tmp=malloc(sizeof(Tao_value));
    tmp->type=C_STR;
    tmp->value.str_value.val = a;
    tmp->value.str_value.len = strlen(a);
    return tmp;
}
