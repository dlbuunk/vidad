namespace IO
{	class KBC // Keyboard Controller
	{	public : KBC();
		public : ~KBC();
		public : void test(); //debug function
		private : void wait_write();
		private : void wait_read();
		private : static void handle_irq(dword ptr);
		private : int inter_num;
	};
};