namespace IO
{	class PIC
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
		private : static const int num_entries = 16;
		private : dword cb[num_entries][3];
	};
}
