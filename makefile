TARGET = cosmic_blaster
CC = gcc
SRCDIR = src
INCDIR = include

CFLAGS = -Wall -std=c99 -I$(INCDIR) `pkg-config --cflags raylib`

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	LIBS = `pkg-config --libs raylib` -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(UNAME_S), Darwin)
	LIBS = `pkg-config --libs raylib` -lm -lpthread \
		-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

$(SRCDIR)/game.o: $(INCDIR)/game.h

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)