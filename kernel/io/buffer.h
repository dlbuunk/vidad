namespace IO
{	// should these be all-inline classes?

	class ByteBuffer // circular buffer (byte)
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

	class WordBuffer // circular buffer (word)
	{	public : WordBuffer(dword size)
		{	this->size = size;
			b = new word [size];
			wc = rc = 0;
		};
		public : ~WordBuffer()
		{	delete b;
		};
		public : dword read()
		{	dword out = 0;
			if (wc == rc) return(0xFFFFFFFF);
			out |= b[rc];
			rc++;
			if (rc == size) rc = 0;
			return(out);
		};
		public : int write(word data)
		{	if (wc == rc - 1) return(-1);
			if (rc == 0 && wc + 1 == size) return(-1);
			b[wc] = data;
			wc++;
			if (wc == size) wc = 0;
			return(0);
		};
		private : word *b; // actual buffer
		private : dword size;
		private : dword wc; // write count
		private : dword rc; // read count
	};
};
