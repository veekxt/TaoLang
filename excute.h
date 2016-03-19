#ifndef EXCUTE_H_INCLUDED
#define EXCUTE_H_INCLUDED

/*
typedef struct
{
    char *name;
    int tp_size;
    int init_size;
    print_func tp_print;
}Tao_type;

typedef struct
{
    Tao_type *type;
    void *data;
}Tao_object;

typedef struct
{
    Tao_type *type;
    long ob_ival;
}Tao_int_object;

*/
#define OBJ_HEAD \
char *name;\
Tao_class_entry *ce;


typedef struct
{
    char *name;
    //funcl *method;
    //prol *properties;
}Tao_class_entry;

typedef struct
{
    OBJ_HEAD
}Tao_object;

typedef struct
{
    OBJ_HEAD
    long ob_ival;
}int_object;

typedef struct
{
    OBJ_HEAD
    double ob_ival;
}float_object;

#endif // EXCUTE_H_INCLUDED
