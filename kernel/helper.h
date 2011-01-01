typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long int qword;

extern void memstob(dword addr, dword count, byte val);
extern void memstow(dword addr, dword count, word val);
extern void memcpyb(dword to, dword count, dword from);
extern void memcpyw(dword to, dword count, dword from);
extern byte inportb(word port);
extern void outportb(word port, byte val);
