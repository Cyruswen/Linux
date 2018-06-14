.PHONY:	all
all:client server
client:client.c
	gcc $^ -o $@ -static
server:server.c
	gcc $^ -o $@ -lpthread
.PHONY:clean
clean:
	rm -rf client server
