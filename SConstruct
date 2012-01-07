execfile("SConset")
env.Replace(CPPPATH="include")



env.SConscript("util/SConstruct")
env.SConscript("system/SConstruct")
env.SConscript("memory/SConstruct")
env.SConscript("thread/SConstruct")


kernel = env.Clone()
kernel.Replace(LINKFLAGS = "-T kernel.ld")
kernel.Program("kernel.binx", ["kernel_entry.s", "int_entry.cxx", "c_entry.c", "cxx_entry.cxx", \
		"util/util.o", "system/system.o", "memory/memory.o", "thread/thread.o"])
kernel.Depends("kernel.binx", "kernel.ld")
kernel.Command("kernel.bin", "kernel.binx", "chmod -x kernel.binx && cp kernel.bin{x,}")



env.Command("boot_block.bin", "", "cd bootloader && scons && cp boot_block.bin .. && cd ..")
env.Command("page_init.bin", "", "cd bootloader && scons && cp page_init.bin .. && cd ..")
if GetOption("clean"):
    env.SConscript("bootloader/SConstruct")



fs = env.Clone()
fs.Replace(LINKFLAGS = "-T fs.ld")
fs.Program("fs.binx", ["fs.s"])
fs.Depends("fs.binx", "fs.ld")
fs.Command("fs.bin", "fs.binx", "chmod -x fs.binx && cp fs.bin{x,}")

env.Command("image", ["boot_block.bin", "page_init.bin", "fs.bin", "kernel.bin"], \
	"cp boot_block.bin image && \
	dd of=image seek=08 if=fs.bin && \
	dd of=image seek=24 if=page_init.bin && \
	dd of=image seek=32 if=kernel.bin && \
	dd of=image seek=2879 count=1 if=/dev/zero")
