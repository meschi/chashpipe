#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HASH_SHA1	1
#define HASH_SHA256	2
#define HASH_SHA512	3

#define ENC_HEX		1
#define ENC_BASE64	2
#define ENC_BASE58	3

#define BLOCKSIZE 100;

#define MAXIMUM(a, b)	(((a) > (b)) ? (a) : (b))

extern char *__progname;

int mflag;			/* use multihash (https://github.com/jbenet/multihash/) */
int fflag = HASH_SHA256;	/* hash function */
int eflag = ENC_HEX;		/* encoding */



void help(void);
void usage(int);

int
main(int argc, char **argv)
{
	int ch;

	int rfd, wfd;
	int nread, nwrit;
	struct stat sbuf;
	char *buf, *rbuf;
	int base_size, current_size, off = 0;

	while ((ch = getopt(argc, argv, "e:f:hm")) != -1) {
		switch (ch) {
		case 'e':
			if (!strcmp(optarg, "hex"))
				eflag = ENC_HEX;
			else if (!strcmp(optarg, "base64"))
				eflag = ENC_BASE64;
			else if (!strcmp(optarg, "base58"))
				eflag = ENC_BASE58;
			else {
				errx(1, "Encoding \"%s\" unknown... Aborting!", optarg);
				exit(1);
				/* NOTREACHED */
			}
			break;
		case 'f':
			if (!strcmp(optarg, "sha1"))
				fflag = HASH_SHA1;
			else if (!strcmp(optarg, "sha256"))
				fflag = HASH_SHA256;
			else if (!strcmp(optarg, "sha512"))
				fflag = HASH_SHA512;
			else {
				errx(1, "Hash function \"%s\" unknown... Aborting!", optarg);
				exit(1);
				/* NOTREACHED */
			}
			break;
		case 'h':
			help();
			/* NOTREACHED */
		case 'm':
			mflag = 1;
			break;
		}
	}
	argv += optind;
	argc += optind;

	if (argc == 0) /* No hash supplied */
		usage(1);

	rfd = fileno(stdin);
	wfd = fileno(stdout);

	if (fstat(wfd, &sbuf))
		err(1, "fstat stdout");

	current_size = base_size = MAXIMUM(sbuf.st_blksize, BUFSIZ);

	if (buf = (char *)malloc(base_size * BLOCKSIZE))
		err(1, "malloc");

	//FIXME DO something clever
	/* TODO use nread and nwrit */
	while ((nread = read(rfd, buf + off, base_size - off)) != -1 && nread != 0) {
		off += nread;
		if (off == base_size) {
			rbuf = (char *)realloc(current_size + base_size);
			if (rbuf == NULL)
		}
	}


	return 0;
}

void
help(void)
{
	usage(0);
	fprintf(stderr,
	    "Command Summary:\n\
	    \t-e        Encoding (base64, base58, hex, bin)\n\
	    \t-f        Hash algorithm (sha1, sha256, sha512, md5)\n\
	    \t-h        Print this help\n\
	    \t-m        Use multihash format\n");
	exit(1);
}

void
usage(int ret)
{
	fprintf(stderr,
	    "usage: %s [-hm] [-e encoding] [-f hashfunction] [hash]\n", __progname);
	if (ret)
		exit(1);
}
