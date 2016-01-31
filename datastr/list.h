#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

//数据结构之列表，可以追加元素，相当于自动增长长度的数组
typedef struct XTlist{
int len;
int max_len;
int data_len;
void *data;//存放具体对象指针的数组
}XTlist;

#define LIST_ADD 100

#define XTlist_get(XTlist,n,type)\
        ((type)(*(((type *)XTlist->data)+(n))))

#define XTlist_assign(XTlist,n,type,dat)\
    {\
        type *tmp=(((type *)(XTlist)->data)+(n));\
        *(tmp) = (dat);\
    }
#define XTlist_add(l,type,dat)\
    {\
        if(l->len+1>l->max_len)\
        {\
            l->data=realloc(l->data,l->data_len*l->max_len+LIST_ADD*l->data_len);\
            l->max_len+=100;\
        }\
        else ; \
        type *tmp=(((type *)l->data)+l->len);\
        *(tmp)=(dat);\
        l->len++;\
    }
XTlist * init_XTlist(int len,int data_len);
#endif // LIST_H_INCLUDED
