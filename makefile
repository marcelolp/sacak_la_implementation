CC = cl

DIR_SRC = src
DIR_LIB = lib
DIR_BIN = bin

EXE_NAME = sacak_la_main.exe

SRCS = \
	${DIR_SRC}/sacak_la_main.c \
	${DIR_SRC}/file_io.c \
	${DIR_SRC}/symbols.c \
	${DIR_SRC}/sacak_la.c \
	${DIR_SRC}/test.c

all : generate move

generate :
	$(CC) ${SRCS}

move : 
	move *.obj ./${DIR_BIN} 
	move *.exe ./${DIR_BIN}

run : 
	${DIR_BIN}/${EXE_NAME}

clean :
	del /f ${DIR_SRC}/*.obj ${DIR_BIN}/${EXE_NAME}