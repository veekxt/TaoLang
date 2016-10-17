#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

typedef enum
{
    C_INT = 1,
    C_BOOL,
    C_FLOAT,
    C_STR,
    C_NONE,
    C_USEROBJ,
}value_type;

typedef struct
{
    long val;
}bn_int;

typedef struct
{
    double val;
}bn_float;

typedef struct
{
    char *val;
    size_t len;
}bn_str;

typedef struct
{
    int val;
}bn_bool;

typedef struct
{
    value_type type;
    union
    {
        bn_int int_value;
        bn_str str_value;
        bn_float float_value;
        bn_bool bool_value;
    }value;
}Tao_value;

Tao_value *new_str(char *a);
Tao_value *new_int(long a);
Tao_value *new_float(double a);
Tao_value *new_bool(int a);
#endif // TYPE_H_INCLUDED
