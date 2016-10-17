
CC=gcc

CFLAGS+=-I./include

src = list.c main.c parser.c tokenizer.c sysfun.c exec.c type.c
tao : $(src)
	$(CC) $(CFLAGS) $(src) -o tao
