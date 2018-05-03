.PHONY:all
all:service client
service:service.c
	gcc -o $@ $^
client:client.c
	gcc -o $@ $^ -static
.PHONY:clean
clean:
	rm -r service client

