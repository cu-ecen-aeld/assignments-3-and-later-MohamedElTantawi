# Set the compiler to use
CC = gcc

# Set the default target
all: writer

# Build the writer application
writer: writer.c
	$(CC) -o writer writer.c

# Clean target to remove the writer application and all .o files
clean:
	rm -f writer *.o

# Support for cross-compilation
ifneq ($(CROSS_COMPILE),)
	CC = $(CROSS_COMPILE)gcc
endif
