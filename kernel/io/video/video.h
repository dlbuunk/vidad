namespace IO
{	class Video_Text // base class
	{	public : Video_Text() {};
		public : virtual ~Video_Text() {};
		// required
		public : virtual void redraw(word *buffer, word cursor_pos) = 0;
		public : word *current_buffer;
		public : virtual byte get_num_col() = 0;
		public : virtual byte get_num_row() = 0;
	};

	class VGA
	{	public : VGA();
		public : ~VGA();
		protected : dword set_mode(dword mode);
		protected : dword current_mode;
		private : byte vga_state[64];
		private : void set_regs(byte *regs);
		private : void set_reg2(byte *regs);
		protected : // register nums
		static const word misc_write = 0x3C2;
		static const word misc_read = 0x3CC;
		static const word seq_index = 0x3C4;
		static const word seq_data = 0x3C5;
		static const word crtc_index = 0x3D4;
		static const word crtc_data = 0x3D5;
		static const word gc_index = 0x3CE;
		static const word gc_data = 0x3CF;
		static const word ac_reset = 0x3DA;
		static const word ac_index = 0x3C0;
		static const word ac_write = 0x3C0;
		static const word ac_read = 0x3C1;
	};

	class VGA_Text: public Video_Text, public VGA
	{	public : VGA_Text(dword mode);
		public : ~VGA_Text();
		// required
		public : void redraw(word *buffer, word cursor_pos);
		public : byte get_num_col();
		public : byte get_num_row();
		// model-specific
		private : void set_cursor(word cursor_pos);
		private : void set_font(const byte *font, byte height);
	};
};
