include ../kernel.mk

util.o: string.o kprint.o
	$(LD) -r -o util.o string.o kprint.o

string.o: string.cxx
	$(CXX) $(CXXFLAGS) -c -o string.o string.cxx

kprint.o: kprint.cxx
	$(CXX) $(CXXFLAGS) -c -o kprint.o kprint.cxx

.PHONEY: clean
clean:
	rm -rf *.o
