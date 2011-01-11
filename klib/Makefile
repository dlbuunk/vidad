# Copyright:
#   This file is part of vidad::klib.
#
#   vidad::klib is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as
#   published by the Free Software Foundation, either version 3 of the
#   License, or (at your option) any later version.
#
#   vidad::klib is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with vidad::klib.  If not, see <http://www.gnu.org/licenses/>.
#
all: force_look
	make -C string
	make -C cstring
	mkdir -p ../lib/
	make -C string installlib
	make -C cstring installlib

doc:
	doxygen

.PHONY: clean
clean:
	make -C string clean
	make -C cstring clean
	rm -f ../lib/*.{a,o}
	rm -rf ../doc/klib/

.PHONY: force_look
force_look:
	true
