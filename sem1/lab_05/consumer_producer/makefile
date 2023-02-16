CC = gcc
INC_DIR = inc
OBJ = src/*.c
FLAGS = -Werror -Wall -std=c99

all: app.out

app.out: 
	${CC} ${OBJ} -I${INC_DIR} ${FLAGS} -o app.out

clean:
	rm *.o *.exe *.out

run: app.out
	./app.out