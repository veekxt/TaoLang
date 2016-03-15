#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

//数据结构之列表，可以追加元素，相当于自动增长长度的数组
typedef struct Taolist_st{
    size_t len;
    size_t cur;
    size_t max_len;
    int state;
    void *data;//存放具体对象指针的数组
}Taolist;

#define Taolist_get(type,n,l)\
        ((type)(*(((type *)l->data)+(n))))


#define Taolist_get_addr(type,n,l)\
        (((type *)l->data)+(n))

#define Taolist_update(type,n,l,dat)\
    do{\
        type *not_use_tmp=(((type *)(l)->data)+(n));\
        *(not_use_tmp) = (dat);\
    }while(0)

#define Taolist_add(type,l,dat)\
    do{\
        if(l->len+1>l->max_len)\
        {\
            l->data=realloc(l->data,sizeof(type)*l->max_len+(l->max_len*3/2)*sizeof(type));\
            l->max_len+=100;\
        }\
        else ; \
        type *not_use_tmp=(((type *)l->data)+l->len);\
        *(not_use_tmp)=(dat);\
        l->len++;\
    }while(0)

#define Taolist_pop(l)\
    do{\
        l->len--;\
    }while(0)

Taolist * init_Taolist_fun(int len,int data_len);
#define Taolist_init(type,init_len) init_Taolist_fun(init_len,sizeof(type))

void test_Taolist();

#endif // LIST_H_INCLUDED
