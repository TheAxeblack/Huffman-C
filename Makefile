# Compiler and linker options
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -I./inc
LDFLAGS =

# Directories
SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = ./inc

# Source and object files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# Executable file
EXEC = $(BINDIR)/my_program

# Default target
all: $(EXEC)

# Link object files to create executable
$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object and executable files
clean:
	rm -f $(OBJDIR)/*.o $(EXEC)
