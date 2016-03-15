#include <stdio.h>
#include <stdlib.h>

#include "list.h"

Taolist * init_Taolist_fun(int len,int data_len)
{
    Taolist *s = (Taolist *)malloc(sizeof(Taolist));
    s->len=0;
    s->cur=0;
    s->state=0;
    s->max_len=len;
    s->data=(len<=0 ? NULL:malloc(data_len*len));
    return s;
}

void Taolist_kill(Taolist *l)
{
    free(l->data);
    free(l);
}

void test_Taolist()
{
    Taolist *intl = Taolist_init(int,10);
    Taolist_add(int,intl,234);
    Taolist_add(int,intl,67);
    Taolist_add(int,intl,89);
    Taolist_add(int,intl,29);
    Taolist_add(int,intl,55);
    Taolist_update(int,2,intl,22222);

    for(int i=0;i<intl->len;i++){

        printf("%d\n",Taolist_get(int,i,intl));
    }
}
