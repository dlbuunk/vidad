namespace IO
{	class Init
	{	public : Init();
		public : ~Init();
		private: // here come pointers to all classes in excistance

		// misc
		PIC *pic;
		Timer *timer;

		// video
		Video_Text *video;
		VGA_Text *vga_text;

		// keyboard
		KBC *kbc;
		Keyboard *keyb;
		Key_Translate *key_translate;
		Key_Translate_Set1 *key_trans1;
		Key_Translate_Set2 *key_trans2;
		WordBuffer *key_buf;

		// terminal
		Term_Buf *term_buf;
		Term_Std *term_std;
	};
}
