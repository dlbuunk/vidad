include ../kernel.mk

thread.o: thread.cxx thread.hxx
	$(CXX) $(CXXFLAGS) -c -o thread.o thread.cxx

thread.hxx: ../kernel.hxx

.PHONEY: clean
clean:
	rm -rf *.o
