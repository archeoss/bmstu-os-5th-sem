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

# all: listen_only

server_to_client: $(targets_listen) $(BIN_FOLDER)server_to
	rm -f /tmp/socket.sock /tmp/client_socket.sock
	killall server_to
client_listen%: client.c
	gcc -o $@ client.c
$(BIN_FOLDER)server_listen: 
	gcc -o $@ server.c

$(BIN_FOLDER)server_to: 
	gcc -o $@ server.c

clean:
	rm -rf $(BIN_FOLDER)*
