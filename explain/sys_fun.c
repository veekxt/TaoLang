#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys_fun.h"
#include "explain.h"

void xtlang_sys_print(struct XTlist *argv)
{
    for(int i=0;i<argv->len;i++)
    {
        struct xt_value arg_tmp =  XTlist_get(argv,i,struct xt_value);
        switch(arg_tmp.type)
        {
            case XT_V_INT:
                {
                    printf("%d",arg_tmp.u.int_value);
                }
            break;
            case XT_V_STR:
                {

                }
            break;
        }
    }
}
