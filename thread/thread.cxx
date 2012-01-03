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

Thread::~Thread()
{
	kill();
	memory::page_free(stack_base, stack_pages);
}

Thread::Thread(void (*init)(void *), void * arg, unsigned long int pages)
{
	stack_base = memory::page_alloc(pages);
	if (! stack_base)
		system::panic("thread::Thread::Thread:"
			" could not allocate pages for a stack");
	esp = (dword *) stack_base;
	esp += pages << 10;
	ebp = esp;
	*--esp = (dword) this;	// argument of die
	*--esp = (dword) arg;	// argument of init AND return adress of die
	*--esp = (dword) &die;	// return adress of init
	eflags = 0x00200202;
	eip = init;
	state = dead;
	running = 0;
}

// live does activate the thread, but only if it is dead.
// live does always change the priority of the thread.
void Thread::live(int p)
{
	if (state == alive)
	{
		if (prio == p)
			return;
		// Replace in list of living threads.
		if (prev)
			prev->next = next;
		else
			alives = next;
		if (next)
			next->prev = prev;
		prio = p;
		insert_alive();
	}

	if (state == asleep && state == alarm)
	{
		prio = p;
		return;
	}

	if (state == dead)
	{
		prio = p;
		state = alive;
		// Insert into the list of living threads.
		insert_alive();
		return;
	}

	// If we get to this point, the state has an undefined value,
	// warn the user and do nothing
	kputs("thread::Thread::live: warning: state has undefined value.");
}

void Thread::sleep(dword ticks)
{
	if (state == alive)
	{
		// Remove from living threads
		if (prev)
			prev->next = next;
		else
			alives = next;
		if (next)
			next->prev = prev;
	}

	else if (state == asleep && ticks == 0)
		return;

	else if (state == asleep)
	{
		// remove from sleeping threads
		if (prev)
			prev->next = next;
		else
			sleeps = next;
		if (next)
			next->prev = prev;
	}

	else if (state == alarm)
	{
		// remove from alarming threads
		if (prev)
			prev->next = next;
		else
			alarms = next;
		if (next)
			next->prev = prev;
	}

	else if (state == dead)
	{
		kputs("thread::Thread::sleep: warning:"
			" cannot to put asleep a dead thread.");
		return;
	}

	else
	{
		kputs("thread::Thread::live: warning:"
			" state has undefined value.");
		return;
	}

	// Now, put the thread where it is supposed to be.
	if (ticks)
	{
		timer_ticks = ticks;
		state = alarm;
		insert_alarm();
	}
	else
	{
		state = asleep;
		insert_sleep();
	}
}

void Thread::wake()
{
	if (state == alive)
		return;

	else if (state == asleep)
	{
		if (prev)
			prev->next = next;
		else
			sleeps = next;
		if (next)
			next->prev = prev;
	}

	else if (state == alarm)
	{
		if (prev)
			prev->next = next;
		else
			alarms = next;
		if (next)
			next->prev = prev;
	}

	else if (state == dead)
	{
		kputs("thread::Thread::sleep: warning:"
			" refusing to wake a dead thread.");
		return;
	}

	else
	{
		kputs("thread::Thread::live: warning:"
			" state has undefined value.");
		return;
	}

	state = alive;
	insert_alive();
}

void Thread::kill()
{
	if (state == alive)
	{
		if (prev)
			prev->next = next;
		else
			alives = next;
		if (next)
			next->prev = prev;
	}

	else if (state == alarm)
	{
		if (prev)
			prev->next = next;
		else
			alarms = next;
		if (next)
			next->prev = prev;
	}

	else if (state == asleep)
	{
		if (prev)
			prev->next = next;
		else
			sleeps = next;
		if (next)
			next->prev = prev;
	}

	else if (state == dead)
	{
		kputs("thread::Thread::sleep: warning:"
			" trying to kill a dead thread");
	}

	else
	{
		kputs("thread::Thread::live: warning:"
			" state has undefined value.");
		return;
	}

	state = dead;
}

void Thread::die(Thread * th)
{
	// Should de-schedule, but not deallocate
	th->kill();
	sched();
	// Just in case some idiot manages to re-schedule the thread.
	for ( ; ; )
	{
		kputs("thread::Thread::die: warning: zombie thread active.");
		th->kill();
		sched();
	}
	// THIS FUNCTION SHOULD _NEVER_ RETURN!
}

void Thread::insert_alive()
{
	next = 0;
	prev = 0;
	Thread * th = alives;
	if (! th)
	{
		// There aro no living threads.
		alives = this;
		return;
	}
	while (th->next && th->next->prio > prio)
		th = th->next;
	if (th->next)
	{
		next = th->next;
		next->prev = this;
	}
	prev = th;
	th->next = this;
}

void Thread::insert_alarm()
{
	next = 0;
	prev = 0;
	Thread * th = alarms;
	if (! th)
	{
		// There are no alarming threads
		alarms = this;
		return;
	}
	while (th->next && th->next->timer_ticks < timer_ticks)
		th = th->next;
	if (th->next)
	{
		next = th->next;
		next->prev = this;
	}
	prev = th;
	th->next = this;
}

void Thread::insert_sleep()
{
	prev = 0;
	next = sleeps;
	sleeps = this;
	if (next)
		next->prev = this;
}

Thread * current = 0;
Thread * alives = 0;
Thread * alarms = 0;
Thread * sleeps = 0;

// This function figures out which task to switch.
// It takes the last thread on the highest prio level.
// (unless that thread is the current one)
// THIS ALGORITHM IS SINGLE CPU!
void sched()
{
	Thread * old = current;
	Thread * th = alives;

	// Check if there are no living threads.
	if (! alives)
		system::panic("thread::sched: nu living threads.");

	// Check if this is the only living thread.
	if (alives == current && ! alives->next)
		return;

	// Check if there is only one living thread.
	if (! alives->next)
	{
		current = alives;
		thread_switch(old, current);
		return;
	}

	// Check if the current thread is the only one in it's prio level.
	if (alives == current && alives->next->prio < alives->prio)
		th = alives->next;

	// Loop over the threads until the prio goes down.
	while (th->next && th->next->prio == th->prio)
		th = th->next;

	thread_switch(old, current = th);
}

void thread_switch(Thread * o, Thread * n)
{
}

}
