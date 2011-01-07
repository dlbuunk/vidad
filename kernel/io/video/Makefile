include ../../../local.mk

video.o: vga.o vga_text.o
	ld -S -r -o video.o vga.o vga_text.o
	cp video.o ..

vga.o: vga.cc
	$(CXX) $(CXXFLAGS) -c -o vga.o vga.cc

vga_text.o: vga_text.cc
	$(CXX) $(CXXFLAGS) -c -o vga_text.o vga_text.cc

.PHONY: clean
clean:
	rm -f *.o