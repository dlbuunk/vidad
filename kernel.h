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
	word mem_mid;
	word mem_high;
	word mem_alt;
	word mem_low;
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
extern void klog_init(int, void (*)(char *));
extern void kprint(char const *, ...);

