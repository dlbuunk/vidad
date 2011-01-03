#include "../kernel.h"

namespace IO
{	class Init
	{	public : Init() {};
		public : ~Init() {};
		public : void main();
		private : static void test(dword ptr);
		private : volatile word lock;
	};

	// should this be an all-inline class?
	class ByteBuffer // circular buffer
	{	public : ByteBuffer(dword size)
		{	this->size = size;
			b = new byte [size];
			wc = rc = 0;
		};
		public : ~ByteBuffer()
		{	delete b;
		};
		public : word read()
		{	word out = 0;
			if (wc == rc) return(0xFFFF);
			out |= b[rc];
			rc++;
			if (rc == size) rc = 0;
			return(out);
		};
		public : int write(byte data)
		{	if (wc == rc - 1) return(-1);
			if (rc == 0 && wc + 1 == size) return(-1);
			b[wc] = data;
			wc++;
			if (wc == size) wc = 0;
			return(0);
		};
		private : byte *b; // actual buffer
		private : dword size;
		private : dword wc; // write count
		private : dword rc; // read count
	};

	class PIC
	{	public : PIC();
		public : ~PIC();
	};

	class Timer // PIT, are there any other timers around?
	{	public : Timer(float freq);
		public : ~Timer();
		public : int reg_cb(dword func, dword obj, dword time);
		public : void set_chan2(float freq);
		private : static void callback(dword ptr);
		private : int int_num;
		private : static const dword num_entries = 16;
		private : dword cb[num_entries][3];
	};
};

#include "video/video.h"
#include "terminal/terminal.h"

extern IO::Terminal *kterm;
