//      system.cxx
//
//      Copyright 2012 D.L.Buunk <dlbuunk@gmail.com>
//
//      This file is part of ViOS.
//
//      ViOS is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include "util.hxx"
using util::kputs;
using util::kprintf;

#include "system.hxx"
#include "thread.hxx"

namespace system
{

void thAf(void * discard)
{
	(void) discard;
	for (int i=0; i<4; i++)
	{
		kprintf("%t Hello from thread A.\n");
		thread::sched();
	}
	for (;;);
}

void thBf(void * discard)
{
	(void) discard;
	for (int i=0; i<4; i++)
	{
		kprintf("%t Hello from thread B.\n");
		thread::sched();
	}
	for (;;);
}

void init(dword mem_low)
{
	kprintf("%t system::init: mem_low == %u .\n", mem_low);

	// Testing multi-threading.
	kprintf("%t system::init: testing multithreading\n");
	kprintf("%t system::init: alives is at 0x%X.\n", thread::alives);
	thread::Thread sys_idle(0,0,0);
	thread::current = &sys_idle;
	sys_idle.live(0);
	thread::sched();
	kprintf("%t system::init: still running\n");

	thread::Thread thA(&thAf);
	thA.live();
	thread::Thread thB(&thBf);
	thB.live();
	thread::sched();
}

void panic(char const * msg)
{
	kputs(msg);
	kputs("system::panic: kernel panic, system halted.\n");
	asm volatile ( "cli\n\t" : : : ) ;
	for ( ; ; ) asm volatile ( "hlt\n\t" : : : ) ;
}

}
