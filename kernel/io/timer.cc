#include <kernel.h>
#include <io/io.h>

#define BASE_FREQ 1193182.0 // Hz

// Location of timer class definition could be clearer. (It's in misc.h, which
// isn't even directly included here...)

namespace IO
{	Timer::Timer(float freq)
	{	word data = (word) (BASE_FREQ / freq);
		// Output port magic, care to document?
		outportb(0x0043, 0x36);
		outportb(0x0040, data);
		outportb(0x0040, data>>8);
		// Register timer as irq0
		if ((int_num = inter_reg((dword) &callback, (dword) this, 0x00)) == -1) kerror("Error, timer cannot get irq0 handle", 0x07);
		printf("Timer (PIT): INIT OK, frequency is %u Hz.\n", (int) freq);
	}

	Timer::~Timer()
	{	inter_dereg(0x00, int_num);
	}

	// If you're going to cast to Timer* anyway, why not have it take a
	// Timer* as the parameter?
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
	}

	// Search for a free entry to register the callback, return the number
	// if one is found or return -1 if it is not. Aha! And based on another
	// function, I'm guessing that this registers the callback to be called
	// ONCE, not to be called every dword time ticks.
	int Timer::reg_cb(dword func, dword obj, dword time)
	{	int i = 0;
		while (cb[i][0]) { i++; if (i == num_entries) return(-1); }
		cb[i][0] = func;
		cb[i][1] = obj;
		cb[i][2] = time;
		return(i);
	}

	// More output port magic...
	void Timer::set_chan2(float freq)
	{	word data = (word) (BASE_FREQ/freq);
		outportb(0x0043, 0xB6);
		outportb(0x0040, data);
		outportb(0x0040, data>>8);
	}
}
