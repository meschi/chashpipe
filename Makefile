all:
	gcc -lssl -lcrypto hashpipe.c -o hashpipe
