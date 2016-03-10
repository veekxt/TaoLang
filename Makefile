cfile = datastr/list.c parser/parser.c parser/tokenizer.c parser/tools.c main.c explain/explain.c explain/sys_fun.c
xtlang:$(cfile)
	gcc $(cfile) -o tmp_lang
