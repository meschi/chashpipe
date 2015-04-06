#include <sys/stat.h>

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXIMUM(a, b)   (((a) > (b)) ? (a) : (b))

int
main(void)
{
	int in, out;
	int nread, nwrit, off = 0;
	int bufsize, blksize;
	struct stat statbuf;
	char *buf, *nbuf;
	int i; //debug counter


	in = fileno(stdin);
	out = fileno(stdout);

	if (fstat(in, &statbuf))
		err(1, "stdout");

	blksize = MAXIMUM(statbuf.st_blksize, BUFSIZ);
//	printf("blksize: %i bufsiz: %i\n", statbuf.st_blksize, BUFSIZ);
	bufsize = blksize * 4; /* mult by any value would work */

	//off += blksize;
	buf = malloc(bufsize);
	//bzero(buf, bufsize);//TEST
	//printf("bufsize %i, BUFSIZ %i, blksize %i\n", bufsize, BUFSIZ, statbuf.st_blksize);


	while ((nread = read(in, buf + off, blksize)) != -1 && nread != 0) {
/*debug*///	printf("read %i bytes\n", nread);
//		if ((nwrit=write(out, buf + off, nread)) <= 0)
//			err(1, "write to stdout");

//		printf("%s",buf + off);// + off);
		off += nread;
	//	printf("bufsize: %i, off: %i, 1-2: %i, blksize: %i\n",
	//	    bufsize, off, bufsize - off, blksize);
		if (bufsize - off < blksize) { /* increase memory */
//			printf("realloc\n");
			/* TODO replace with reallocarray */
			if ((nbuf = realloc(buf, bufsize * 2)) == NULL) {
				free(buf);
				err(1, "buffer");
			}

			buf = nbuf;
			bufsize *= 2;
		}
		printf("%s", buf+off);
	}

	if (nread == -1)
		err(1, "stdin");

	//buf[off] = '\0';
	for (i = 0; i < off; i++) printf("%c", buf[i]);
//	printf("%.*s",off, buf);

	free(buf);

	return 0;
}
