COMMON_FLAGS = -Wall -Wextra -O0 -pedantic
CFLAGS       = $(COMMON_FLAGS) -nostdlib -nostartfiles -nodefaultlibs -nostdinc \
		-I$(ROOT_DIR)/include
CXXFLAGS     = $(CFLAGS) -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector
CC           = gcc -std=gnu99
CXX          = g++ -std=gnu++0x
