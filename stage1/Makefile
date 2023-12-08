CFLAGS= -Wall -Wextra -Wpedantic -std=c++11
CFILES= stage1main.C *.cpp
BIN= stage1

all: ${CFILES}
	g++  ${CFILES} ${CFLAGS} -o ${BIN}

run: all
	./${BIN} data1/101.dat out/101.lst out/101.asm
