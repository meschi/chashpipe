all:
	gcc -lssl -lcrypto hashpipe.c -o hashpipe

clean:
	rm hashpipe

install:
	cp hashpipe /usr/bin/hashpipe
