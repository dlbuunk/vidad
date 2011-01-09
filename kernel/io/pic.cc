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

#include <kernel.h>
#include <io/io.h>

namespace IO
{	PIC::PIC()
	{	asm("cli");
		// command to re-init
		outportb(0x0020, 0x11);
		outportb(0x00A0, 0x11);
		// vector offset
		outportb(0x0021, 0x20);
		outportb(0x00A1, 0x28);
		// other data
		outportb(0x0021, 0x04);
		outportb(0x00A1, 0x02);
		outportb(0x0021, 0x01);
		outportb(0x00A1, 0x01);
		// unmask all irq's
		outportb(0x0021, 0x00);
		outportb(0x00A1, 0x00);
		asm("sti");
		printf("Programmable Interrupt Controller: INIT and unmask all irq's.\n");
	}

	PIC::~PIC()
	{	// mask everything
		outportb(0x0021, 0xFF);
		outportb(0x00A1, 0xFF);
		printf("Programmable Interrupt Controller: turned off.\n");
	}
}
