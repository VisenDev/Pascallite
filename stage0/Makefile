CFLAGS= -Wall -Wextra -Wpedantic -std=c++11
CFILES= *.C *.cpp
BIN=main

all:
	g++ ${CFILES} ${CFLAGS} -o ${BIN}

run: all
	./${BIN} dat/001.asm out/test.list out/test.asm 

test:
	g++ -DTEST ${CFILES} ${CFLAGS} -o ${BIN}
