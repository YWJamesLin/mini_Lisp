#! /bin/bash
bison -d -o y.tab.c miniLisp.y
gcc -c -g -I.. y.tab.c

flex -o lex.yy.c miniLisp.l
gcc -c -g -I.. lex.yy.c

gcc -o miniLisp y.tab.o lex.yy.o -lfl
