#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

XTlist * init_XTlist(int len,int data_len)
{
    XTlist *s = (XTlist *)malloc(sizeof(XTlist));
    s->len=0;
    s->data_len=data_len;
    s->max_len=len;
    s->data=len<=0 ? NULL:malloc(data_len*len);
    return s;
}

int main_t_list(void)
{
    XTlist *s=init_XTlist(2,sizeof(int));
    XTlist_add(s,int,12);
    XTlist_add(s,int,11);
    XTlist_add(s,int,13);
    XTlist_add(s,int,14);
    printf("%d ",XTlist_get(s,0,int));
    printf("%d ",XTlist_get(s,1,int));
    printf("%d ",XTlist_get(s,2,int));
    printf("%d ",XTlist_get(s,3,int));
    printf("\n-%d-%d-",s->len,s->max_len);
}
