#include <sys/stat.h>

#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/whrlpool.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HASH_SHA1	1
#define HASH_SHA256	2
#define HASH_SHA512	3
#define HASH_MD5	4
#define HASH_RIPEMD160	5
#define HASH_WHIRLPOOL	6

#define GROWFAC 2

#define MAXIMUM(a, b)	(((a) > (b)) ? (a) : (b))

extern char *__progname;

int fflag = HASH_SHA256;	/* hash function */

char *fname = "sha256";

int digest_size(void);
void check_hash(char *, int, char *);
void help(void);
void usage(int);

int
main(int argc, char **argv)
{
	int ch, i;
	int rfd, wfd;
	int nread, nwrit;
	struct stat sbuf;
	char *buf, *nbuf; /* buffer for stdin; buffer for realloc() */
	int blksize, bufsize, off = 0;
	unsigned char *digest;
	char fdigest[SHA_DIGEST_LENGTH * 2 + 1];

	while ((ch = getopt(argc, argv, "f:h")) != -1) {
		switch (ch) {
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
		}
	}
	argv += optind;
	argc -= optind;


	if (argc == 0) /* No hash supplied */
		usage(1);
	else if (strlen(argv[0]) != digest_size() * 2)
		errx(1, "Hash length does not match hash function %s\n", fname);



	rfd = fileno(stdin);
	wfd = fileno(stdout);

	if (fstat(wfd, &sbuf))
		err(1, "stdout");

	blksize = MAXIMUM(sbuf.st_blksize, BUFSIZ);
	bufsize = blksize * 4; /* value may be tweakable */

	if (!(buf = malloc(bufsize)))
		err(1, "malloc");


	while ((nread = read(rfd, buf + off, blksize)) != -1 && nread != 0) {
		off += nread;

		if (bufsize - off < blksize) {
			if ((nbuf = realloc(buf, bufsize * GROWFAC)) == NULL)
				err(1, "increasing buffer");

			buf = nbuf;
			bufsize *= GROWFAC;
		}
	}

	if (nread == -1)
		err(1, "stdin");


	check_hash(buf, off, argv[0]);

	/* TODO Output STDIN Buff here */
	printf("%s", buf);
	return 0;
}

int
digest_size()
{
	switch (fflag) {
	case HASH_SHA1:
		return SHA_DIGEST_LENGTH;
	case HASH_SHA256:
		return SHA256_DIGEST_LENGTH;
	case HASH_SHA512:
		return SHA512_DIGEST_LENGTH;;
	case HASH_RIPEMD160:
		return RIPEMD160_DIGEST_LENGTH;
	case HASH_WHIRLPOOL:
		return WHIRLPOOL_DIGEST_LENGTH;
	}
}

/*
 * exits on wrong hash value
 */
void
check_hash(char *buf, int size, char *chkhash)
{
	unsigned char *digest;
	char *fdigest;
	int digsize;
	int i;

	digsize = digest_size();

	if ((digest = malloc(digsize)) == NULL)
		err(1, "digest");

	/* hex representation is twice the size */
	if ((fdigest = malloc(digsize * 2 + 1)) == NULL) {
		free(digest);
		err(1, "fdigest");
	}


	/* hash */
	switch (fflag) {
	case HASH_SHA1:
		SHA1(buf, size, digest);
		break;
	case HASH_SHA256:
		SHA256(buf, size, digest);
		break;
	case HASH_SHA512:
		SHA512(buf, size, digest);
		break;
	case HASH_RIPEMD160:
		RIPEMD160(buf, size, digest);
		break;
	case HASH_WHIRLPOOL:
		WHIRLPOOL(buf, size, digest);
	}

	/* create hexadecimal ascii encoding */
	for (i = 0; i < digsize; i++)
		snprintf(&fdigest[i * 2], 3, "%02x", digest[i]);

	if (strcmp(chkhash, fdigest))
		errx(1, "Hashes do not match: %s\n", fdigest);

	free(fdigest);
	free(digest);
}

void
help(void)
{
	usage(0);
	fprintf(stderr,
	    "Command Summary:\n\
	    \t-f        Hash algorithm (sha1, sha256, sha512, md5)\n\
	    \t-h        Print this help\n\
	    ");
	exit(1);
}

void
usage(int ret)
{
	fprintf(stderr,
	    "usage: %s [-h] [-f hashfunction] [hash]\n", __progname);
	if (ret)
		exit(1);
}
