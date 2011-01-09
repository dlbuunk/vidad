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
