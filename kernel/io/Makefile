include ../../local.mk

io.o: init.o pic.o timer.o force_look
	make -C video
	make -C keyboard
	make -C terminal
	ld -S -r -o io.o init.o pic.o timer.o video.o keyb.o terminal.o
	cp io.o ..

init.o: init.cc
	$(CXX) $(CXXFLAGS) -c -o init.o init.cc

pic.o: pic.cc
	$(CXX) $(CXXFLAGS) -c -o pic.o pic.cc

timer.o: timer.cc
	$(CXX) $(CXXFLAGS) -c -o timer.o timer.cc

.PHONY: clean
clean:
	rm -f *.o
	make -C video clean
	make -C keyboard clean
	make -C terminal clean

.PHONY: force_look
force_look:
	true
