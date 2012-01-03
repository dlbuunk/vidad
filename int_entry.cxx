//      int_entry.cxx
//      
//      Copyright 2011 D.L.Buunk <dlbuunk@gmail.com>
//
//      This file is part of ViOS.
//      
//      ViOS is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      ViOS is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with ViOS; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

//	All execptions, interrupt requests and system calls are send to
//	this file by the handlers in kernel_entry.s .
//	Although this code currently only prints an error message,
//	it is later expected to handle exceptions and rout irq's and syscalls
//	to the appropriate handlers elsewhere.

#include "kernel.hxx"
#include "util.hxx"

// Structure definitions
struct IntData
{
	dword eax;
	dword ebx;
	dword ecx;
	dword edx;
	dword esi;
	dword edi;
	dword ebp;
	dword es;
	dword ds;
	dword ret_addr;
	dword int_num;
	dword eip;
	dword cs;
	dword eflags;
	dword esp;
	dword ss;
};

struct ExcepData
{
	dword eax;
	dword ebx;
	dword ecx;
	dword edx;
	dword esi;
	dword edi;
	dword ebp;
	dword es;
	dword ds;
	dword ret_addr;
	dword int_num;
	dword error_code;
	dword eip;
	dword cs;
	dword eflags;
	dword esp;
	dword ss;
};

// Main function
extern "C" void __int_entry(IntData * id)
{
	// Figure out if we have an error code.
	ExcepData * ed = 0;
	if (id->int_num == 0x8 || id->int_num == 0xA || id->int_num == 0xB
		|| id->int_num == 0xC || id->int_num == 0xD
		|| id->int_num == 0xE || id->int_num == 0x11)
		ed = (ExcepData *) id;

	// Update the kernel counter and insert ss:esp if needed.
	static int kernel_counter = 1;
	if (kernel_counter && ed)
		asm (
			"xorl	%%eax,%%eax\n\t"
			"movw	%%ss,%%ax\n\t"
			"movl	%%esp,%%ebx\n\t"
			: "=a" (ed->ss)
			, "=b" (ed->esp)
			:
			:
			) ;
	else if (kernel_counter)
		asm (
			"xorl	%%eax,%%eax\n\t"
			"movw	%%ss,%%ax\n\t"
			"movl	%%esp,%%ebx\n\t"
			: "=a" (id->ss)
			, "=b" (id->esp)
			:
			:
			) ;
	kernel_counter++;

	// Place hooks for handled exceptions here...




	// Unhandled exceptions go here...
	// Print debug data.
	if (ed)
	{
		util::kprintf("%t UNHANDLED INTERRUPT, number %i\n", id->int_num);
		util::kprintf("%t eax 0x%X  ebx 0x%X  ecx 0x%X  edx 0x%X\n",
				ed->eax, ed->ebx, ed->ecx, ed->edx);
		util::kprintf("%t esi 0x%X  edi 0x%X  ebp 0x%X  esp 0x%X\n",
				ed->esi, ed->edi, ed->ebp, ed->esp);
		util::kprintf("%t  cs 0x%X   ss 0x%X   ds 0x%X   es 0x%X\n",
				ed->cs, ed->ss, ed->ds, ed->es);
		util::kprintf("%t eip 0x%X  eflags 0x%X  error code 0x%X\n",
				ed->eip, ed->eflags, ed->error_code);
	}
	else
	{
		util::kprintf("%t UNHANDLED INTERRUPT, number %i\n", id->int_num);
		util::kprintf("%t eax 0x%X  ebx 0x%X  ecx 0x%X  edx 0x%X\n",
				id->eax, id->ebx, id->ecx, id->edx);
		util::kprintf("%t esi 0x%X  edi 0x%X  ebp 0x%X  esp 0x%X\n",
				id->esi, id->edi, id->ebp, id->esp);
		util::kprintf("%t  cs 0x%X   ss 0x%X   ds 0x%X   es 0x%X\n",
				id->cs, id->ss, id->ds, id->es);
		util::kprintf("%t eip 0x%X  eflags 0x%X\n",
				id->eip, id->eflags);
	}

	// And hang the system.
	asm volatile (
		"cli\n\t"
		"hlt\n\t"
		:
		:
		:
		) ;
}
