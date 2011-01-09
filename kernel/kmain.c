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

void kmain(struct kheader *kh, dword stack_base)
{	word memsize;

	memstow((word *) 0xB8000, 0x0720, 2000);
	kprint_pos = 0;
	kprints("SYSTEM LOADED\n", 0x01);

	/* turn on cache (if > 486) */
	if (kh->cpu_type >= 4) asm("mov %%cr0,%%eax\n\tand $0x9FFFFFFF,%%eax\n\tmov %%eax,%%cr0\n\t" : : : "%eax");

	/* initialise interrupt handling code */
	inter_init();

	/* set the memsize in 32 KB units */
	memsize = kh->mem_low >> 5;

	/* we need 32 KB for heap and 32 KB for DMA buffer */
	if (memsize < (stack_base>>15) + 2)
	{	kprints("Error: not enough low memory, ViDad will not run on this system.\n", 0x04);
		return;
	}

	/* init the heap, but leave enough space for a 32 KB DMA buffer */
	alloc_init(stack_base, (memsize-1)<<15);

	/* call the c++ entry function */
	ccentry(kh, (memsize-1)<<15);
}
