LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as
CC = /usr/local/cross/bin/i386-elf-gcc
CXX= /usr/local/cross/bin/i386-elf-g++

CCFLAGS = -nostdlib -nostartfiles -nodefaultlibs -fno-builtin \
	-Wall -Wextra -Werror -std=gnu99 -O3
CXXFLAGS= -nostdlib -nostartfiles -nodefaultlibs -fno-builtin \
	-fno-exceptions -fno-rtti -fno-stack-protector \
	-Wall -Wextra -Werror -std=gnu++98 -O3
