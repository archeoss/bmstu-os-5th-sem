CC = gcc
# INCLUDE_DIR = include
# INCLUDE_DIR = ../backup/3/include
BINARIES = $(patsubst src/%.c, bin/%.o, $(wildcard src/*.c))
# BINARIES = $(patsubst ../backup/3/src/%.c,../backup/3/bin/%.o, $(wildcard ../backup/2/src/*.c))
# Remove -g if not in debug mode
COMPILE_FLAGS = -Wall -Werror -std=c99
LIST = 1 2 3 4 5
BIN_FOLDER = bin/
targets_listen = $(addprefix $(BIN_FOLDER)client_listen, $(LIST))

all: listen_only

listen_only: $(targets_listen) $(BIN_FOLDER)server_listen
	rm -f /tmp/demo_socket.sock
	cd $(BIN_FOLDER)
	./$(BIN_FOLDER)server_listen & st ./$(BIN_FOLDER)client_listen1 & st ./$(BIN_FOLDER)client_listen2 & st ./$(BIN_FOLDER)client_listen3 & st ./$(BIN_FOLDER)client_listen4 & st ./client_listen5
client_listen%: client.c
	gcc -o $@ client.c
$(BIN_FOLDER)server_listen: 
	gcc -o $@ server.c

clean:
	rm -rf $(BIN_FOLDER)*
