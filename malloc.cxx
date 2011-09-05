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

void * operator new(unsigned long int size)
{
	return malloc((dword) size);
}

void * operator new[](unsigned long int size)
{
	return malloc((dword) size);
}

void operator delete(void * p)
{
	free(p);
}

void operator delete[](void * p)
{
	free(p);
}

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

	reinterpret_cast<struct mobj *>(start_addr)->used = 0;
	reinterpret_cast<struct mobj *>(start_addr)->prev = 0;
	reinterpret_cast<struct mobj *>(start_addr)->next = 0;

	kprint("%u\tkibibyte at 0x%X for small allocs.", SMALL_SIZE * SMALL_NUM / 1024, start_small);
	kprint("%u\tkibibyte at 0x%X for large allocs.", (end_addr - start_addr) / 1024, start_addr); 
}

// returns the number of free bytes left in the main allocator
dword alloc_status()
{
	dword size = 0;
	for (int i=0; i<SMALL_NUM; i++)
		if (start_small[i*SMALL_SIZE + SMALL_SIZE-1] == 0)
			size += SMALL_SIZE;
	kprint("alloc_status: %u kibibyte left in small allocator.", size / 1024);

	struct mobj * m = reinterpret_cast<struct mobj *>(start_addr);
	size = 0;
	while (m->next)
	{
		if (m->used == 0)
			size += (dword) m->next - (dword) m;
		m = m->next;
	}
	size += (dword) end_addr - (dword) m;
	kprint("alloc_status: %u kibibyte left in main allocator.", size / 1024);
	return size;
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
		kprint("malloc, warning: memory in small allocator exhausted.");
	}

	// align size
	size = (((size - 1) >> 2) + 1) << 2;

	// assign pointer to struct
	struct mobj * m = reinterpret_cast<struct mobj *>(start_addr);

	while (1)
	{
		// find empty memory
		while (m->used)
			m = m->next;

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

/* memory map:
0x00000000 - 0x000003FF (    1   kB) IVT, realmode artifact, do not touch.
0x00000400 - 0x000005FF (    0.5 kB) BDA, BIOS artifact, do not touch.
0x00000600 - 0x000007FF (    0.5 kB) memory size data, do not touch.
0x00000800 - 0x00000FFF (    2   kB) IDT, important CPU table, do not touch.
0x00001000 - 0x00001FFF (    4   kB) bootloader, still contains usefull functions/data.
0x00002000 - 0x00002FFF (    4   kB) GDT, important CPU table (512 entries), do not touch.
0x00003000 - 0x00003FFF (    4   kB) Page directory for kernel.
0x00004000 - 0x00007FFF (   16   kB) Page tables (4) for the first 16 MB. (kernel)
0x00008000 - 0x0000C3FF (   18   kB) DMA buffer for floppy disk. (bootloader)
0x0000C400 - 0x0000FFF0 (   14   kB) Main kernel stack.
0x0000FFF0 - 0x0000FFF1 (         2) empty
0x0000FFF2 - 0x0000FFF7 (         6) GDT pointer
0x0000FFF8 - 0x0000FFF9 (         2) empty
0x0000FFFA - 0x0000FFFF (         6) IDT pointer
0x00010000 - 0x0009FBFF (  575   kB) Kernel code + data.
0x0009FC00 - 0x0009FFFF (    1   kB) EBDA, BIOS artifact, do not touch.
0x000A0000 - 0x000BFFFF (  128   kB) Video memory.
0x000C0000 - 0x000FFFFF (  256   kB) BIOS ROM.
0x00100000 - 0x0013FFFF (  256   kB) DMA buffers.
0x00140000 - 0x00FFFFFF (15      MB) Memory mapped into kernel address range (no ID paging).
*/

