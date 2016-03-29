src = list.c main.c parser.c tokenizer.c sysfun.c exec.c type.c
tao : $(src) list.h parser.h exec.h parser_comment.h sysfun.h type.h
	gcc $(src) -o tao
