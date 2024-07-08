CC = gcc
CFLAGS = -g -Wall -Wextra -Iinclude $(shell pkg-config --cflags glib-2.0)

LIBS = $(shell pkg-config --libs glib-2.0)

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/firewall_cli

$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS)  $^ $(LIBS) -o $@
	@mkdir -p $(BINDIR)
	@mv $@ $(BINDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(BINDIR) 

.PHONY: clean



