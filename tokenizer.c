#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "list.h"
#define isalpha_(c) (isalpha(c) || (c)=='_')
#define isalnum_(c) (isalnum(c) || (c)=='_')

//关键字对比
token_type which_keyword(const char *s)
{
    if(s==NULL)return T_ERROR;
    if(strcmp(s,"lambda")==0)
    {
        return T_LAMBDA;
    }
    else if (strcmp(s,"null")==0)
    {
        return T_NULL;
    }
    else if (strcmp(s,"let")==0)
    {
        return T_LET;
    }
    else if (strcmp(s,"true")==0)
    {
        return T_TRUE;
    }
    else if (strcmp(s,"false")==0)
    {
        return T_FALSE;
    }
    else if (strcmp(s,"def")==0)
    {
        return T_DEF;
    }
    else if (strcmp(s,"class")==0)
    {
        return T_CLASS;
    }
    else if (strcmp(s,"continue")==0)
    {
        return T_CTN;
    }
    else if (strcmp(s,"break")==0)
    {
        return T_BRK;
    }
    else if (strcmp(s,"try")==0)
    {
        return T_TRY;
    }
    else if (strcmp(s,"catch")==0)
    {
        return T_CATCH;
    }
    else if (strcmp(s,"throw")==0)
    {
        return T_THROW;
    }
    else if (strcmp(s,"if")==0)
    {
        return T_IF;
    }
    else if (strcmp(s,"elif")==0)
    {
        return T_ELIF;
    }
    else if (strcmp(s,"else")==0)
    {
        return T_ELSE;
    }
    else if (strcmp(s,"while")==0)
    {
        return T_WHILE;
    }
    else if (strcmp(s,"for")==0)
    {
        return T_FOR;
    }
    else if (strcmp(s,"do")==0)
    {
        return T_DO;
    }
    else if (strcmp(s,"in")==0)
    {
        return T_IN;
    }
    else
    {
        return T_IDEN;
    }
}


//输出一个token，格式： token名称 => token内容
void print_token(token *t)
{
    printf("line %d , %-15s => %s\n",t->line,token_name[t->type],t->content==NULL?token_name[t->type]:t->content);
}

//获取文件长度
long int get_file_size(FILE *fp)
{
    fseek(fp,0,SEEK_END);
    long int size=ftell(fp);
    rewind(fp);
    return size;
}
//把文件读入字符串
file_string * file_into_string(FILE *fp)
{
    long int file_size=get_file_size(fp);
    file_string *s = (file_string *)malloc(sizeof(file_string)+2*sizeof(char)+sizeof(char)*file_size);
    fread(s->str,1,file_size,fp);
    s->str[file_size]='\n';
    s->str[file_size+1]=0;
    s->len=file_size;
    s->cur=s->str;
    s->line=1;
    return s;
}

//跳过无意义的部分，包括注释、空白符
void jmp_useless(file_string *fs)
{
    char *s = fs->cur;
    for(;;)
    {
        if(*s=='\0')break;
        while(is_white(*s))
        {
            s++;
        }
        if(*s=='/')
        {
            if(*(++s)=='/')
            {
                for(; *s!='\n'; s++)
                {
                    ;
                }
                continue;
            }
            else if(*s=='*')
            {
                for(++s;*s!='\0';++s)
                {
                    if(*s=='\n')fs->line++;
                    if(*s=='*' && *(s+1)=='/')
                    {
                        s=s+2;
                        break;
                    }
                }
                continue;
            }
            else
            {
                s--;
                break;
            }
        }
        else if(*s=='#')
        {
            for(; *s!='\n'; s++)
            {
                ;
            }
            continue;
        }
        else break;
    }
    fs->cur = s;
}

//给一个字符串添加一个字符
void add_char_to_str(char **s,char ch)
{
    size_t len = strlen(*s);
    *s = (char *)realloc(*s,sizeof(char)*(len+2));
    (*s)[len] = ch;
    (*s)[len+1] = 0;
}

