// TODO: make key sets 2 and 3

namespace IO
{	class Key_Translate;
	class Keyboard;

	class KBC // Keyboard Controller
	{	public : KBC();
		public : ~KBC();
		public : void reset_cpu();
		public : void set_keyset(int set);
		public : void set_leds(byte status);
		public : void set_translator(Key_Translate *new_tl);
		private : void init();
		private : void wait_write();
		private : void wait_read();
		private : void send(byte val);
		private : byte last_send;
		private : static void handle_irq(KBC *th);
		private : int inter_num;
		private : Key_Translate *tl;
		private : volatile bool echo, error;
	};

	class Key_Translate // base class
	{	public : Key_Translate() {};
		public : virtual ~Key_Translate() {};
		public : virtual void feed_scancode(byte code) = 0;
	};

	// these classes are similiar, as they all deal with the KBC,
	// but there should be no merge of some sort, i might want to add a USB keyboard.
	class Key_Translate_Set1: public Key_Translate
	{	public : Key_Translate_Set1(Keyboard *out, KBC *kbc);
		public : ~Key_Translate_Set1();
		public : void feed_scancode(byte incode);
		private : Keyboard *out;
		private : word status; // CTRL ALT BREAK 0 ESCAPE0 CAPS NUM SCROLL
		private : KBC *kbc;
	};

	class Key_Translate_Set2: public Key_Translate
	{	public : Key_Translate_Set2(Keyboard *out, KBC *kbc);
		public : ~Key_Translate_Set2();
		public : void feed_scancode(byte incode);
		private : Keyboard *out;
		private : word status; // CTRL ALT BREAK ESCAPE1 ESCAPE0 CAPS NUM SCROLL
		private : KBC *kbc;
	};

	class Keyboard // control center
	{	public : Keyboard();
		public : ~Keyboard();
		public : void feed_code(word code);
		public : void set_user(WordBuffer *buffer, int new_mode);
		private : WordBuffer *buffer;
		private : int mode;
		private : int shift_state;
	};
}
