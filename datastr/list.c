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
