CFLAGS= -g -Wall -Wextra -Wpedantic -std=c++11 -ferror-limit=2
CFILES=stage0main.C impl.cpp grammar.cpp codegen.cpp lexer.cpp helpers.cpp
BIN=main

all:
	g++ ${CFILES} ${CFLAGS} -o ${BIN}

test:
	g++ -DTEST ${CFILES} ${CFLAGS} -o ${BIN}
	./main
