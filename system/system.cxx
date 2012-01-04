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

thread::Thread * thA, * thB, * thC;

void thAf(void * str)
{
	for (int i=0; i<4; i++)
	{
		kprintf("%t Hello from thread %s.\n", str);
		if (i == 0)
			thC->sleep();
		thread::sched();
	}
}

void thBf(void * str)
{
	for (int i=0; i<4; i++)
	{
		kprintf("%t Hello from thread %s.\n", str);
		if (i == 2)
			thC->wake();
		thread::sched();
	}
}

void thCf(void * str)
{
	for (int i=0; i<4; i++)
	{
		kprintf("%t Hello from thread %s.\n", str);
		thread::sched();
	}
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

	thA = new thread::Thread(&thAf, (void *) "A");
	thA->live();
	thB = new thread::Thread(&thBf, (void *) "B");
	thB->live();
	thC = new thread::Thread(&thCf, (void *) "C");
	thC->live(2);
	thread::sched();
	kprintf("%t Hello from sys_idle.\n");
	for (;;);
}

void panic(char const * msg)
{
	kputs(msg);
	kputs("system::panic: kernel panic, system halted.\n");
	asm volatile ( "cli\n\t" : : : ) ;
	for ( ; ; ) asm volatile ( "hlt\n\t" : : : ) ;
}

}
