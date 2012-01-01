include ../kernel.mk

memory.o: page.o malloc.o
	$(LD) -r -o memory.o page.o malloc.o

page.o: page.cxx memory.hxx
	$(CXX) $(CXXFLAGS) -c -o page.o page.cxx

malloc.o: malloc.cxx memory.hxx
	$(CXX) $(CXXFLAGS) -c -o malloc.o malloc.cxx

.PHONEY: clean
clean:
	rm -rf *.o
