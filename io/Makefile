include ../kernel.mk

io.o: manager/manager.o
	cp manager/manager.o io.o

manager/manager.o:
	make -C manager

.PHONEY: clean
clean:
	make -C manager clean
	rm -rf *.o
