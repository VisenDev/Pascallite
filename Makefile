CFLAGS= -g -Wall -Wextra -Wpedantic -std=c++11 -ferror-limit=2
CFILES= stage0/*.C stage0/*.cpp
BIN=main

all:
	g++ ${CFILES} ${CFLAGS} -o ${BIN}

test:
	g++ -DTEST ${CFILES} ${CFLAGS} -o ${BIN}
	./main
