#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

typedef enum
{
    C_INT,
    C_BOOL,
    C_FLOAT,
    C_STR,
    C_NONE,
    C_USEROBJ,
}obj_type;

struct Tao_object_s;

typedef struct Tao_object_s *(*to_str_fun)(struct Tao_object_s *);
typedef struct Tao_object_s *(*cal_fun)(struct Tao_object_s *,struct Tao_object_s *);

typedef struct
{
    char *name;//类名
    obj_type ob_type;//类型
    //魔术方法
    cal_fun ob_add;//四则运算
    cal_fun ob_sub;
    cal_fun ob_mul;
    cal_fun ob_div;
    cal_fun ob_mod;
    cal_fun ob_pow;

    to_str_fun ob_to_str;//转换为str
    //暂时只需要这些

//    to_str_fun ob_init;
//    to_str_fun ob_len;
    //todo:属性

}Tao_class_entry;

#define OBJ_HEAD \
Tao_class_entry *ce;\

typedef struct Tao_object_s
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
    int ob_bval;
}bool_object;

typedef struct
{
    OBJ_HEAD
    double ob_fval;
}float_object;

typedef struct
{
    OBJ_HEAD
    char *str;
    size_t len;
}str_object;

Tao_object *int_new(long int);
Tao_object *int_to_str(Tao_object *n);
Tao_object *int_add(Tao_object *a,Tao_object *b);
Tao_object *int_sub(Tao_object *a,Tao_object *b);
Tao_object *int_mul(Tao_object *a,Tao_object *b);
Tao_object *int_div(Tao_object *a,Tao_object *b);
Tao_object *int_mod(Tao_object *a,Tao_object *b);

Tao_object *str_new(char *);
Tao_object *str_to_str(Tao_object *n);

/*
Tao_object *flaot_to_str(Tao_object *n);
Tao_object *bool_to_str(Tao_object *n);
Tao_object *float_new(double);
Tao_object *bool_new(int n);
*/

#endif // EXCUTE_H_INCLUDED
