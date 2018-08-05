ROOT=$(shell pwd)

.PHONY:all
all:httpd cgi

httpd:httpd.c
	gcc -o $@ $^ -lpthread
cgi:
	cd $(ROOT)/wwwroot/cgi; make clean; make; cd -


.PHONY:output
output:
	mkdir -p output/wwwroot/cgi
	mkdir -p output/wwwroot/lib
	cp httpd output
	cp start.sh output
	cp -rf lib/lib output/lib
	cp -f wwwroot/*.html output/wwwroot
	cp -rf wwwroot/imag output/wwwroot
	cp -f wwwroot/cgi/netCal output/wwwroot/cgi
	cp -f wwwroot/cgi/selectData output/wwwroot/cgi
	cp -f wwwroot/cgi/insertData output/wwwroot/cgi

.PHONY:clean
clean:
	rm -f httpd
	cd $(ROOT)/wwwroot/cgi; make clean; cd -
	rm -rf output









