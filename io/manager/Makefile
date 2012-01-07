include ../../kernel.mk

manager.o: manager.cxx manager.hxx ../../include/util.hxx
	$(CXX) $(CXXFLAGS) -c -o manager.o manager.cxx

../../include/util.hxx: ../../kernel.hxx

.PHONEY: clean
clean:
	rm -rf *.o
