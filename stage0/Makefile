CFLAGS= -Wall -Wextra -Wpedantic -std=c++11
CFILES= *.C stage0.cpp
BIN=main

all:
	g++ ${CFILES} ${CFLAGS} -o ${BIN}

run: all
	./test.sh ${BIN} ./dat/

test:
	g++ -DTEST ${CFILES} ${CFLAGS} -o ${BIN}
