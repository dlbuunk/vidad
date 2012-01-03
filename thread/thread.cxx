//      thread.cxx
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

#include "system.hxx"
#include "memory.hxx"
#include "thread.hxx"

namespace thread
{

void scheduler()
{
}

void sched_add(Thread * th)
{
	(void) th;
}

void sched_rm(Thread * th)
{
	(void) th;
}

Thread::~Thread()
{
	sched_rm(this);
	memory::page_free(stack_base, stack_pages);
}

Thread::Thread(void (*init)(Thread *th), unsigned long int pages)
{
	stack_base = memory::page_alloc(pages);
	if (! stack_base)
		system::panic("thread::Thread::Thread:"
			" could not allocate pages for a stack");
	esp = (dword *) stack_base;
	esp += pages << 10;
	ebp = esp;
	*--esp = (dword) this;	// argument of die
	*--esp = (dword) this;	// argument of init AND return adress of die
	*--esp = (dword) &die;	// return adress of init
	eflags = 0x00200202;
	eip = init;
	sched_add(this);
	died = 0;
}

void Thread::yield()
{
	scheduler();
}

void Thread::die(Thread * th)
{
	// Should de-schedule, but not deallocate
	th->died = 1;
	sched_rm(th);
	scheduler();
	// Just in case some idiot re-schedules the thread.
	for ( ; ; )
	{
		kputs("thread::Thread::die: warning: zombie thread active.");
		sched_rm(th);
		scheduler();
	}
	// THIS FUNCTION SHOULD _NEVER_ RETURN!
}

}
