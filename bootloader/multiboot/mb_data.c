//	mb_data.c
//
//	Copyright 2012 D.L.Buunk <dlbuunk@gmail.com>
//
//	This file is part of ViOS.
//
//	ViOS is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	ViOS is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with ViOS; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//	MA 02110-1301, USA.

// General typedefs
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long long int qword;

// Printing function
extern void puts(char *);

// Start of the free data area
extern dword bss_end;
dword * data;		// constant
byte * end;		// moving

// Helper function
void memcpy(void * dest_o, void * src_o, dword num)
{
	byte * dest = (byte *) dest_o;
	byte * src = (byte *) src_o;

	for (dword i=0; i<num; i++)
		*dest++ = *src++;
}

void strcpy(char * dest, char * src)
{
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
}

int strlen(char * str)
{
	char * str_orig = str;
	while (*str)
		str++;
	return str - str_orig;
}

// Main function
dword * mb_data(dword magic, dword * info)
{
	if (magic != 0x2BADB002)
	{
		puts("Error in magic number, halting");
		asm ( "hlt\n\t" : : : ) ;
	}

	puts("ViOS multiboot loader active.\n");

	// Okay, the idea here is that we copy mb structures to upper mem.
	// We only change pointers (obviously) and keep the rest intact.
	// After the big copy, all these pointers will be off by a known value.
	// We only put the existing structures here, and don't even make space
	// for non-existent mb-structures.

	end = (byte *) (data = &bss_end);

	// Multiboot information structure
	memcpy(data, info, 88);
	end += 88;

	// flag 2: cmdline
	if (*data & 0x00000004)
	{
		strcpy((char *) end, (char *) data[4]);
		data[4] = (dword) end;
		end += strlen((char *) end);
		end++;
	}

	// flag 3: mods_cound / mods_addr
	if (*data & 0x00000008 && data[5])
	{
		dword * tmp = (dword *) end;
		memcpy(end, (void *) data[6], data[5] << 4);
		data[6] = (dword) tmp;
		end += data[5] << 4;
		// copy strings
		for (dword i = 0; i < data[5]; i++)
		{
			if (tmp[(i<<2)+2])
			{
				strcpy((char *) end, (char *) tmp[(i<<2)+2]);
				tmp[(i<<2)+2] = (dword) end;
				end += strlen((char *) end);
				end++;
			}
		}
	}

	// skip 4 and 5, we KNOW we are flat binary, not some ELF or a.out

	// flag 6: mmap_length / mmap_addr
	if (*data & 0x00000040)
	{
		memcpy(end, (void *) data[12], data[11] + 4);
		data[12] = (dword) end;
		end += data[11] + 4;
	}

	// flag 7: drives_length / drives_addr
	if (*data & 0x00000080 && data[13])
	{
		memcpy(end, (void *) data[14], data[13]);
		data[14] = (dword) end;
		end += data[13];
	}

	// flag 9: boot_loader_name
	if (*data & 0x00000200 && data[16])
	{
		strcpy((char *) end, (char *) data[16]);
		data[16] = (dword) end;
		end += strlen((char *) end);
		end++;
	}

	// flag 10: apm_table
	if (*data & 0x00000400)
	{
		memcpy(end, (void *) data[17], 20);
		data[17] = (dword) end;
		end += 20;
	}

	// skip 11, we know we didn't ask for vbe

	puts("Multiboot data obtained.\n");

	if (data[5] != 1)
	{
		puts("Error: number of modules is not equal to one.");
		asm ( "hlt\n\t" : : : ) ;
	}

	return data;
}
