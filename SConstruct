execfile("SConset")
env.Replace(CPPPATH="include")


env.SConscript("util/SConstruct")
env.SConscript("system/SConstruct")
env.SConscript("memory/SConstruct")
env.SConscript("thread/SConstruct")
env.SConscript("io/SConstruct")


kernel = env.Clone()
kernel.Replace(LINKFLAGS = "-T kernel.ld")
kernel.Program("kernel.binx", ["kernel_entry.s", "int_entry.cxx", "c_entry.c", "cxx_entry.cxx", \
		"util/util.o", "system/system.o", "memory/memory.o", "thread/thread.o", "io/io.o"])
kernel.Depends("kernel.binx", "kernel.ld")
kernel.Command("kernel.tmp", "kernel.binx", "chmod -x kernel.binx && cp kernel.binx kernel.tmp")


page_init = env.Clone()
page_init.Replace(LINKFLAGS = "-T page_init.ld")
page_init.Program("page_init.binx", ["kheader.s", "page_init.c"])
page_init.Depends("page_init.binx", "page_init.ld")
page_init.Command("page_init.tmp", "page_init.binx", "chmod -x page_init.binx && cp page_init.binx page_init.tmp")


env.Command("kernel.bin", ["kernel.tmp", "page_init.tmp"], \
	"cp page_init.tmp kernel.bin && \
	dd of=kernel.bin seek=08 if=kernel.tmp")
