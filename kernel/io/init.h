/* This file is part of Vidad.
*
*  Vidad is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Vidad is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Vidad.  If not, see <http://www.gnu.org/licenses/>.
*/

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
		Key_Translate_Set3 *key_trans3;
		WordBuffer *key_buf;

		// terminal
		Term_Buf *term_buf;
		Term_Std *term_std;
	};
}
