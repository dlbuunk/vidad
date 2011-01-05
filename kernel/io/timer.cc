#include "io.h"

#define BASE_FREQ 1193182.0 // Hz

namespace IO
{	Timer::Timer(float freq)
	{	word data = (word) (BASE_FREQ / freq);
		outportb(0x0043, 0x36);
		outportb(0x0040, data);
		outportb(0x0040, data>>8);
		if ((int_num = inter_reg((dword) &callback, (dword) this, 0x00)) == -1) kerror("Error, timer cannot get irq0 handle", 0x07);
		printf("Timer (PIT): INIT OK, frequency is %u Hz.\n", (int) freq);
	};

	Timer::~Timer()
	{	inter_dereg(0x00, int_num);
	};

	void Timer::callback(dword ptr)
	{	Timer *th; //this pointer for static function
		th = (Timer *) ptr;
		int i;
		for (i=0; i<num_entries; i++)
		{	if (th->cb[i][0] == 0) continue;
			if (th->cb[i][2]) th->cb[i][2]--;
			else
			{	(*((void (*)(dword)) th->cb[i][0]))(th->cb[i][1]);
				th->cb[i][0] = 0;
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
	{	word data = (word) (BASE_FREQ/freq);
		outportb(0x0043, 0xB6);
		outportb(0x0040, data);
		outportb(0x0040, data>>8);
	};
};
