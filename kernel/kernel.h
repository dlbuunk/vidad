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

/* function in kmain.c */
extern void (*kpanic)(void);
extern void _kpanic(void);

/* function pointers (from kmain.c) */
extern int (*bb_read_block)(byte *, int);
extern void (*bb_puts)(char *);
extern void (*bb_timer)(dword);
extern void (*bb_exit)(void);

/* other pointers (from kmain.c) */
extern struct MemInfo * meminfo;
extern struct Info * info;
extern word * GDTP;
extern word * IDTP;

/* helper.c functions */
extern void hexb(byte);
extern void hexw(word);
extern void hexd(dword);

extern void memstob(byte *, byte, dword);

/* apart from the functions here mentioned, there are:
void kmain( ... ) in kmain.c
and (all in malloc.c) :
extern void * malloc(dword);
extern void * calloc(dword, dword);
extern void free(void *);
extern void alloc_init(dword, dword);
extern dword alloc_base;
extern dword_alloc_top;
*/
