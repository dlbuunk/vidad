include ../../../local.mk

terminal.o: term_buf.o term_std.o
	ld -S -r -o terminal.o term_buf.o term_std.o
	cp terminal.o ..

term_buf.o: term_buf.cc
	$(CXX) $(CXXFLAGS) -c -o term_buf.o term_buf.cc

term_std.o: term_std.cc
	$(CXX) $(CXXFLAGS) -c -o term_std.o term_std.cc

.PHONY: clean
clean:
	rm -f *.o
