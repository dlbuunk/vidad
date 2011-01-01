#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define BROWN 6
#define W 7
#define GRAY 8
#define YELLOW 14
#define WHITE 15

#define WHITE_BLUE 0x1F
#define WHITE_GREEN 0x2F
#define WHITE_RED 0x4F

extern word kprint_pos;
extern void kprint(char ch, byte color);
extern void kprints(char *str, byte color);
extern void kprintb(byte val, byte color);
extern void kprintw(word val, byte color);
extern void kprintd(dword val, byte color);
