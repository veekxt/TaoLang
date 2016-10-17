#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

//数据结构之列表，可以追加元素，相当于自动增长长度的数组
typedef struct Taolist_st{
    size_t len;
    size_t cur;
    size_t max_len;
    int status;
    void *data;//存放具体数据的数组
}Taolist;

#define Taolist_get(type,n,l)\
        ((type)(*(((type *)l->data)+(n))))


#define Taolist_get_addr(type,n,l)\
        (((type *)l->data)+(n))

//更新一个值
#define Taolist_update(type,n,l,dat)\
    do{\
        type *not_use_tmp=(((type *)(l)->data)+(n));\
        *(not_use_tmp) = (dat);\
    }while(0)

//在尾部添加元素，空间不够时自动扩充空间为原来的3/2
#define Taolist_add(type,l,dat)\
    do{\
        if(l->len+1>l->max_len)\
        {\
            l->data=realloc(l->data,sizeof(type)*l->max_len+(l->max_len*3/2)*sizeof(type));\
            l->max_len+=l->max_len*3/2;\
        }\
        else ; \
        type *not_use_tmp=(((type *)l->data)+l->len);\
        *(not_use_tmp)=(dat);\
        l->len++;\
    }while(0)

//在中间插入一个元素
#define Taolist_inser(type,n,dat,l)\
    do{\
        if(l->len+1>l->max_len)\
        {\
            l->data=realloc(l->data,sizeof(type)*l->max_len+(l->max_len*3/2)*sizeof(type));\
            l->max_len+=l->max_len*3/2;\
        }\
        else ;\
        for(size_t i =l->len-1;i>=n;i--)\
        {\
            type tmp = Taolist_get(type,i,l);\
            Taolist_update(type,i+1,l,tmp);\
        }\
        type *not_use_tmp=(((type *)l->data)+n);\
        *(not_use_tmp)=(dat);\
        l->len++;\
    }\
    while(0)


//删除一个元素，但是并不缩小空间
#define Taolist_del(type,n,l)\
    do{\
        if((n)<0 || (n)>=l->len)break;\
        for(size_t i = (n)+1;i<l->len;i++)\
        {\
            type tmp = Taolist_get(type,i,l);\
            Taolist_update(type,i-1,l,tmp);\
        }\
        l->len--;\
    }while(0)

//删除最后一个元素，但是并不缩小空间
#define Taolist_pop(l)\
    do{\
        l->len--;\
    }while(0)

//删除所有元素，但是并不释放空间
#define Taolist_clear(l)\
    do{\
        l->len=0;\
    }while(0)

//交换两个位置的数据
#define Taolist_swap(type,n1,n2,l)\
    do{\
        type tmp1 = Taolist_get(type,n1,l);\
        type tmp2 = Taolist_get(type,n2,l);\
        Taolist_update(type,n1,l,tmp2);\
        Taolist_update(type,n2,l,tmp1);\
    }while(0)


//克隆一个列表
Taolist * Taolist_clone_fun(const Taolist *src,int data_len);
#define Taolist_clone(l,type) Taolist_clone_fun(l,sizeof(type))

//初始化列表一个
Taolist * init_Taolist_fun(int len,int data_len);
#define Taolist_init(type,init_len) init_Taolist_fun(init_len,sizeof(type))

void test_Taolist();
void Taolist_kill(Taolist *l);

#endif // LIST_H_INCLUDED
