TARGET := chip8

CC := g++

FLAGS  := -Wall -Wextra -pedantic-erros
LFLAGS :=

SRCDIR := ./src
BINDIR := ./bin

OBJS := $(SRCDIR)/*.cpp

default: $(OBJS)
	$(CC) $^ $(FLAGS) -o $(BINDIR)/$(TARGET)

