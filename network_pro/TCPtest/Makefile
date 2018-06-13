.PHONY:all
all:client service
client:client.c
	gcc $^ -o $@
service:service.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm client service
