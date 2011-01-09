include ../../../local.mk

keyb.o: kbc.o key_translate1.o key_translate2.o keyboard.o
	ld -S -r -o keyb.o kbc.o key_translate1.o key_translate2.o keyboard.o
	cp keyb.o ..

kbc.o: kbc.cc
	$(CXX) $(CXXFLAGS) -c -o kbc.o kbc.cc

keyboard.o: keyboard.cc
	$(CXX) $(CXXFLAGS) -c -o keyboard.o keyboard.cc

key_translate1.o: key_translate1.cc
	$(CXX) $(CXXFLAGS) -c -o key_translate1.o key_translate1.cc

key_translate2.o: key_translate2.cc
	$(CXX) $(CXXFLAGS) -c -o key_translate2.o key_translate2.cc

.PHONY: clean
clean:
	rm -f *.o
