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
kernel.Command("kernel.bin", "kernel.binx", "chmod -x kernel.binx; cp kernel.bin{x,}")



env.Command("boot_block.bin", "", "cd bootloader; scons; cp boot_block.bin ..; cd ..")
if GetOption("clean"):
    env.SConscript("bootloader/SConstruct")
