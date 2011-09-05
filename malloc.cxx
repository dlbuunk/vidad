#include "kernel.h"

#define SMALL_SIZE 32
#define SMALL_NUM 1024
// random number (credit goes to Wolfram Alpha)
#define MAGIC 0x438E4635

static byte * start_small;
static byte * start_addr;
static byte * end_addr;

struct mobj
{
	dword used;
	struct mobj * prev;
	struct mobj * next;
	dword mem;
};

void alloc_init(void * addr, void * limit)
{
	kprint("Starting memory allocator.");

	// make page-aligned
	start_small = (byte *) ((((dword)addr >> 12) + 1) << 12);
	for (int i = 0; i < SMALL_SIZE * SMALL_NUM; i++)
		start_small[i] = 0;

	start_addr = start_small + SMALL_SIZE * SMALL_NUM;
	start_addr = (byte *) (((((dword)start_addr - 1) >> 12) + 1) << 12);
	end_addr = (byte *) limit;

	((struct mobj *) start_addr)->used = 0;
	((struct mobj *) start_addr)->prev = 0;
	((struct mobj *) start_addr)->next = 0;

	kprint("%u\tkibibyte at 0x%X for small allocs.", SMALL_SIZE * SMALL_NUM / 1024, start_small);
	kprint("%u\tkibibyte at 0x%X for large allocs.", (end_addr - start_addr) / 1024, start_addr); 
}

void * malloc(dword size)
{
	if (size < SMALL_SIZE)
	{
		for (int i=0; i<SMALL_NUM; i++)
			if (start_small[i*SMALL_SIZE + SMALL_SIZE-1] == 0)
			{
				start_small[i*SMALL_SIZE + SMALL_SIZE-1] = 1;
				return (void *) &start_small[i*SMALL_SIZE];
			}
	}

	// align size
	size = (((size - 1) >> 2) + 1) << 2;

	// assign pointer to struct
	struct mobj * m = reinterpret_cast<struct mobj *>(start_addr);

	while (1)
	{
		// find empty memory
		while (m->used)
		{
			m = m->next;
		}

		// found, check if at end of chain
		if (m->next == 0)
		{
			// check size
			if ((dword)(end_addr - (byte *) m) < (size + 12))
			{
				kprint("malloc: cannot allocate memory.");
				return 0;
			}

			// allocate block
			m->used = MAGIC;
			m->next = (struct mobj *) ((dword) m + size + 12);
			m->next->used = 0;
			m->next->next = 0;
			m->next->prev = m;
			return &m->mem;
		}

		// not at end, check size
		if ((dword) m->next - (dword) m < (size + 12))
		{
			m = m->next;
			continue;
		}

		// do we need to split the block?
		if ((dword) m->next - (dword) m >= (size + 12 + SMALL_SIZE))
		{
			m->used = MAGIC;
			m->next->prev = (struct mobj *) ((dword) m + size + 12);
			m->next->prev->next = m->next;
			m->next = m->next->prev;
			m->next->used = 0;
			m->next->prev = m;
			return &m->mem;
		}

		// don't split the block
		m->used = MAGIC;
		return &m->mem;
	}
}

void free(void * addr)
{
	struct mobj * m = reinterpret_cast<struct mobj *>((dword) addr - 12);

	if (addr < start_addr && addr >= start_small)
		*((byte *) addr + 31) = 0;

	else if (m->used == MAGIC)
	{
		m->used = 0;

		// see if this is the last block
		if (m->next->next == 0)
			m->next = 0;

		// see if we can merge next
		else while (m->next->used == 0)
		{
			m->next = m->next->next;
			m->next->prev = m;
		}

		// see if we can merge prev
		while (m->prev && m->prev->used == 0)
		{
			m->prev->next = m->next;
			m = m->prev;
			m->next->prev = m;
		}
	}

	else
		kprint("malloc: cannot free at address 0x%X.", addr);
}

