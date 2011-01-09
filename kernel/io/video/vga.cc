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
{	VGA::VGA()
	{	// store VGA state
		int r = 0;
		byte i;
		// read MISC register
		vga_state[r] = inportb(misc_read);
		r++;
		// read SEQ registers
		for (i=0; i<5; i++)
		{	outportb(seq_index, i);
			vga_state[r] = inportb(seq_data);
			r++;
		}
		// read CRTC registers
		for (i=0; i<25; i++)
		{	outportb(crtc_index, i);
			vga_state[r] = inportb(crtc_data);
			r++;
		}
		r++; // skip ahead to 0x20
		// read GC registers
		for (i=0; i<9; i++)
		{	outportb(gc_index, i);
			vga_state[r] = inportb(gc_data);
			r++;
		}
		// read AC registers
		outportb(misc_write, vga_state[0] | 0x01);
		for (i=0; i<21; i++)
		{	inportb(ac_reset);
			outportb(ac_index, i);
			vga_state[r] = inportb(ac_read);
			r++;
		}
		inportb(ac_reset);
		outportb(ac_index, 0x20);
		r += 2; //skip ahead to 0x40
	}

	VGA::~VGA()
	{	// restore VGA state
		// always restore the second set of regs first, as set_reg2() messes with the MISC register
		set_reg2(vga_state + 32);
		set_regs(vga_state);
	}

	void VGA::set_reg2(byte *regs)
	{	byte i;
		for (i=0; i<9; i++)
		{	outportb(gc_index, i);
			outportb(gc_data, *regs);
			regs++;
		}
		outportb(misc_write, inportb(misc_read) | 0x01);
		for (i=0; i<21; i++)
		{	inportb(ac_reset);
			outportb(ac_index, i);
			outportb(ac_write, *regs);
			regs++;
		}
		inportb(ac_index);
		outportb(ac_index, 0x20);
	}

	void VGA::set_regs(byte *regs)
	{	byte i;
		byte temp;
		// sync reset
		outportb(seq_index, 0x00);
		outportb(seq_data, 0x01);
		*(regs + 1) = 0x01;
		// write MISC
		outportb(misc_write, *regs);
		regs++;
		// write SEQ
		for (i=0; i<5; i++)
		{	outportb(seq_index, i);
			outportb(seq_data, *regs);
			regs++;
		}
		// unlock CRTC
		outportb(crtc_index, 0x11);
		temp = inportb(crtc_data);
		temp &= 0x7F;
		outportb(crtc_index, 0x11);
		outportb(crtc_data, temp);
		// write CRTC
		for (i=0; i<25; i++)
		{	outportb(crtc_index, i);
			outportb(crtc_data, *regs);
			regs++;
		}
		// end sync reset
		outportb(seq_index, 0x00);
		outportb(seq_data, 0x03);
	}

	dword VGA::set_mode(dword mode)
	{	switch (mode)
		{	case 1 : { set_reg2(vga_state + 32); set_regs(vga_state); } break;
			case 2 : { set_reg2(vga_mode_t); set_regs(vga_mode2); } break;
			case 3 : { set_reg2(vga_mode_t); set_regs(vga_mode3); } break;
			case 4 : { set_reg2(vga_mode_t); set_regs(vga_mode4); } break;
			case 5 : { set_reg2(vga_mode_t); set_regs(vga_mode5); } break;
			case 6 : { set_reg2(vga_mode_t); set_regs(vga_mode6); } break;
			case 7 : { set_reg2(vga_mode_t); set_regs(vga_mode7); } break;
			case 8 : { set_reg2(vga_mode_t); set_regs(vga_mode8); } break;
			case 9 : { set_reg2(vga_mode_t); set_regs(vga_mode9); } break;
			default: return(0);
		}
		current_mode = mode;
		return(mode);
	}
}
