include ../kernel.mk

system.o: system.cxx system.hxx
	$(CXX) $(CXXFLAGS) -c -o system.o system.cxx

system.hxx: ../kernel.hxx

.PHONEY: clean
clean:
	rm -rf *.o
