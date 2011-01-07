COMMON_FLAGS = -Wall -Wextra -O0 -pedantic
CFLAGS       = $(COMMON_FLAGS) -nostdlib -nostartfiles -nodefaultlibs -nostdinc
CXXFLAGS     = $(CFLAGS) -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector -I$(ROOT_DIR)/include
CC           = gcc -std=gnu99
CXX          = g++ -std=gnu++0x
