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
//      ViOS is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with ViOS; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#ifndef THREAD_HXX
#define THREAD_HXX

#include "kernel.hxx"

namespace thread
{

const int prio_max = 31;
const int prio_min = 1;

enum ThreadState
{
	dead = 0,
	alive,
	alarm, // asleep WITH alarm clock
	asleep // and without
};

class Thread
{
	public:
	Thread(void (*init)(void*), void *arg = 0, unsigned long int pages = 1);
	~Thread();

	void live(int p=4);
	void sleep(dword ticks=0);
	void wake();
	void kill();

	private:

	// scheduling-related
	friend void sched();
	friend void thread_switch(Thread *, Thread *);
	int prio;
	dword timer_ticks;
	ThreadState state;
	bool running;
	Thread * prev;
	Thread * next;

	// Linked list helper functions.
	void insert_alive();
	void insert_alarm();
	void insert_sleep();

	// exit function, gets called when init returns
	static void die(Thread * th);

	// stack
	unsigned long int stack_pages;
	void * stack_base;

	// registers
	struct
	{
		dword * ebp;
		dword * esp;
		void (*eip)(void *);
		dword eflags;
	} volatile regs;
};

extern Thread * current;
extern Thread * alives[];
extern Thread * alarms;
extern Thread * sleeps;

void sched();
void thread_switch(Thread * o, Thread * n);

}

#endif // THREAD_HXX
