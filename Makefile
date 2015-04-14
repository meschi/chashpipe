all:
	gcc hashpipe.c -lssl -lcrypto -o bin/hashpipe

clean:
	rm bin/hashpipe

install:
	cp bin/hashpipe /usr/bin/hashpipe
