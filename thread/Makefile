include ../kernel.mk

thread.o: thread.cxx thread.hxx	../include/system.hxx \
		thread.hxx ../include/memory.hxx ../include/util.hxx
	$(CXX) $(CXXFLAGS) -c -o thread.o thread.cxx


thread.hxx ../include/memory.hxx: ../kernel.hxx
../include/system.hxx ../include/util.hxx: ../kernel.hxx

.PHONEY: clean
clean:
	rm -rf *.o