//从文件字符串读取一个token
token get_a_token(file_string *fs)
{
    jmp_useless(fs);
    char *s=fs->cur;
    token target={T_ERROR,OP0,fs->line,NULL};
    priority pri = OP0;
    if(*s=='\0')
    {
        target.type=T_END;
    }
    //标识符或关键字
    else if(isalpha_(*s))
    {
        s++;
        while(isalnum_(*s))
        {
            s++;
        }
        int len_str = (s - fs->cur) / sizeof(char);
        char *content = malloc(sizeof(char)*len_str+1);
        memcpy(content,fs->cur,len_str * sizeof(char));
        content[len_str] = 0;
        target.content = content;
        target.type = which_keyword(content);
    }
    //数字
    else if(isdigit(*s))
    {
        s++;
        int is_float = 0;
        while(1)
        {
            if(*s=='.' &&  is_float == 0)
            {
                s++;
                if(isdigit(*s))
                {
                    is_float = 1 ;
                }else{
                    //todo 回退"."
                    s--;
                    break;
                };
            }
            else if(isdigit(*s))
            {
                s++;
            }else
            {
                break;
            }
        }
        int len_str = (s - fs->cur) / sizeof(char);
        char *content = malloc(sizeof(char)*len_str+1);
        memcpy(content,fs->cur,len_str * sizeof(char));
        target.content = content;
        content[len_str] = 0;
        target.type = is_float==0?T_INT:T_FLOAT;
    }
    //字符串
    else if(*s=='"')
    {
        s++;
        char *buf = calloc(1,sizeof(char));
        while(*s!='\0')
        {
            if(*s=='\\')
            {
                switch(*(s+1))
                {
                case 'n':
                    add_char_to_str(&buf,'\n');
                    s+=2;
                break;
                case '"':
                    add_char_to_str(&buf,'"');
                    s+=2;
                break;
                case 'a':
                    add_char_to_str(&buf,'\a');
                    s+=2;
                break;
                case 'b':
                    add_char_to_str(&buf,'\b');
                    s+=2;
                break;
                case 'f':
                    add_char_to_str(&buf,'\f');
                    s+=2;
                break;
                case 'r':
                    add_char_to_str(&buf,'\r');
                    s+=2;
                break;
                case 't':
                    add_char_to_str(&buf,'\t');
                    s+=2;
                break;
                case 'v':
                    add_char_to_str(&buf,'\v');
                    s+=2;
                break;
                case '\\':
                    add_char_to_str(&buf,'\\');
                    s+=2;
                break;
                case '0':
                    add_char_to_str(&buf,'\0');
                    s+=2;
                break;
                default:
                    s++;
                    add_char_to_str(&buf,'\\');
                    ;
                }
            }else
            {
                if(*s=='"')
                {
                    s++;
                    break;
                }
                else
                {
                    add_char_to_str(&buf,*s);
                    s++;
                }
            }
        }
        target.content=buf;
        target.type=T_STRING;
    }
    else
    {
        switch(*(s++))
        {
            case '=':
                {
                    switch(*(s++))
                    {
                        case '=':
                            target.type = T_EQUAL;
                            pri = OP7;
                        break;
                        case '>':
                            target.type = T_TO;
                        break;
                        default:target.type = T_ASSIGN;
                                s--;
                    }
                }
            break;
            case '.':target.type = T_VISIT;pri=OP3;
            break;
            case ':':target.type = T_SVISIT;
            break;
            //字符串字面量已经单独处理
            //case '"':target.type = T_STRING;
            //break;
            case ',':target.type = T_COMMA;
            break;
            case '\n':
                fs->line++;
                target.type = T_SEMI_N;
            break;
            case ';':target.type = T_SEMI;
            break;
            case '(':target.type = T_LPAR;
            break;
            case ')':target.type = T_RPAR;
            break;
            case '[':target.type = T_LSQB;
            break;
            case ']':target.type = T_RSQB;
            break;
            case '{':target.type = T_LBRACE;
            break;
            case '}':target.type = T_RBRACE;
            break;
            case '+':target.type = T_ADD;pri=OP6;
            break;
            case '-':target.type = T_MINUS;pri=OP6;
            break;
            case '*':
                {
                    if(*(s++)=='*')
                    {
                        target.type = T_TWOSTAR;
                        pri=OP5;
                    }else
                    {
                        s--;
                        target.type = T_STAR;
                        pri=OP5;
                    }
                }
            break;
            case '/':target.type = T_SLASH;pri=OP5;
            break;
            case '%':target.type = T_PERSENT;pri=OP5;
            break;
            case '<':
                {
                    if(*(s++)=='=')
                    {
                        target.type = T_LESSEQ;
                        pri=OP7;
                    }else
                    {
                        s--;
                        target.type = T_LESS;
                        pri=OP7;
                    }
                }
            break;
            case '>':
                {
                    if(*(s++)=='=')
                    {
                        target.type = T_MOREEQ;
                        pri=OP7;
                    }else
                    {
                        s--;
                        target.type = T_MORE;
                        pri=OP7;
                    }
                }
            break;
            case '!':
                {
                    if(*(s++)=='=')
                    {
                        target.type = T_NOTEQUAL;
                        pri=OP7;
                    }else
                    {
                        s--;
                        target.type = T_NOT;
                        pri=OP4;
                    }
                }
            break;
            case '|':
                {
                    if(*(s++)=='|')
                    {
                        target.type = T_OR;
                        pri=OP8;
                    }else
                    {
                        s--;
                        target.type = T_ERROR;
                    }
                }
            break;
            case '&':
                {
                    if(*(s++)=='&')
                    {
                        target.type = T_AND;
                        pri=OP8;
                    }else
                    {
                        s--;
                        target.type = T_ERROR;
                    }
                }
            break;
            default :target.type = T_ERROR;
        }
    }
    fs->cur = s;
    target.line = fs->line;
    target.pri = pri;
    return target;
}

