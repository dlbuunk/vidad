include ../../../local.mk

keyb.o: kbc.o key_translate.o keyboard.o
	ld -S -r -o keyb.o kbc.o key_translate.o keyboard.o
	cp keyb.o ..

kbc.o: kbc.cc
	$(CXX) $(CXXFLAGS) -c -o kbc.o kbc.cc

keyboard.o: keyboard.cc
	$(CXX) $(CXXFLAGS) -c -o keyboard.o keyboard.cc

key_translate.o: key_translate.cc
	$(CXX) $(CXXFLAGS) -c -o key_translate.o key_translate.cc


.PHONY: clean
clean:
	rm -f *.o
