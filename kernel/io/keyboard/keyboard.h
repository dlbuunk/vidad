namespace IO
{	class Key_Translate;

	class KBC // Keyboard Controller
	{	public : KBC();
		public : ~KBC();
		public : void reset_cpu();
		public : void set_leds(byte status);
		public : void set_translator(Key_Translate *new_tl);
		private : void wait_write();
		private : void wait_read();
		private : static void handle_irq(dword ptr);
		private : int inter_num;
		private : Key_Translate *tl;
	};

	class Key_Translate // base class
	{	public : Key_Translate() {};
		public : virtual ~Key_Translate() {};
		public : virtual void feed_scancode(byte code) = 0;
	};

	class Key_Translate_Set1: public Key_Translate
	{	public : Key_Translate_Set1(KBC *kbc);
		public : ~Key_Translate_Set1();
		public : void feed_scancode(byte code);
		private : byte status; // CTRL ALT BREAK 0 0 CAPS NUM SCROLL
		private : KBC *kbc;
	};

	class Keyboard // control center
	{	public : Keyboard();
		public : ~Keyboard();
		public : void feed_code();
		public : void set_user(WordBuffer buffer, int new_mode);
		private : WordBuffer current_buffer;
		private : int mode;
	};
};
