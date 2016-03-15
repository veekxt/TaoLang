src = list.c main.c parser.c tokenizer.c
tao : $(src) list.h parser.h
	gcc $(src) -o tao
