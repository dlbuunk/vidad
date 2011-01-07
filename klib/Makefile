all: force_look
	make -C string
	make -C cstring
	mkdir -p lib/
	make -C string installlib
	make -C cstring installlib
	

.PHONY: clean
clean:
	make -C string clean
	make -C cstring clean
	rm -f lib/*.{a,o}

.PHONY: force_look
force_look:
	true
