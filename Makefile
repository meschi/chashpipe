all:
	gcc hashpipe.c -lssl -lcrypto -o hashpipe

clean:
	rm hashpipe

install:
	cp hashpipe /usr/bin/hashpipe
