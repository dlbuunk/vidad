typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long long int qword;

struct MemInfo
{
	qword base;
	qword length;
	dword type;
};

struct Info
{
	word mem_mid;	// kb
	word mem_high;	// 64 kb
	word mem_alt;	// kb
	word mem_low;	// kb
	word hw_inf;
	byte magic;
	byte num_entries;
};

//defined in kernel.ld
extern dword start_ctors;
extern dword end_ctors;
extern dword start_dtors;
extern dword end_dtors;
extern dword start_bss;
extern dword end_bss;

// defined in kinit.cxx
// extern "C" void kinit( ... );
extern dword const volatile ktime;
extern dword boot_time;

// defined in kprint.cxx
extern void klog_init(void (*)(char *));
extern void klog_init();
extern void kprint(char const *, ...);

// defined in malloc.cxx
// new, new[], delete, delete[]
extern void alloc_init(void *, void *);
extern dword alloc_status();
extern void * malloc(dword);
extern void free(void *);

