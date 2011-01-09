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

#include <kernel.h>

/* bug in this implementation:
* malloc() breaks when trying to allocate in the upper 2GB of memory
* also, realloc() is missing */

#define BACK *((dword *) ptr)
#define FORW *((dword *) (ptr + 4))
#define NEXT_BACK *((dword *) FORW)
#define NEXT_FORW *((dword *) (FORW + 4))
#define PREV_FORW *((dword *) (BACK + 4))
#define PREV_BACK *((dword *) BACK)
#define NEW_BACK *((dword *) (ptr + size + 8))
#define NEW_FORW *((dword *) (ptr + size + 12))

dword alloc_base;
dword alloc_top;

void alloc_init(dword heap_base, dword top_of_mem)
{	alloc_base = heap_base;
	alloc_top = top_of_mem;
	*((dword *) alloc_base) = 0;
	*((dword *) (alloc_base + 4)) = 0;
}

void alloc_error()
{	kerror("Fatal error: out of memory", 0x2F);
}

void *malloc(dword size)
{	dword ptr;
	ptr = alloc_base;

	size = (((size-1)>>2) + 1) << 2;

	for (;;)
	{	/* skip ahead until we find a unused block */
		while (FORW & 0x80000000) ptr = FORW & 0x7FFFFFFF;

		/* is block large enough? */

		if (FORW == 0)
		/* Yes, we are at the top of the heap */
		{	if (size + ptr + 8 > alloc_top) alloc_error();
			FORW = (ptr + size + 8) | 0x80000000;
			/* set up new BACK ptr */
			NEW_BACK = ptr;
			/* set up new FORW */
			NEW_FORW = 0;
			return((void *) (ptr + 8));
		}

		if (FORW - (ptr + 8) >= size + 24)
		/* Yes, the block is too large, we will split it */
		{	/* edit old NEXT_BACK ptr */
			NEXT_BACK = ptr + size + 8;
			/* set up new NEXT_BACK ptr */
			NEW_BACK = ptr;
			/* set up new NEXT_FORW ptr */
			NEW_FORW = FORW;
			/* set up new FORW ptr */
			FORW  = (ptr + size + 8) | 0x80000000;
			return((void *) (ptr + 8));
		}

		if (FORW - (ptr + 8) >= size)
		/* Yes, the block fits */
		{	FORW |= 0x80000000;
			return((void *) (ptr + 8));
		}

		/* No, set ptr to next block and try again */
		ptr = FORW;
	}
}

void free(void *block)
{	dword ptr;
	ptr = (dword) block;
	ptr -= 8;

	/* mark block as unused */
	FORW &= 0x7FFFFFFF;

	/* merge "next" blocks */
	if (! (NEXT_FORW & 0x80000000))
	{	if (NEXT_FORW == 0) /* end of heap */
		{	FORW = 0;
			if (ptr == alloc_base) return; /* heap empty */
			if (! (PREV_FORW & 0x80000000)) PREV_FORW = 0;
			return;
		}
		FORW = NEXT_FORW;
		/* FORW now points to first used block */
		NEXT_BACK = ptr;
	}

	/* merge "prev" blocks */
	if (ptr == 0) return; /* start of heap */
	if (! (PREV_FORW & 0x80000000))
	{	NEXT_BACK = BACK;
		PREV_FORW = FORW;
	}
}

void *calloc(dword n, dword size)
{	void *ptr;
	size *= n;
	ptr = malloc(size);
	memstob((byte *) ptr, 0, size);
	return(ptr);
}
