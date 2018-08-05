

.PHONY:all
all:server client

server:server.c comm.c
	gcc -o $@ $^
client:client.c comm.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f server client

