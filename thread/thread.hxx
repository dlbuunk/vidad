//      thread.hxx
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

#ifndef THREAD_HXX
#define THREAD_HXX

#include "kernel.hxx"

namespace thread
{

class Thread
{
	public:
	Thread(void (*init)(Thread *th), unsigned long int pages = 1);
	~Thread();

	void yield();

	private:
	// exit function, gets called when init returns
	static void die(Thread * th);
	bool died;

	// stack
	unsigned long int stack_pages;
	void * stack_base;

	// registers
	dword eax;
	dword ebx;
	dword ecx;
	dword edx;
	dword esi;
	dword edi;
	dword * ebp;
	dword * esp;
	void (*eip)(Thread *);
	dword eflags;
};

void scheduler();
void sched_add(Thread * th);
void sched_rm(Thread * th);

}

#endif // THREAD_HXX
