/* standard typedefs */
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long long int qword;

/* Info structures */
struct MemInfo
{
	qword base;
	qword length;
	dword type;
};

struct Info
{
	word mem_mid;
	word mem_high;
	word mem_alt;
	word mem_low;
	word hw_inf;
	byte magic;
	byte num_entries;
};

/* define to put a "timestamp" on the message */
#define puts(foo) (*bb_puts)("\n[0:00:00.00] "foo)

/* function pointers */
int (*bb_read_block)(byte *, int);
void (*bb_puts)(char *);
void (*bb_timer)(dword);
void (*bb_exit)(void);

/* other pointers */
struct MemInfo * meminfo;
struct Info * info;
word * GDTP;
word * IDTP;

/* a few helper functions */
extern void hexb(byte);
extern void hexw(word);
extern void hexd(dword);

/* needed for malloc-init */
extern dword end_bss;
extern void alloc_init(dword, dword);

/* kernel panic function */
void (*kpanic)(void);
void _kpanic(void)
{
	puts ("Kernel has panicked, halting");
	asm ("\tcli\n");
	asm ("\thlt\n");
}

/* kernel main function */
void kmain(int (*_bb_read_block)(byte *, int), void (*_bb_puts)(char *), void (*_bb_timer)(dword), void (*_bb_exit)(void), struct MemInfo * _meminfo, struct Info * _info, word * _GDTP, word * _IDTP)
{
	int i;

	/* set the pointers */
	bb_read_block = _bb_read_block;
	bb_puts = _bb_puts;
	bb_timer = _bb_timer;
	bb_exit = _bb_exit;
	meminfo = _meminfo;
	info = _info;
	GDTP = _GDTP;
	IDTP = _IDTP;
	kpanic = &_kpanic;

	/* print a welcome message */
	puts("VIOS version 0.00 started.");

	/* print some memory info */
	puts("Memory and hardware information:");
	puts("BIOS INT 0x11 hardware info:           ");
	hexw(info->hw_inf);
	puts("Low memory present (KiB):              ");
	hexw(info->mem_low);
	puts("Middle memory present (KiB):           ");
	hexw(info->mem_mid);
	puts("Alternative high memory (KiB):         ");
	hexw(info->mem_alt);
	puts("High memory present (KiB):         ");
	hexd(info->mem_high<<6);
	if (info->num_entries)
	{
		puts("Memory block details:");
		for (i=0; i<info->num_entries; i++)
		{
			puts("Block starting at ");
			hexd((dword) (meminfo+i)->base);
			(*bb_puts)(" has size ");
			hexd((dword) (meminfo+i)->length);
			(*bb_puts)(" with type ");
			hexb((byte) (meminfo+i)->type);
		}
	}

	/* we don't need the hw anymore, stop it */
	puts("Stopping bootloader-provided floppy-subsystem.");
	(*bb_exit)();

	/* init malloc */
	puts("Initialising dynamic memory allocation");
	alloc_init(end_bss, ((dword)info->mem_low)<<10);
}
