namespace IO
{	class Terminal // base class
	{	public : Terminal() {};
		public : virtual ~Terminal() {};
		// req for every terminal
		public : virtual void puts_err(char *str) = 0;
		public : virtual void puts(char *str) = 0;
		public : virtual char *gets(char *str) = 0;
		// optional
		public : virtual void set_active() {};
		public : virtual void set_color(byte color) { (void) color; };
	};

	class Term_Buf
	{	public : Term_Buf(Video_Text *video, byte col, byte row);
		public : ~Term_Buf();
		public : word *buffer;
		public : Video_Text *vid;
		public : word cursor_pos;
		public : byte num_col;
		public : byte num_row;
	};

	class Term_Std: public Terminal
	{	public : Term_Std(Term_Buf *stderr, Term_Buf *stdout, WordBuffer *stdin);
		public : ~Term_Std();
		//req for every terminal
		public : void puts_err(char *str);
		public : void puts(char *str);
		public : char *gets(char *str);
		//optional
		public : void set_active();
		public : void set_color(byte color);
		//model-specific
		private : Term_Buf *err;
		private : Term_Buf *out;
		private : WordBuffer *in;
		private : byte current_color;
	};
}
