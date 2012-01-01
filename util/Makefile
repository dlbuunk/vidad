include ../kernel.mk

util.o: string.o kprint.o
	$(LD) -r -o util.o string.o kprint.o

string.o: string.cxx util.hxx
	$(CXX) $(CXXFLAGS) -c -o string.o string.cxx

kprint.o: kprint.cxx util.hxx
	$(CXX) $(CXXFLAGS) -c -o kprint.o kprint.cxx

util.hxx: ../kernel.hxx

.PHONEY: clean
clean:
	rm -rf *.o