Taolist * file_to_token_list(const char *fname)
{
    FILE *fp =fopen(fname,"rb");

    if(fp==NULL){printf("error: Can't open file!\n");exit(0);}

    file_string *fs = file_into_string(fp);
    Taolist *token_l = Taolist_init(token,300);

    token_type pre_type = T_ERROR;
    for(;;)
    {
        //获取token并添加到列表里
        //合并连续的换行符和(或)分号，规则：
        // ;\n => ;
        // ;; => ;
        // \n; => ;
        // \n\n => \n
        // 因为句末分号可选，但是两种分隔符又不完全一样
         token tmp = get_a_token(fs);
         if((pre_type ==T_SEMI || pre_type ==T_SEMI_N) && tmp.type==T_SEMI)
         {
             Taolist_update(token,token_l->len-1,token_l,tmp);
         }else if(!((pre_type ==T_SEMI &&  tmp.type==T_SEMI_N)||(pre_type ==T_SEMI_N && tmp.type==T_SEMI_N)))
         {
             Taolist_add(token,token_l,tmp);
         }
         pre_type = tmp.type;
         if(tmp.type==T_ERROR)
         {
             printf("error: Unkonwn token on line %d\n",tmp.line);
             token_l->status = -1;
         }
         if(tmp.type==T_END)return token_l;
    }
    if(token_l->status == -1)
    {
        Taolist_kill(token_l);
        return NULL;
    }
    return token_l;
}

void print_token_l(Taolist * l)
{
    while(1)
    {
        token *x = get_token(0,1,l);
        if(x->type==T_END){print_token(x);break;}
        print_token(x);
    }
    l->cur=0;
}

//获取一个token，n超前前几个位置，i内部指针偏移多少
token * get_token(int n,int i,Taolist *l)
{
    token *tmp = Taolist_get_addr(token,l->cur+n,l);
    l->cur+=i;
    return tmp;
}
