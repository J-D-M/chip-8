TARGET := out

CC := g++

FLAGS ?= -Wall

SRCDIR ?= ./src
BINDIR ?= ./bin

OBJS ?= $(SRCDIR)/*.cpp

default: $(OBJS)
	$(CC) $^ $(FLAGS) -o $(BINDIR)/$(TARGET)

