SRC = src
#BUILD = build

#SOURCES = main.cpp ui_widget.cpp ui_instance.cpp
SOURCES = main.c interpreter.c
#OBJECTS = $(patsubst %.c,%.o, $(SOURCES))
#OBJECTS += $(patsubst %.cpp,%.o, $(SOURCES))

CC = gcc
#CC = g++
#CPPFLAGS = -O0 -ggdb -Wall -Wextra #-I$(SRC)
CPPFLAGS = -O3 -Wall -Wextra
LIBS = -lc

.PHONY: Makefile

brainfunk: $(SOURCES)
	$(CC) $(SOURCES) $(CPPFLAGS) $(LIBS) -o $@

#$(BUILD)/$(OBJECTS): $(SOURCES)
#	$(CC) -c $(CFLAGS) $(SRC)/$< -o $(BUILD)/$@

#$(BUILD):
#	mkdir $(BUILD)

.PHONY: clean
clean:
	rm brainfunk
