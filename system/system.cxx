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
//      ViOS is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with ViOS; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include "util.hxx"
using util::kputs;
using util::kprintf;

#include "system.hxx"
#include "thread.hxx"
using thread::Thread;
using thread::sched;

namespace system
{

thread::Thread * init_thread;
void init(void * mem_low_p);

void init_nt(dword mem_low)
{
	// Starting multi-threading.
	kprintf("%t system::init_nt: starting multithreading\n");
	Thread sys_idle(0,0,0);
	thread::current = &sys_idle;
	sys_idle.live(0);
	sched();

	// Creating main init thread.
	init_thread = new Thread(&init, (void *) mem_low, 4);
	init_thread->live(8);
	sched();

	// This is where the sys_idle thread comes when activated.
	for ( ; ; )
	{
		// Check if there are living threads with prio>0.
		for (int i=31; i>0; i--)
		{
			if (thread::alives[i])
				goto end_loop;
		}

		// Check if there are any alarming || sleeping threads.
		if (thread::alarms || thread::sleeps)
			goto end_loop;

		// There are no other threads, bail out...
		break;

		end_loop:;
		asm volatile ( "hlt\n\t" : : : ) ;
		sched();
	}

	// Shutdown routine.
	kputs("system::init_nt: no more living threads with prio > 0 left.");
	delete init_thread;
}

void init(void * mem_low_p)
{
	// Still needs to be distributed.
	kprintf("%t system::init: mem_low == %u.\n", mem_low_p);

	// Testing buffers.
	byte msg[16];
	util::Buffer buf(16);
	buf.w(63);
	buf.w(63);
	buf.w(33);
	buf.r();
	buf.r();
	buf.r();
	kprintf("%t buffer_test_write: %i.\n", buf.w((byte const *) "Hello, world!", 14));
	kprintf("%t buffer_test_read : %i, %s.\n", buf.r(msg, 14), msg);
}

void panic(char const * msg)
{
	kputs(msg);
	kputs("system::panic: kernel panic, system halted.\n");
	asm volatile ( "cli\n\t" : : : ) ;
	for ( ; ; ) asm volatile ( "hlt\n\t" : : : ) ;
}

}
