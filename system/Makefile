include ../kernel.mk

system.o: system.cxx system.hxx ../include/thread.hxx ../include/util.hxx
	$(CXX) $(CXXFLAGS) -c -o system.o system.cxx

system.hxx ../include.hxx ../include/util.hxx: ../kernel.hxx

.PHONEY: clean
clean:
	rm -rf *.o
