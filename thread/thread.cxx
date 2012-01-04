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
	// stack_base is 0 iff this is the null-thread.
	if (stack_base)
		memory::page_free(stack_base, stack_pages);
}

Thread::Thread(void (*init)(void *), void * arg, unsigned long int pages)
{
	if (! pages)
	{
		// This is not an error, it is actually an order to turn the
		// currently running code into an thread. (on system::init)
		state = dead;
		running = true;
		stack_pages = 0xDEADDEAD;
		stack_base = 0;
		// registers need no init, they are set when switching out.
		return;
	}
	stack_pages = pages;
	stack_base = memory::page_alloc(pages);
	if (! stack_base)
		system::panic("thread::Thread::Thread:"
			" could not allocate pages for a stack");
	regs.esp = (dword *) stack_base;
	regs.esp += pages << 10;
	regs.ebp = regs.esp;
	*--regs.esp = (dword) this; // argument of die
	*--regs.esp = (dword) arg;  // argument of init AND return adress of die
	*--regs.esp = (dword) &die; // return adress of init
	regs.eflags = 0x00200202;
	regs.eip = init;
	state = dead;
	running = false;
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
		next->prev = prev;
		prev->next = next;
		if (next == this)
			alives[prio] = 0;
		else if (alives[prio] == this)
			alives[prio] = next;
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
		next->prev = prev;
		prev->next = next;
		if (next == this)
			alives[prio] = 0;
		else if (alives[prio] == this)
			alives[prio] = next;
	}

	else if (state == asleep && ticks == 0)
		return;

	else if (state == asleep)
	{
		// remove from sleeping threads
		next->prev = prev;
		prev->next = next;
		if (next == this)
			sleeps = 0;
		else if (sleeps == this)
			sleeps = next;
	}

	else if (state == alarm)
	{
		// remove from alarming threads
		next->prev = prev;
		prev->next = next;
		if (next == this)
			alarms = 0;
		else if (alarms == this)
			alarms = next;
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
		next->prev = prev;
		prev->next = next;
		if (next == this)
			sleeps = 0;
		else if (sleeps == this)
			sleeps = next;
	}

	else if (state == alarm)
	{
		next->prev = prev;
		prev->next = next;
		if (next == this)
			alarms = 0;
		else if (alarms == this)
			alarms = next;
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
		next->prev = prev;
		prev->next = next;
		if (next == this)
			alives[prio] = 0;
		else if (alives[prio] == this)
			alives[prio] = next;
	}

	else if (state == alarm)
	{
		next->prev = prev;
		prev->next = next;
		if (next == this)
			alarms = 0;
		else if (alarms == this)
			alarms = next;
	}

	else if (state == asleep)
	{
		next->prev = prev;
		prev->next = next;
		if (next == this)
			sleeps = 0;
		else if (sleeps == this)
			sleeps = next;
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
	if (! alives[prio])
	{
		alives[prio] = this;
		next = this;
		prev = this;
		return;
	}

	prev = alives[prio]->prev;
	next = alives[prio];
	alives[prio]->prev->next = this;
	alives[prio]->prev = this;
}

void Thread::insert_alarm()
{
	// This is going to be slightly more difficult...
	if (! alarms)
	{
		alarms = this;
		next = this;
		prev = this;
		return;
	}

	if (timer_ticks > alarms->prev->timer_ticks)
	{
		// Insert 1 before alarms.
		prev = alarms->prev;
		next = alarms;
		alarms->prev->next = this;
		alarms->prev = this;
		return;
	}

	if (timer_ticks < alarms->timer_ticks)
	{
		// Insert AT alarms.
		next = alarms;
		prev = alarms->prev;
		alarms->prev->next = this;
		alarms->prev = this;
		alarms = this;
		return;
	}

	// Okay, should inser somewhere in the middle.
	Thread * th = alarms;
	while (th->timer_ticks < timer_ticks)
		th = th->next;
	// th now pointing to what should become next.
	prev = th->prev;
	next = th;
	th->prev->next = this;
	th->prev = this;
}

void Thread::insert_sleep()
{
	if (! sleeps)
	{
		sleeps = this;
		next = this;
		prev = this;
		return;
	}

	prev = sleeps->prev;
	next = sleeps;
	sleeps->prev->next = this;
	sleeps->prev = this;
}

Thread * current = 0;
Thread * alives[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Thread * alarms = 0;
Thread * sleeps = 0;

// Single CPU only.
void sched()
{
	for (int i=31; i>=0; i--)
	{
		if (! alives[i])
			continue;
		if (alives[i]->next->running && i)
			continue;
		// Okay, found thread to run.
		alives[i] = alives[i]->next;
		current->running = false;
		alives[i]->running = true;
		thread_switch(current, alives[i]);
		return;
	}
	// No threads found, panic.
	system::panic("thread::sched: no threads to run"
		" and no sys_idle thread present.");
}

void thread_switch(Thread * o, Thread * n)
{
	if (o == n)
	{
		// DEBUG
	//	kputs("OLD == NEW");
		//~DEBUG
		return;
	}
	// old in edi, new in esi
	asm volatile (
	// DEBUG
//	"xchg	%%bx,%%bx\n\t"
	//~DEBUG
	"movl	%%ebp,(%%edi)\n\t"
	"movl	%%esp,4(%%edi)\n\t"
	"movl	$1f,8(%%edi)\n\t"
	"pushfl\n\t"
	"popl	%%eax\n\t"
	"movl	%%eax,12(%%edi)\n\t"
	"movl	12(%%esi),%%eax\n\t"
	"pushl	%%eax\n\t"
	"popfl\n\t"
	"movl	(%%esi),%%ebp\n\t"
	"movl	4(%%esi),%%esp\n\t"
	"movl	8(%%esi),%%eax\n\t"
	"jmpl	*%%eax\n"
	"1:\n\t"
	:
	: "S" (&n->regs) , "D" (&o->regs)
	: "%eax", "%ebx", "%ecx", "%edx"
	, "%ebp", "%esp", "memory"
	) ;
}

}
