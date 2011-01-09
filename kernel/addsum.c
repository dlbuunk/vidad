/* This file is part of Vidad.
*
*  Vidad is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Vidad is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Vidad.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

struct stat st;

int main(int argc, char **argv)
{	unsigned char *buffer;
	FILE *f;
	unsigned int i, size, sum;
	if (argc != 3) { puts("TWO file names please."); return(1); };
	if ((f = fopen(argv[1], "rb")) == NULL) { printf("Cannot open \"%s\"\n", argv[1]); return(-2); }
	if (stat(argv[1], &st)) { puts("stat() error"); return(-2); }
	size = st.st_size;
	size = (((size>>9)+1)<<9);
	if ((buffer = (unsigned char *) calloc(1,size)) == NULL) { puts("Memory allocation failure"); return(-1); }
	for (i=0; i<st.st_size; i++) buffer[i] = getc(f);
	fclose(f);

	*((unsigned int *) (buffer + 0x14)) = size;

	sum = 0;
	for (i=0x80; i<size; i+=4)
	{	sum = (sum>>1) + (sum<<31);
		sum += *((unsigned int *) (buffer + i));
	}
	*((unsigned int *) (buffer + 0x2C)) = sum;

	sum = 0;
	for (i=0; i<size; i+=4) sum += *((unsigned int *) (buffer + i));
	*((unsigned int *) (buffer + 0x28)) = 0 - sum;

	if ((f = fopen(argv[2], "wb")) == NULL) { printf("Cannot open \"%s\"\n", argv[2]); return(-2); }
	for (i=0; i<size; i++) putc(buffer[i], f);
	fclose(f);
	free(buffer);
	return(0);
};
