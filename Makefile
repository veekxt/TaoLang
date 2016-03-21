src = list.c main.c parser.c tokenizer.c sysfun.c intobj.c exec.c
tao : $(src) list.h parser.h
	gcc $(src) -o tao
