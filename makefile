CC       = clang++
CFLAGS   = -Wall -Wextra -pedantic-errors -std=c++17 -O2
LFLAGS   = -lsfml-system -lsfml-window -lsfml-graphics

SRCDIR   = ./src
BINDIR   = ./bin
OBJDIR   = ./obj

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJS     = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCFILES))

ONAME    = 'chip8'

default: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $(BINDIR)/$(ONAME)


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm $(OBJDIR)/*.o
