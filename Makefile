CUR_DIR = $(shell pwd)

SRC_DIR = ${CUR_DIR}/src
OBJ_DIR = ${CUR_DIR}/build
CFLAGS = -g -Wall
TARGET = hash_test

${TARGET}: ${OBJ_DIR}/main.o ${OBJ_DIR}/hash_table.o ${OBJ_DIR}/prime.o
	gcc ${CFLAGS} $^ -o $@ -lm

${OBJ_DIR}/main.o: ${SRC_DIR}/main.c
	gcc ${CFLAGS} -c $< -o $@

${OBJ_DIR}/hash_table.o: ${SRC_DIR}/hash_table.c ${SRC_DIR}/hash_table.h
	gcc ${CFLAGS} -c $< -o $@

${OBJ_DIR}/prime.o: ${SRC_DIR}/prime.c ${SRC_DIR}/prime.h
	gcc ${CFLAGS} -c $< -o $@

clean:
	rm ${OBJ_DIR}/*.o
	rm ${TARGET}
