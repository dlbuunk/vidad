#include "io.h"

namespace IO
{	Timer::Timer(float freq)
	{	word data = (word) (base_freq / freq);
		outportb(0x0043, 0x36);
		outportb(0x0040, data);
		outportb(0x0040, data>>8);
		dword ptr;
		asm ("movl %1,%%eax\n\tmovl %%eax,%0\n\t" : "=r" (ptr) : "r" (&IO::Timer::callback) : "%eax");
		if ((int_num = inter_reg(ptr, (dword) this, 0x00)) == -1) kerror("Error, timer cannot get irq0 handle", WHITE_GREEN);
	};

	Timer::~Timer()
	{	inter_dereg(0x00, int_num);
	};

	void Timer::callback()
	{	int i;
		for (i=0; i<num_entries; i++)
		{	if (cb[i][0] == 0) continue;
			if (cb[i][2]) cb[i][2]--;
			else
			{	(*((void (*)(dword)) cb[i][0]))(cb[i][1]);
				cb[i][0] = 0;
			}
		}
	};

	int Timer::reg_cb(dword func, dword obj, dword time)
	{	int i = 0;
		while (cb[i][0]) { i++; if (i == num_entries) return(-1); }
		cb[i][0] = func;
		cb[i][1] = obj;
		cb[i][2] = time;
		return(i);
	};

	void Timer::set_chan2(float freq)
	{	word data = (word) (base_freq/freq);
		outportb(0x0043, 0xB6);
		outportb(0x0040, data);
		outportb(0x0040, data>>8);
	};
};
