CFLAGS= -Wall -Wextra -Wpedantic -std=c++11
CFILES= stage1main.C *.cpp
BIN= stage1

all: ${CFILES}
	g++  ${CFILES} ${CFLAGS} -o ${BIN}


run: all
	./${BIN} ../stage0/dat/052.dat test.lst test.asm
