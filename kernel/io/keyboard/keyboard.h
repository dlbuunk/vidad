namespace IO
{	class KBC // Keyboard Controller
	{	public : KBC(ByteBuffer *outbuffer);
		public : ~KBC();
		public : void reset_cpu();
		public : void set_leds(byte status);
		private : void wait_write();
		private : void wait_read();
		private : static void handle_irq(dword ptr);
		private : int inter_num;
		private : ByteBuffer *ob; // output buffer
	};
};
