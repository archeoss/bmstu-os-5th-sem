rm client1 client2 client3 client4 client5 server /tmp/demo_socket.sock
gcc -o server server.c
gcc -o client1 client.c
gcc -o client2 client.c
gcc -o client3 client.c
gcc -o client4 client.c
gcc -o client5 client.c
clear
./server & st ./client1 & st ./client2 & st ./client3 & st ./client4 & st ./client5
