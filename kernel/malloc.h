extern dword alloc_base;
extern void alloc_init(dword heap_base, dword top_of_mem);
extern void alloc_error();
extern void *malloc(dword size);
extern void *calloc(dword n, dword size);
extern void free(void *block);
