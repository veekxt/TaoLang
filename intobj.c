#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

Tao_class_entry int_entry=
{
    "int",
    C_INT,
    int_add,
    int_sub,
    int_mul,
    int_div,
    int_mod,
    NULL,
    int_to_str,
};

Tao_object *int_new(long int n)
{
    int_object *a = malloc(sizeof(int_object));
    a->ce = &int_entry;
    a->ob_ival = n;
    return (Tao_object *)a;
}

Tao_object *int_to_str(Tao_object *n)
{
    char tmp[100];
    sprintf(tmp,"%ld",((int_object*)n)->ob_ival);
    return str_new(tmp);
}

Tao_object *int_add(Tao_object *a,Tao_object *b)
{
    return int_new(((int_object *)a)->ob_ival+((int_object *)b)->ob_ival);
}
Tao_object *int_sub(Tao_object *a,Tao_object *b)
{
    return int_new(((int_object *)a)->ob_ival-((int_object *)b)->ob_ival);
}
Tao_object *int_mul(Tao_object *a,Tao_object *b)
{
    return int_new(((int_object *)a)->ob_ival*((int_object *)b)->ob_ival);
}
Tao_object *int_div(Tao_object *a,Tao_object *b)
{
    return int_new(((int_object *)a)->ob_ival/((int_object *)b)->ob_ival);
}
Tao_object *int_mod(Tao_object *a,Tao_object *b)
{
    return int_new(((int_object *)a)->ob_ival%((int_object *)b)->ob_ival);
}

Tao_class_entry str_entry=
{
    "str",
    C_STR,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    str_to_str,
};

Tao_object *str_new(char *str)
{
    size_t len = strlen(str);
    str_object *s = malloc(sizeof(str_object));
    s->ce = &str_entry;
    s->str = malloc(sizeof(char)*(len+1));
    strcpy(s->str,str);
    s->len = len;
    return (Tao_object *)s;
}

Tao_object *str_to_str(Tao_object *n)
{
    return n;
}

/*

Tao_class_entry float_entry=
{
    "float",
    C_FLOAT,
    float_to_str,
    NULL,
    NULL,
};

Tao_class_entry bool_entry=
{
    "bool",
    C_BOOL,
    bool_to_str,
    NULL,
    NULL,
};

Tao_class_entry none_entry=
{
    "none",
    C_NONE,
    none_to_str,
    NULL,
    NULL,
};

Tao_object *none_new(long int n)
{
    int_object *a = malloc(sizeof(none_object));
    a->ce = &none_entry;
    return (Tao_object *)a;
}

Tao_object *none_to_str(Tao_object *n)
{
    char tmp[100];
    sprintf(tmp,"%ld",((int_object*)n)->ob_ival);
    return str_new(tmp);
}

Tao_object *float_new(double n)
{
    float_object *a = malloc(sizeof(float_object));
    a->ce = &float_entry;
    a->ob_fval = n;
    return (Tao_object *)a;
}

Tao_object *bool_new(int n)
{
    bool_object *a = malloc(sizeof(bool_object));
    a->ce = &bool_entry;
    a->ob_bval = n==0?0:1;
    return (Tao_object *)a;
}


Tao_object *float_to_str(Tao_object *n)
{
    char tmp[100];
    sprintf(tmp,"%f",((float_object*)n)->ob_fval);
    return str_new(tmp);
}

Tao_object *bool_to_str(Tao_object *n)
{
    char tmp[10];
    sprintf(tmp,"%s",((bool_object*)n)->ob_bval==0?"false":"true");
    return str_new(tmp);
}
*/
