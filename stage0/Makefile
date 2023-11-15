CFLAGS= -Wall -Wextra -Wpedantic -std=c++11 -ferror-limit=2
CFILES= *.C *.cpp
BIN=main

all:
	g++ ${CFILES} ${CFLAGS} -o ${BIN}

run: all
	./${BIN} test.dat out/test.list out/test.asm 

test:
	g++ -DTEST ${CFILES} ${CFLAGS} -o ${BIN}
