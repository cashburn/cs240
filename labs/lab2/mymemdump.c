
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
	fprintf(fd, "0x%016lX: ", (unsigned long) p);
	for (int i = 0; i < len; i++) {
		fprintf(fd, "%02X ", p[i]&0xFF);
		if (((i+1) % 16) == 0){
			fprintf(fd, " ");
			for (int j = (i - 15); j <= i; j++) {
				if (((p[j]&0xFF) < 127) && ((p[j]&0xFF) >= 32))
					fprintf(fd, "%c", p[j]&0xFF);
				else
					fprintf(fd, "%c", '.');
			}
			fprintf(fd, "\n");
			if ((i+1) < len)
				fprintf(fd, "0x%016lX: ", (unsigned long) (p + i + 1));
		}
		else if (((len % 16) != 0) && !((i+1) < len)) {
			for (int j = 0; j < (3 * (16 - (len % 16))) + 1; j++)			
				fprintf(fd, " ");
			for (int j = (i - (len % 16) + 1); j <= i; j++) {
				if (((p[j]&0xFF) < 127) && ((p[j]&0xFF) >= 32))
					fprintf(fd, "%c", p[j]&0xFF);
				else
					fprintf(fd, "%c", '.');
			}
			fprintf(fd, "\n");
		}
	}
}
