CC=gcc
LFLAGS=-lm
CFLAGS=-g -ggdb
OUT=compiler
IGNORE=-Wno-implicit-function-declaration

all: stack variables commands parser lexer compile

stack: stack.c
	$(CC) -c stack.c -o stack.o

variables: variables.c
	$(CC) -c variables.c -o variables.o

commands: commands.c
	$(CC) -c commands.c -o commands.o

parser: parser.y
	bison -d parser.y

lexer: lexer.l
	flex lexer.l

compile: parser lexer
	$(CC) $(CFLAGS) $(IGNORE) lex.yy.c parser.tab.c *.o $(LFLAGS) -o $(OUT)

clean:
	rm lex.yy.c parser.tab.c parser.tab.h *.o out.mips $(OUT)
