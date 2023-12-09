CFLAGS= -Wall -Wextra -Wpedantic -std=c++11
CFILES= stage1main.C
BIN= stage1
DAT= 101

all: ${CFILES}
	g++  ${CFILES} ${CFLAGS} -o ${BIN}

run: all
	./${BIN} data1/${DAT}.dat out/${DAT}.lst out/${DAT}.asm
